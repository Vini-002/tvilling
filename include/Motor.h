#pragma once
#include <Arduino.h>

#define MAX_INTEGRAL 512

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
    const float filter_k = 0.05;
    int error_sum = 0;

    #define N 5
    int ring_buffer[N];
    int ring_index = 0;
    int median;
    
    struct Target {int current, end, increment, i;} target;

    struct Calibration {int low_value, high_value;} calibration = {0, 1023};

    int previous_position = 0;
    
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
      target.end = target.current = analogRead(_analog_pin); // Updates target to initial position
      for (int i = 0; i < N; i++) ring_buffer[i] = 0;
      target.i = 0;
    }

    void control();
    void accelerate(int target);
    void set_target(int target);
    String get_position();
    int filter(int new_value);
};

void Motor::control()
{
    // Makes new measurement and updates estimate
    int measured_position = analogRead(_analog_pin);
    int position = filter(measured_position);
    int dInput = position - previous_position;
    previous_position = position;

    // Ramp target (linear interpolation)
    target.i ++;
    if (target.i == 10) {
        if (target.current < target.end) {
            target.current = min(target.end, target.current + target.increment);
        } else {
            target.current = max(target.end, target.current - target.increment);
        }
        target.i = 0;
    }

    int error = position - target.current;
    error_sum += error;
    error_sum = constrain(error_sum, -MAX_INTEGRAL, MAX_INTEGRAL);

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

// Implementation of median filter
int Motor::filter(int new_value) {
    // Insert new value into ring buffer (overwrite oldest value)
    ring_buffer[ring_index] = new_value;
    ring_index = (ring_index + 1) % N;

    // Copy elements to be sorted
    int sorted[N];
    for (int i = 0; i < N; i++) {
        sorted[i] = ring_buffer[i];
    }

    // Implementation of Insertion sort algorithm
    for (int i = 1; i < N; i++) {
        int j = i;
        while (j > 0 && sorted[j-1] > sorted[j] ) {
            int temp = sorted[j];
            sorted[j] = sorted[j-1];
            sorted[j-1] = temp;
            j--;
        }
    }
    return sorted[N/2];
}

void Motor::set_target(int new_target) {
    target.current = new_target;
    // position.target = map(target, 0, 180, calibration.low_value, calibration.high_value);
}

String Motor::get_position() {
    return String(previous_position);
    // return String(map(position.estimate, calibration.low_value, calibration.high_value, 0, 180));
}
