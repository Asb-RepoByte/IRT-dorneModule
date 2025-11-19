// tft_test.ino
#include "st7789_display.h"
#include "tft_setup.h"
#include <TFT_eSPI.h>

void st7789Task(void *parameter) {

    TFT_eSPI tft = TFT_eSPI();

    const int cw = tft.width() / 2;
    const int ch = tft.height() / 2;
    const int s = min(cw / 4, ch / 4);

    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);

    tft.setTextFont(1);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(CC_DATUM);
    tft.drawString("Makerguides", ch, cw + s);

    tft.fillCircle(ch, cw / 2 + s / 2, s / 2, TFT_RED);
    tft.fillRect(1.5 * ch - s, cw / 2, s, s, TFT_GREEN);
    tft.fillTriangle(ch / 2, cw / 2, ch / 2 + s, cw / 2, ch / 2, cw / 2 + s, TFT_BLUE);

    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}