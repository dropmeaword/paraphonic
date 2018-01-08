#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Arduino.h>
#include <WiFiUdp.h>

#include "config.h"
#include "networking.h"
#include "webserver.h"

#include "log.h"
#include "motor.h"
#include "ledstrip.h"

void setup()
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  motor_init();

  ledstrip_init();
  ledstrip_test_pattern();

  wifi_init();
  web_init();
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop()
{
  retry.run();
  
  motor_loop();
  ledstrip_loop();
  web_loop();
}