#include <Arduino.h>
#include "Motor.h"


Motor motor[] = {{5, 4, 3},{8, 7, 9},{12,11,10}};
int AnaloRead[]= {A0,A1,A2};
void show_options();
void run();

char* buffer;
int measure, estimate;
const int low_value = 67, high_value = 893;
int error, error_sum = 0, command;

struct controller {float p, i, d;} K = {1, 0, 0};

// This coefficient determines how much wheight the
// filter gives to the new measure vs the previous estimate
const float filter_k = 0.8;

unsigned long t, nt = 0;

void setup() {
  Serial.begin(9600);
  show_options();
}

void loop() {
  // Waits for input
  while (!Serial.available()) continue;

  switch (Serial.read())
  {
  case 'P':
    K.p = Serial.parseFloat();
    break;
  
  case 'I':
    K.i = Serial.parseFloat();
    break;

  case 'D':
    K.d = Serial.parseFloat();
    break;
  
  case 'r':
    delay(2);
      while (Serial.available()) Serial.read();
      run();
    break;

  default:
    break;
  }
  show_options();
}

void run()
{
  while (true)
   {
    for (size_t i = 0; i < 3; i++)
    {
      // Receives new target
      if (Serial.available()) {
        // Serial.readBytes((char *) &target, 2);
        Serial.readBytes(buffer, 2);
        if (buffer[0] == 's') break;
        
        int option = buffer[0] - 'a';
        motor[option].set_target(buffer[1]);
        Serial.println("Target " + String(buffer[0]) + ": " + String(motor[option].target_position));
      }

      // Makes new measurement and updates estimate
      measure = analogRead(AnaloRead[i]);
      estimate = measure*filter_k + motor[i].prev_estimate*(1 - filter_k);
      int dInput = estimate - motor[i].prev_estimate;
      motor[i].prev_estimate = estimate;
      
      error = estimate - motor[i].target_position;
      error_sum += error;

      command = error*K.p + error_sum*K.i - dInput*K.d;
      
      motor[i].accelerate(command);
   }
    t = millis();
    if (t - nt > 50) {
      nt = t;
      Serial.println("A " + String(map(motor[0].prev_estimate, low_value, high_value, 0, 180)) + ", B " + String(map(motor[1].prev_estimate, low_value, high_value, 0, 180)) + ", C " + String(map(motor[2].prev_estimate, low_value, high_value, 0, 180)));
    }
    
  }
}

void show_options() {
  Serial.println("---------------------------------------------");
  Serial.println("P: " + String(K.p, 3));
  Serial.println("I: " + String(K.i, 3));
  Serial.println("D: " + String(K.d, 3));
  Serial.println("r -> RUN");
  Serial.println("---------------------------------------------"); 
}
