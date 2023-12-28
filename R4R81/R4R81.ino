#include "CrsfSerial.h"

CrsfSerial crsf(Serial, 115200);

int inputs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // 4 sticks + AUX1 AKA CH5

/*

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control

int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M2 Direction Control

*/


#define CRSF_MIDDLE = 1000;

#define M1_SPEED 5
#define M2_SPEED 6

#define M1_DIRECTION 4  // HIGH - forward
#define M2_DIRECTION 7

#define DEADZONE 3

void updateMotors() {

  if (inputs[4] > 1600) {  // ARM/DISARM
    //digitalWrite(M1_DIRECTION, HIGH);
    //digitalWrite(M2_DIRECTION, HIGH);

    int rawValueX = map(inputs[0], 1000, 2000, -100, 100);
    int rawValueY = map(inputs[1], 1000, 2000, -100, 100);

    // int speedX = constrain(abs(rawValue), 0, 100);  // abs(rawValue) * 255
    // int directionX = rawValue < 0 ? LOW : HIGH;

    // if (speedX < DEADZONE) {
    //   speedX = 0;
    // }

    // Serial.println(String(rawValueX) + ": " + String(speedX) + " -> " + String(directionX));

    rawValueX *= -1;
    float V = (100.f-abs(rawValueX)) * (rawValueY/100.f) + rawValueY;
    float W = (100.f-abs(rawValueY)) * (rawValueX/100.f) + rawValueX;
    float R = (V+W)/2.f;
    float L = (V-W)/2.f;

    L = constrain(L, -100, 100); 
    R = constrain(R, -100, 100);

    //Serial.println(String(L) + " x " + String(R));

    analogWrite(M1_SPEED, abs(L) * 2.55);
    analogWrite(M2_SPEED, abs(R) * 2.55);

    digitalWrite(M1_DIRECTION, L > 0 ? HIGH : LOW);
    digitalWrite(M2_DIRECTION, R > 0 ? HIGH : LOW);
  } else {
    analogWrite(M1_SPEED, 0);
    analogWrite(M2_SPEED, 0);
  }
}

void packetChannels() {

  for (int i = 0; i < 8; i++) {
    int value = crsf.getChannel(i + 1);  // 1 based
    inputs[i] = value;
  }

  for (int i = 1; i <= 8; i++) {
    //Serial.print(String(crsf.getChannel(i)) + " ");
    //Serial.print(String(inputs[i-1]) + " ");
  }
  //Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial1.begin(115200);
  crsf.onPacketChannels = &packetChannels;
  //Serial.println("Hello");
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(M1_SPEED, OUTPUT);
  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_SPEED, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  crsf.loop();

  digitalWrite(LED_BUILTIN, inputs[4] > 1600 ? HIGH : LOW);
  updateMotors();

  // if (Serial1.available()) {
  //   int d = Serial1.read();
  //   Serial.write((char)(d));
  // }
}
