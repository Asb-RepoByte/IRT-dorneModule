#pragma once

#include "threadSafeSensor.h"
struct BmpData {
    float temperature;
    float pressure;
    float altitude;
};

extern ThreadSafeSensor<BmpData> bmpStore;

void bmp280Task(void *parameter);