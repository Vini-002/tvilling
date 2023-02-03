#include <Arduino.h>

#define STBY 2
#define PWMA 5

int32_t setpoint = 512;
float erro;
float leitura;
uint8_t comando;
float leituraa = 512;
int soma_erro = 0, forca;
float Kp = 3, Ki = 0.01;

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(STBY, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(PWMA, OUTPUT);

  analogWrite(PWMA, 180);
  digitalWrite(STBY, HIGH);
//  Serial.setTimeout(10);
}

void loop()
{
  while (Serial.available()>0){
    setpoint = Serial.parseInt();
    setpoint = map(setpoint,0,180,0,1023);

  }
  leitura = analogRead(A0);
  leitura = leitura*0.2 + leituraa*0.8;
  
  leituraa = leitura;
  
  erro = leitura - setpoint;
  soma_erro += erro;

  if (abs(erro) < 5)
  {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
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
  
  forca = abs(erro*Kp + soma_erro*Ki);
  analogWrite(PWMA, forca);
//  if (erro < (1 << 5)){
//    analogWrite(PWMA, erro << 3);
//  }
//  else
//  {
//    digitalWrite(PWMA, HIGH);
//  }
  Serial.print(0);
  Serial.print(", ");
  Serial.print(180);
  Serial.print(", ");
  Serial.println((float) leitura * 180 / 1023);
//  Serial.print(" ");
//  Serial.println(setpoint);
}