#include <Arduino.h>

#define STBY 2
#define PWMA 5

int32_t setpoint = 512;
int32_t erro;
int32_t leitura;
uint8_t comando;

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(STBY, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(PWMA, OUTPUT);

  analogWrite(PWMA, 180);
  digitalWrite(STBY, HIGH);
}

void loop()
{
  leitura = analogRead(A0);
  erro = leitura - setpoint;

  if (abs(erro) < 3)
  {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    // break;q
  }
  else if (erro > 0)
  {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
  else
  {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  }
  erro = abs(erro);
  if (erro < (1 << 5)){
    analogWrite(PWMA, erro << 3);
  }
  else
  {
    digitalWrite(PWMA, HIGH);
  }
  delay(4);
  Serial.print(leitura);
  Serial.print(" ");
}