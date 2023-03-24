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

  Serial.begin(115200);
  Serial.setTimeout(5);
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

  int option = Serial.read();
  if ('A' <= option & option <= 'C') {
    option -= 'A';
    motor[option].set_target(Serial.parseInt());
  } else if (option == 'X'){
    int x = Serial.parseInt();
    int y = Serial.parseInt();
    int z = Serial.parseInt();
    inverse_kinematics(x, y, z);
  }
  
}

void inverse_kinematics(int x, int y, int z) {

  // if (invalid_coordinates(x, y, z)) return;

  double base_angle = atan2(y, x);
  int chord_sq = x*x + y*y + z*z;
  double link_angle = acos((12800 - chord_sq) / 12800.);
  double planar_base = atan2(z, sqrt(x*x + y*y));
  planar_base += (PI - link_angle) / 2.;
  link_angle += planar_base;
  motor[0].set_target(base_angle);
  motor[1].set_target(planar_base);
  motor[2].set_target(link_angle);
}
    // """Inverse kinematics of robotic arm"""
    // base_angle = math.atan2(y, x)
    // chord_sq = x**2 + y**2 + z**2
    // link_angle = math.acos((128 - chord_sq) / 128)
    // planar_base = math.atan2(z, math.sqrt(x**2 + y**2))
    // planar_base += (math.pi - link_angle) / 2
    // link_angle += planar_base
    // return (math.degrees(base_angle), math.degrees(planar_base), math.degrees(link_angle))
