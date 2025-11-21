#include "vpsCnx.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "bmp280.h"

void vpsTask(void *parameter) {
    WiFiClient wifiClient;
    PubSubClient mqttClient(wifiClient);

    const char* ssid = "A52";
    const char* password = "1111111188";

    const char* mqtt_server = "192.168.186.167";
    const int mqtt_port = 1883;
    const char *token_bmp280 = "WfwVfLsPVhvDE7YlKXCA";

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wifi");
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }
    Serial.println("\nWifi connected");

    mqttClient.setServer(mqtt_server, mqtt_port);
    BmpData bmpData;

    for (;;) {
        while (!mqttClient.connected()) {
            mqttClient.connect("GroundModule", token_bmp280, "");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        mqttClient.loop();

        if (bmpStore.read(bmpData)) {
            char jsonPayLoad[256];
            snprintf(jsonPayLoad, sizeof(jsonPayLoad), "{\"temperature\": %.2f,\"pressure\": %.2f,\"altitude\": %.2f}",
                bmpData.temperature, bmpData.pressure, bmpData.altitude);
            mqttClient.publish("v1/devices/me/telemetry", jsonPayLoad);
            Serial.println(jsonPayLoad);
            Serial.println("Published.");
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}