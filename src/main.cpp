#include <Arduino.h>

#define STBY 2
#define PWMA 5

int setpoint = 512;
int error;

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(STBY, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(PWMA, OUTPUT);

  digitalWrite(STBY, HIGH);


}

void loop() {
  int leitura = analogRead(A0);
  Serial.println(leitura);
  error = leitura - setpoint;

  if (error > 0){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }else
  {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  }
  analogWrite(PWMA, error/10);
  
}