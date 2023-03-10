#include <Arduino.h>
#include "Motor.h"

Motor motor[] = {{5, 4, 3, A0}, {8, 7, 9, A1}, {12, 11, 10, A2}};
void show_options();
void run();

char* buffer;
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
    // Receives new target
    if (Serial.available()) {
      // Serial.readBytes((char *) &target, 2);
      Serial.readBytes(buffer, 2);
      if (buffer[0] == 's') break;
      
      int option = buffer[0] - 'a';
      motor[option].set_target(buffer[1]);
      Serial.println("Target " + String(buffer[0]) + ": " + String(motor[option].position.target));
    }
    
    for (size_t i = 0; i < 3; i++)
    {
      motor[i].control();
    }
    t = millis();
    if (t - nt > 50) {
      nt = t;
      Serial.println(motor[0].get_position() + ", " + motor[1].get_position() + ", " + motor[2].get_position());
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
