#include "config_proj.h"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "cnx.h"
#include "bmp280.h"
#include "packer.h"
#include "threadSafeSensor.h"

extern bool role;
uint8_t address[][6] = { "1Node", "2Node" };


void droneCnxTask(void *parameter) {
    // instantiate an object for the nRF24L01 transceiver
    RF24 radio(CE_PIN, CSN_PIN);

    // initialize the transceiver on the SPI bus
    if (!radio.begin())
    {
        Serial.println(F("radio hardware is not responding!!"));
        while (1)
        {
        } // hold in infinite loop
    }

    // print example's introductory prompt
    Serial.println(F("RF24/examples/GettingStarted"));

    Serial.print(F("radioNumber = "));
    Serial.println((int)role);

    // Set the PA Level low to try preventing power supply related problems
    // because these examples are likely run with nodes in close proximity to
    // each other.
    radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.
    radio.stopListening(); // put radio in TX mode
    radio.openReadingPipe(1, address[!role]); // using pipe 1

    BmpData bmpData;
    Packet packet;
    bool report = false;
    unsigned long start_timer, end_timer;
    for (;;)
    {
        if (bmpStore.read(bmpData)) {
            start_timer = micros();               // start the timer
            packBmp280(packet, &bmpData);
            report = radio.write(&packet, sizeof(packet)); // transmit & save the report
            end_timer = micros();                 // end the timer

            if (report)
            {
                Serial.print(F("Transmission successful! ")); // payload was delivered
                Serial.print(F("Time to transmit = "));
                Serial.print(end_timer - start_timer); // print the timer result
                Serial.print(F(" us. Sent: bmpData"));
            }
            else
            {
                Serial.println(F("Transmission failed or timed out")); // payload was not delivered
            }
        }

        // to make this example readable in the serial monitor
        vTaskDelay(1000 / portTICK_PERIOD_MS); // slow transmissions down by 1 second
    }
} 


void groundCnxTask(void *parameter) {
    // instantiate an object for the nRF24L01 transceiver
    SPIClass hspi(HSPI);
    hspi.begin(18, 19, 23, -1);
    RF24 radio(CE_PIN, CSN_PIN);

    // initialize the transceiver on the SPI bus
    if (!radio.begin(&hspi))
    {
        Serial.println(F("radio hardware is not responding!!"));
        while (1)
        {
        } // hold in infinite loop
    }

    // print example's introductory prompt
    Serial.println(F("RF24/examples/GettingStarted"));

    Serial.print(F("radioNumber = "));
    Serial.println((int)role);

    radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.

    // set the TX address of the RX node for use on the TX pipe (pipe 0)
    radio.stopListening(); // put radio in TX mode

    // set the RX address of the TX node into a RX pipe
    radio.openWritingPipe(address[role]);
    radio.openReadingPipe(1, address[!role]); // using pipe 1
    radio.startListening(); // put radio in RX mode

    BmpData bmpData;
    Packet packet;

    for (;;)
    {
        // This device is a RX node
        uint8_t pipe;
        if (radio.available(&pipe))
        {                                           // is there a payload? get the pipe number that received it
            uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
            radio.read(&packet, bytes);            // fetch payload from FIFO
            Serial.print(F("Received "));
            Serial.print(bytes); // print the size of the payload
            Serial.print(F(" bytes on pipe "));
            Serial.print(pipe); // print the pipe number
            Serial.print(F(": "));
            
            unpackBmp280(packet, bmpData);
            bmpStore.write(bmpData);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}