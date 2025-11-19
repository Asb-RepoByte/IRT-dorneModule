#include "config_proj.h"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "cnx.h"


extern bool role;
uint8_t address[][6] = { "1Node", "2Node" };

void groundCnxTask(void *parameter) {
    // instantiate an object for the nRF24L01 transceiver
    RF24 radio(CE_PIN, CSN_PIN);
    // For this example, we'll be using a payload containing
    // a single float number that will be incremented
    // on every successful transmission
    float payload = 0.0;

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

    // save on transmission time by setting the radio to only transmit the
    // number of bytes we need to transmit a float
    radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

    // set the TX address of the RX node for use on the TX pipe (pipe 0)
    radio.stopListening(address[role]); // put radio in TX mode

    // set the RX address of the TX node into a RX pipe
    radio.openReadingPipe(1, address[!role]); // using pipe 1

    for (;;)
    {
        unsigned long start_timer = micros();               // start the timer
        bool report = radio.write(&payload, sizeof(float)); // transmit & save the report
        unsigned long end_timer = micros();                 // end the timer

        if (report)
        {
            Serial.print(F("Transmission successful! ")); // payload was delivered
            Serial.print(F("Time to transmit = "));
            Serial.print(end_timer - start_timer); // print the timer result
            Serial.print(F(" us. Sent: "));
            Serial.println(payload); // print payload sent
            payload += 0.01;         // increment float payload
        }
        else
        {
            Serial.println(F("Transmission failed or timed out")); // payload was not delivered
        }

        // to make this example readable in the serial monitor
        vTaskDelay(1000 / portTICK_PERIOD_MS); // slow transmissions down by 1 second
    }
} 


void droneCnxTask(void *parameter) {
    // instantiate an object for the nRF24L01 transceiver
    RF24 radio(CE_PIN, CSN_PIN);
    // For this example, we'll be using a payload containing
    // a single float number that will be incremented
    // on every successful transmission
    float payload = 0.0;

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

    // save on transmission time by setting the radio to only transmit the
    // number of bytes we need to transmit a float
    radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

    // set the TX address of the RX node for use on the TX pipe (pipe 0)
    radio.stopListening(address[role]); // put radio in TX mode

    // set the RX address of the TX node into a RX pipe
    radio.openReadingPipe(1, address[!role]); // using pipe 1

    radio.startListening(); // put radio in RX mode

    for (;;)
    {
        // This device is a RX node
        uint8_t pipe;
        if (radio.available(&pipe))
        {                                           // is there a payload? get the pipe number that received it
            uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
            radio.read(&payload, bytes);            // fetch payload from FIFO
            Serial.print(F("Received "));
            Serial.print(bytes); // print the size of the payload
            Serial.print(F(" bytes on pipe "));
            Serial.print(pipe); // print the pipe number
            Serial.print(F(": "));
            Serial.println(payload); // print the payload's value
        }
    }
}