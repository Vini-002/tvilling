#include <Arduino.h>

#define STBY 2
#define PWMA 5

const uint8_t pin_a = 3, pin_b = 4;

int target = 512;
int measure, estimate, prev_estimate = 512;

int error, error_sum = 0, command;

struct controller {float p, i, d;} K = {1, 0, 0};

const int low_value = 67, high_value = 893;

// This coefficient determines how much wheight the
// filter gives to the new measure vs the previous estimate
const float filter_k = 0.8;

unsigned long now, last_update = 0;

void setup() {

  Serial.begin(9600);

  // Set up the H-bridge pins 
  pinMode(STBY, OUTPUT);
  pinMode(pin_a, OUTPUT);
  pinMode(pin_b, OUTPUT);
  pinMode(PWMA, OUTPUT);

  digitalWrite(STBY, HIGH);
}

void loop()
{
  // Receives new target
  while (Serial.available()) {
    Serial.readBytes((char *) &target, 2);
    target = map(target, 0, 180, low_value, high_value);
  }

  // Makes new measurement and updates estimate
  measure = analogRead(A0);
  estimate = measure*filter_k + prev_estimate*(1 - filter_k);
  int dInput = estimate - prev_estimate;
  prev_estimate = estimate;
  
  error = estimate - target;
  error_sum += error;

  command = abs(error*K.p + error_sum*K.i - dInput*K.d);
  
  if (abs(command) < 3) {  // Hysterisis
    digitalWrite(pin_a, HIGH);
    digitalWrite(pin_b, HIGH);
  } else if (command > 0) {
    digitalWrite(pin_a, HIGH);
    digitalWrite(pin_b, LOW);
  } else {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, HIGH);
  }
  
  analogWrite(PWMA, command);

  now = millis();
  if (now - last_update > 50) {  // F_update =< 20 Hz
    last_update = now;
    Serial.write(map(estimate, low_value, high_value, 0, 180));
  }
  // F_inicial 64 Hz
}