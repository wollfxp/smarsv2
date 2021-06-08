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
  int xValue = map(analogRead(joyAxisX), 0,1024, -100, 100);
  int yValue = map(analogRead(joyAxisY), 0,1024, -100, 100);

  if (abs(xValue) <= deadzone){
    xValue = 0;
  }

  if (abs(yValue) <= deadzone){
    yValue = 0;
  }

  // https://home.kendra.com/mauser/Joystick.html
  xValue *= -1;
  int V = (100-abs(xValue)) * (yValue/100) + yValue;
  int W = (100-abs(yValue)) * (xValue/100) + xValue;
  int R = (V+W)/2;
  int L = (V-W)/2;

  RadioCommand r;
  r.R = R;
  r.L = L;

  bool sent = radio.write(&r, sizeof(RadioCommand));  
}
