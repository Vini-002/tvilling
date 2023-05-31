#pragma once
#include <Arduino.h>
#include "Blink.h"



namespace Communication {

  int setup () {
    Serial.begin(115200);
    Serial.setTimeout(10);

    Blink::setup();

    // wait for other side
    while (!Serial.available()) {
      Blink::interval_ms(500);
    };
    Blink::on(); // Indicate sucessful connection

    // Throw away whatever came
    while (Serial.available()) Serial.read();
    Serial.println("A, B, C");
  }

  // void receive_target() {
  //   // Implementation not finished
  //   if (!Serial.available()) return;
  //   int option = Serial.read() - 'A';
  //   motor[option].set_target(Serial.parseInt());
  // }

}
