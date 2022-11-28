#pragma once

#include <Arduino.h>

class Motor
{
    int8_t in1_pin;
    int8_t in2_pin;
    int8_t pwm_pin;

public:
    Motor(int8_t in1_pin, int8_t in2_pin, int8_t pwm_pin)
    {
        pinMode(in1_pin, OUTPUT);
        pinMode(in2_pin, OUTPUT);
        pinMode(pwm_pin, OUTPUT);
    }
    ~Motor() {}
};