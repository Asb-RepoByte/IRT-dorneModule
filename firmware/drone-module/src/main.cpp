#include <Arduino.h>
#include "config_proj.h"
#include "scanner.h"
#include "st7789_display.h"
#include "protocol_common.h"
#include "bmp280.h"
#include "cnx.h"
#include "vpsCnx.h"

// Answeres the question am I the ground module?
bool role = true; // true for ground module, false for drone module
uint8_t msg_id_counter = 0;

void setup() {
  pinMode(ROLE_PIN, INPUT);
  role = digitalRead(ROLE_PIN);

  Serial.begin(115200);

  if (role) {
    Serial.println("This is the ground module");
    // include the tasked for ground module
    xTaskCreate(groundCnxTask, "groundCnx", 4096, NULL, 1, NULL);
    xTaskCreate(st7789Task, "st7789", 4096, NULL, 1, NULL);
    xTaskCreate(vpsTask, "vsp", 4096, NULL, 1, NULL);

  } else {
    Serial.println("This is the drone module");
    // include the tasked for drone module
    xTaskCreate(bmp280Task, "bmp280", 4096, NULL, 1, NULL);
    xTaskCreate(droneCnxTask, "droneCnx", 4096, NULL, 1, NULL);

  }

}

void loop() {
}