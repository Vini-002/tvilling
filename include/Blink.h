#pragma once
#include <Arduino.h>
#include "pinout.h"

namespace Blink {

  unsigned long last_blink;

  void setup() {
    pinMode(STATUS_LED, OUTPUT);
    last_blink = millis();
  }

  void interval_ms(int period_ms) {
    unsigned long now = millis();
    if (now - last_blink > period_ms) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      last_blink = now;
    }
  }

  void on() {
    pinMode(STATUS_LED, HIGH);
  }
}
