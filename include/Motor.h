#pragma once
#include <Arduino.h>

class Motor
{
private:
    uint8_t _pin_a;
    uint8_t _pin_b;
    uint8_t _pwm_pin;
    uint8_t _analog_pin;

public:
    int speed;
    uint8_t direction = 1;
    const float filter_k = 0.8;
    int error_sum = 0;
    
    struct Position {int estimate, prev_estimate, target;} position = {512, 512, 512};

    struct Calibration {int low_value, high_value;} calibration = {0, 1023};
    
    struct Controller {float p, i, d;} K = {5, 0, 0};
    
    Motor(uint8_t pin_a, uint8_t pin_b, uint8_t pwm_pin, uint8_t analog_pin)
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
      _analog_pin = analog_pin;
      position.target = analogRead(_analog_pin); // Updates target to initial position
    }

    void control();
    void accelerate(int target);
    void set_target(int target);
    String get_position();
};

void Motor::control()
{
    // Makes new measurement and updates estimate
    int measured_position = analogRead(_analog_pin);
    position.estimate = measured_position*filter_k + position.prev_estimate*(1 - filter_k);
    int dInput = position.estimate - position.prev_estimate;
    position.prev_estimate = position.estimate;

    int error = position.estimate - position.target;
    error_sum += error;

    int command = error*K.p + error_sum*K.i - dInput*K.d;

    accelerate(command);
}

void Motor::accelerate(int target)
{
    // Already on target speed
    if (speed == target) return;
    speed = target;

    // Applies PWM
    int pwm_signal = min(abs(speed), 1023);
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

void Motor::set_target(int target) {
    position.target = target;
    // position.target = map(target, 0, 180, calibration.low_value, calibration.high_value);
}

String Motor::get_position() {
    return String(position.estimate);
    // return String(map(position.estimate, calibration.low_value, calibration.high_value, 0, 180));
}
