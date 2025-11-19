#include "scanner.h"
#include "config_proj.h"


void scanTask(void *parameter) {

    RF24 radio{CE_PIN, CSN_PIN};
    //
    // Channel info
    //

    static const char* SCAN = "SCANNER";

    const uint8_t num_channels = 126; // 0-125 are supported
    uint8_t values[num_channels];     // the array to store summary of signal counts per channel

    // To detect noise, we'll use the worst addresses possible (a reverse engineering tactic).
    // These addresses are designed to confuse the radio into thinking
    // that the RF signal's preamble is part of the packet/payload.
    const uint8_t noiseAddress[][2] = {{0x55, 0x55}, {0xAA, 0xAA}, {0xA0, 0xAA}, {0xAB, 0xAA}, {0xAC, 0xAA}, {0xAD, 0xAA}};

    const int num_reps = 100;  // number of passes for each scan of the entire spectrum
    //bool constCarrierMode = 0; // this flag controls example behavior (scan mode is default)
    SPIClass *hspi = new SPIClass();
    hspi->begin(14, 12, 13, -1);

    if (!radio.begin(hspi)) {
        ESP_LOGE(SCAN, "nRF24 hardware not responding!");
        while (true) {
            // holding infinite loop in case of failure
        }
    }

    Serial.println("hardware check passed");

    //radio.stopConstCarrier(); // in case MCU was reset while in const carrier mode
    radio.setAutoAck(false); // Don't acknowldge arbitrary signals
    radio.disableCRC(); // Accept any signal the moduel find
    radio.setAddressWidth(2); // A reverse engineering tactic (not recommended for normal use)
    for (uint8_t i = 0; i < 6; i++) {
        radio.openReadingPipe(i, noiseAddress[i]);
    }

    // set the data rate
    radio.setDataRate(RF24_1MBPS);

    // Get in standby mode
    radio.startListening();
    radio.stopListening();
    radio.flush_rx();

    ESP_LOGI(SCAN, "Started Scanning...");
    Serial.println("Started Scanning...");

    for (;;) {
        memset(values, 0, sizeof(values));
        int rep_counter = num_reps;
        while (rep_counter--) {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            int i = num_channels;
            while (i--) {
                // select this channel
                radio.setChannel(i);

                // Listen for a little
                radio.startListening();
                delayMicroseconds(128);
                bool foundSignal = radio.testRPD();
                radio.stopListening();

                if (foundSignal || radio.testRPD() || radio.available()) {
                    ++values[i];
                    radio.flush_rx(); // sicard packets of noise
                }
            }
        }

        // print out channel measurements, calmped down to a single hex
        for (int i = 0; i < num_channels; ++i) {
            if (values[i])
                Serial.print(min((uint8_t)0xF, values[i]), HEX);
            else
                Serial.print(F("-"));
        }
        Serial.println();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

}