#include <Arduino.h>
#include "Motor.h"
#include "Blink.h"
#include "pinout.h"

#define UPDATE_FREQ 20
#define UPDATE_INTERVAL_ms (1000 / UPDATE_FREQ)

Motor motor[] = {{17, 5, 18, 34}, {16, 4, 2, 35}, {26, 21, 19, 36}};

void receive_target();


unsigned long last_update = 0;

void setup() {

  Serial.begin(115200);
  Serial.setTimeout(10);
  Blink::setup();
  // wait for other side
  while (!Serial.available()) {
    Blink::update(500);
  };
  pinMode(2, HIGH);
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
