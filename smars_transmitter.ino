#include <DigitalIO.h>
#include <RF24.h>

RF24 radio(2, 3); // CE, CSN
const byte address[6] = "00001";

const int joyAxisX = A0;
const int joyAxisY = A1;
const int deadzone = 16;

float mapJoyAxis(int rawInput) {
  return abs((rawInput - 512.f) / 512.f * 255.f);
}

void setup() {
  Serial.begin(115200);
  Serial.println("smars v2 trasnmitter start!");

  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!radio.begin())
  {
    Serial.println("NO radio!");
    while (1){};
  }
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
  Serial.println("Radio OK!");
}
void loop()
{
//  int xValue = getValueFromAxisPin(joyAxisX);
//  int yValue = getValueFromAxisPin(joyAxisY);
  //int xValue = analogRead(joyAxisX);
  int yValue = analogRead(joyAxisY);

  //radio.write(&xValue, sizeof(xValue));  //Sending the message to receiver
  radio.write(&yValue, sizeof(yValue));  //Sending the message to receiver
  Serial.println("Sending " + String(yValue));
  delay(50);
}

int getValueFromAxisPin(int pin)
{
  int value = analogRead(pin);
  if (value > (512 - deadzone) && value < (512 + deadzone))
  {
    // deadzone, return -1;
    return -1;
  }
  return mapJoyAxis(value);
}
