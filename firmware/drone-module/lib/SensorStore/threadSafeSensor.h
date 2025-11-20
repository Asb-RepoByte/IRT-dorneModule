#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

template <typename T>
struct ThreadSafeSensor {
    ThreadSafeSensor()
    : updated{ false }
    {
        mutex = xSemaphoreCreateMutex();
    }
    // write new sensor data from sensor to back
    void write(const T &in) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        value = in;
        updated = true; // set the flag
        xSemaphoreGive(mutex);
    }

    bool read(T &out) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        if (!updated) {
            xSemaphoreGive(mutex);
            return false;
        }
        out = value;
        updated = false;
        xSemaphoreGive(mutex);
        
        return true;
    }

private:
    T value;
    bool updated = false;
    xSemaphoreHandle mutex = xSemaphoreCreateMutex();
};