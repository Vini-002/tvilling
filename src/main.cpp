#include <Arduino.h>

#define STBY 2
#define PWMA 5

#define N 256

uint16_t setpoint = 512;
int16_t erro[N];
uint16_t leitura[N];
uint8_t comando[N];
uint8_t direction = 0;

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(STBY, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(PWMA, OUTPUT);

  digitalWrite(STBY, HIGH);

  for (uint8_t i = 0; i < 256; i++)
  {
    leitura[i] = analogRead(A0);
    erro[i] = leitura[i] - setpoint;

    if (erro[i] > 0){
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
    }else
    {
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
    }
    analogWrite(PWMA, erro[i] >> 3);
    delay(10);
  }
  Serial.println(leitura[0]);
}

void loop() {
}