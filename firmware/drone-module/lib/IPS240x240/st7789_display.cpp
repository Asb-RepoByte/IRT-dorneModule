// tft_test.ino
#include "st7789_display.h"
#include "tft_setup.h"
#include <TFT_eSPI.h>
#include "bmp280.h"

void st7789Task(void *parameter) {
    TFT_eSPI tft = TFT_eSPI();

    const int cw = tft.width();
    const int ch = tft.height();

    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);

    tft.setTextFont(1);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);


    uint8_t offsetX = 20;
    uint8_t offsetY = 20;
    BmpData BmpData;
    for (;;) {
        bmpStore.read(BmpData);
        int16_t temp_width = tft.drawString("Temperature: ", offsetX, offsetY);
        tft.drawFloat(BmpData.temperature, 2, offsetX + temp_width - 10, offsetY);
        offsetY += 50;

        temp_width = tft.drawString("Pressure: ", offsetX, offsetY);
        tft.drawFloat(BmpData.pressure, 2, offsetX + temp_width - 10, offsetY);
        offsetY += 50;

        temp_width = tft.drawString("altitude: ", offsetX, offsetY);
        tft.drawFloat(BmpData.altitude, 2, offsetX + temp_width - 10, offsetY);

        temp_width = 0;
        offsetX = 20;
        offsetY = 20;

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}