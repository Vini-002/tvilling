#include <Arduino.h>

void setup() {

  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, HIGH);
  // digitalWrite(3, HIGH);
  // digitalWrite(4, LOW);
  // analogWrite(5, 100);
  // delay(1000);
  // digitalWrite(4, HIGH);
}

void loop() {
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  for (int i = 0; i < 256; i++){
    analogWrite(5, i);
    delay(10);
  }
  delay(100);
  for (int i = 255; i >= 0; i--){
    analogWrite(5, i);
    delay(10);
  }
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  for (int i = 0; i < 256; i++){
    analogWrite(5, i);
    delay(10);
  }
  delay(100);
  for (int i = 255; i >= 0; i--){
    analogWrite(5, i);
    delay(10);
  }
  // delay(2000);
  // digitalWrite(3, LOW);
  // digitalWrite(4, HIGH);
  // digitalWrite(13, LOW);
  // delay(2000);
  // digitalWrite(3, HIGH);
  // digitalWrite(4, LOW);
  // digitalWrite(13, HIGH);
}