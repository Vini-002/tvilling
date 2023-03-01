#pragma once

#include <Arduino.h>

class Motor
{
private:
    uint8_t _pin_a;
    uint8_t _pin_b;
    uint8_t _pwm_pin;

public:
    int speed;
    uint8_t direction = 1;

    Motor(uint8_t pin_a, uint8_t pin_b, uint8_t pwm_pin)
    {
      pinMode(pin_a, OUTPUT);
      pinMode(pin_b, OUTPUT);
      pinMode(pwm_pin, OUTPUT);
      digitalWrite(pin_a, HIGH);
      digitalWrite(pin_b, LOW);
      digitalWrite(pwm_pin, LOW);
      _pin_a = pin_a;
      _pin_b = pin_b;
      _pwm_pin = pwm_pin;
    }
    void accelerate(int target);
};


void Motor::accelerate(int target)
{
    int erro = target - speed;

    // Already on target speed
    if (!erro) return;

    // Applies PWM
    uint8_t pwm_signal = (uint8_t) min(abs(target), 255);
    analogWrite(_pwm_pin, pwm_signal);

    // Already on target direction
    if ((speed > 0) == direction) return;

    if (speed > 0) {
        digitalWrite(_pin_a, HIGH);
        digitalWrite(_pin_b, LOW);
        direction = 1;
    } else {
        digitalWrite(_pin_a, LOW);
        digitalWrite(_pin_b, HIGH);
        direction = 0;
    }
}
