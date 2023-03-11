#include <Arduino.h>
#include "Motor.h"

#define UPDATE_FREQ 20
#define UPDATE_INTERVAL_ms (1000 / UPDATE_FREQ)

Motor motor[] = {{5, 4, 3, A0}, {8, 7, 9, A1}, {12, 11, 10, A2}};

void receive_target();

unsigned long last_update = 0;

void setup() {
  TCCR2B = (TCCR2B | 1) & ~B110;
  TCCR1B = (TCCR1B | 1) & ~B110;
  // motor[1].calibration = {67, 893};

  Serial.begin(9600);
  Serial.setTimeout(60);
  // wait for other side
  while (!Serial.available()) continue;
  // Throw away whatever came
  while (Serial.available()) Serial.read();
  Serial.println("A, B, C");
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
