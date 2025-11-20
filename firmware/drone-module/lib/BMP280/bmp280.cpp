#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include "threadSafeSensor.h"
#include "bmp280.h"

ThreadSafeSensor<BmpData> bmpStore;

void bmp280Task(void *parameter) {
    Adafruit_BMP280 bmp; // I2C
    Serial.println(F("BMP280 test"));
    unsigned status;
    // status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
    status = bmp.begin(0x76);
    if (!status)
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                         "try a different address!"));
        while (1)
            delay(10);
    }

    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    BmpData bmpData;
    float temp, pressure, altitude;
    for (;;)
    {
        Serial.print(F("Temperature = "));
        temp = bmp.readTemperature();
        Serial.print(temp);
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        pressure = bmp.readPressure();
        Serial.print(pressure);
        Serial.println(" Pa");

        Serial.print(F("Approx altitude = "));
        altitude = bmp.readAltitude(1013.25);
        Serial.print(altitude); /* Adjusted to local forecast! */
        Serial.println(" m");

        Serial.println();

        bmpData.temperature = temp;
        bmpData.pressure =  pressure;
        bmpData.altitude = altitude;

        bmpStore.write(bmpData);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}