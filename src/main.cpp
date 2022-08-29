#include <Arduino.h>

#define STBY 2
#define PWMA 5

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(STBY, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(PWMA, OUTPUT);


  digitalWrite(STBY, HIGH);
  // digitalWrite(3, HIGH);
  // digitalWrite(4, LOW);
  // analogWrite(PWMA, 100);
  // delay(1000);
  // digitalWrite(4, HIGH);
}

void loop() {
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  for (int i = 0; i < 256; i++){
    Serial.println(analogRead(A0));
    analogWrite(PWMA, i);
    delay(10);
  }
  delay(100);
  for (int i = 255; i >= 0; i--){
    Serial.println(analogRead(A0));
    analogWrite(PWMA, i);
    delay(10);
  }
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  for (int i = 0; i < 256; i++){
    Serial.println(analogRead(A0));
    analogWrite(PWMA, i);
    delay(10);
  }
  delay(100);
  for (int i = 255; i >= 0; i--){
    Serial.println(analogRead(A0));
    analogWrite(PWMA, i);
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