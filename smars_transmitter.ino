#include <DigitalIO.h>
#include <RF24.h>

RF24 radio(2, 3); // CE, CSN
const byte address[6] = "00001";

const int joyAxisX = A0;
const int joyAxisY = A1;
const int deadzone = 16;

struct RadioCommand
{
  int R;
  int L;
};

void setup() {
  Serial.begin(115200);
  Serial.println("smars v2 transmitter start!");
  
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
  int rawX = analogRead(joyAxisX);
  int rawY = 1024 - analogRead(joyAxisY);
  int xValue = map(rawX, 0,1024, -100, 100);
  int yValue = map(rawY, 0,1024, -100, 100);

  if (abs(xValue) <= deadzone){
    xValue = 0;
  }

  if (abs(yValue) <= deadzone){
    yValue = 0;
  }

  // https://home.kendra.com/mauser/Joystick.html
  xValue *= -1;
  float V = (100.f-abs(xValue)) * (yValue/100.f) + yValue;
  float W = (100.f-abs(yValue)) * (xValue/100.f) + xValue;
  float R = (V+W)/2.f;
  float L = (V-W)/2.f;

  RadioCommand r;
  r.R = R;
  r.L = L;
  bool sent = radio.write(&r, sizeof(RadioCommand));  
//  Serial.println("X:"+String(xValue * -1) + " Y:" + String(yValue));
//  Serial.println("V:"+String(V) + " W:" + String(W));
//  Serial.println("R:"+String(R) + " L:" + String(L));
}
