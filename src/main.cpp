#include <Arduino.h>
#include "Motor.h"
#include "Communication.h"
#include "pinout.h"

#define UPDATE_FREQ 20
#define UPDATE_INTERVAL_ms (1000 / UPDATE_FREQ)

Motor motor[] = {
  // {AIN1, AIN2, PWMA, POS_A},
  {BIN1, BIN2, PWMB, POS_B},
  {CIN1, CIN2, PWMC, POS_C},
  {DIN1, DIN2, PWMD, POS_D}
};

void receive_target();

unsigned long last_update = 0;

void setup() {
  Communication::setup();
}

void loop() {
  receive_target();
  
  for (size_t i = 0; i < 3; i++) {
    motor[i].control();
  }
  
  unsigned long now = millis();
  if (now - last_update > UPDATE_INTERVAL_ms) {
    last_update = now;
    Serial.println(motor[0].get_position() + ", " + motor[1].get_position() + ", " + motor[2].get_position());
  }
}

void receive_target() {
  // Implementation not finished
  if (!Serial.available()) return;
  int option = Serial.read() - 'A';
  motor[option].set_target(Serial.parseInt());
}
