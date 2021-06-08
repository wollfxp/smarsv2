#include <DigitalIO.h>
#include <RF24.h>

RF24 radio(2, 3); // CE, CSN
const byte address[6] = "00001";

#define m1Forward 10
#define m1Backward 9
#define m2Forward 11
#define m2Backward 13

struct RadioCommand
{
  int R;
  int L;
};


void updateMotors(int value, bool backward) {
  analogWrite(m1Forward, value);
  analogWrite(m1Backward, backward * 255);

  analogWrite(m2Forward, value);
  analogWrite(m2Backward, (!backward) * 255);
}

void setup() {
  
  Serial.begin(115200);
  Serial.println("Hello from smars v2 tank");
  Serial.println("3...");
  delay(333);
  Serial.println("2...");
  delay(333);
  Serial.println("1...");
  delay(333);
  Serial.println("GO!");
  delay(100);
  // RADIO START

  if (!radio.begin())
  {
    Serial.println("Radio is not responding!");
    while (1){}
  } else {
    Serial.println("Radio OK!");
  }
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver

  // RADIO END

  pinMode(m1Forward, OUTPUT);
  pinMode(m1Backward, OUTPUT);

  pinMode(m2Forward, OUTPUT);
  pinMode(m2Backward, OUTPUT);

  updateMotors(0, false);
}

void loop() {

  if (radio.available())
  {

  if(radio.getDynamicPayloadSize() < sizeof(RadioCommand)){
      Serial.println("garbage");
      // Corrupt payload has been flushed
      return;
    }
    
    RadioCommand cmd;
    radio.read(&cmd, sizeof(RadioCommand));
    Serial.println("Got R" + String(cmd.R));
    Serial.println("Got L" + String(cmd.L));

    int r = cmd.R;
    int l = cmd.L;

    if (r == 0 && l == 0)
    {
      updateMotors(0,false);
      return; // NO INPUT, IDLE
    }

    analogWrite(m1Forward, map(abs(r), 0, 100, 0, 255));
    analogWrite(m1Backward, r < 0 ? true : false);
  
    analogWrite(m2Forward, map(abs(l), 0, 100, 0, 255));
    analogWrite(m2Backward, l < 0 ? false : true);
  }
  else {
    //delay(200);
    //Serial.println("Waiting for radio...");
    //updateMotors(0, false); // STOP
  }
}
