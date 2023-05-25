#pragma once
#include <Arduino.h>

namespace Blink {

  unsigned long last_blink;

  void setup() {
    pinMode(2, OUTPUT);
    last_blink = millis();
  }

  void update(int period_ms) {
    unsigned long now = millis();
    if (now - last_blink > period_ms) {
      digitalWrite(2, !digitalRead(2));
      last_blink = now;
    }
  }
}
