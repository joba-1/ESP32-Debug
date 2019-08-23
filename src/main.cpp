/*
Nothing debug related in here. Look into platformio.ini.
This is just a small blink sketch that is noisy on serial and on a gpio.
It uses a local, static variable so one can play with it during debugging.
*/

#include <Arduino.h>

#define LED_PIN LED_BUILTIN

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static unsigned long count = 0;

  Serial.print(__TIMESTAMP__ "  ");
  Serial.print(++count);
  Serial.print(" blink on io");
  Serial.println(LED_PIN);

  digitalWrite(LED_PIN, digitalRead(LED_PIN) == HIGH ? LOW : HIGH);

  delay(500);
}