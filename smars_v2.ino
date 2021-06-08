

#define joyX A0
#define joyY A1

#define PWMHIGH 255
#define PWMLOW 0

#define m1Forward 10
#define m1Backward 9

#define m2Forward 11
#define m2Backward 13

enum Direction
{
  LEFT,
  RIGHT,
  FORWARD,
  BACKWARD
};

float mapJoyAxisToOutput(int axisInput, float deadzone, float axisOutput)
{
  float axisResolution = 1024.f;
  float zeroToOneMapping = axisInput / axisResolution;
  float minusOneToOneMapping = (zeroToOneMapping - 0.5f) / 0.5f;

  if (minusOneToOneMapping < deadzone && minusOneToOneMapping > -deadzone){
    return 0;
  }
  
  float returnValue = minusOneToOneMapping * axisOutput;
  return minusOneToOneMapping;
}

float mapper(int input){
 // 0.0 = 255
 // 0.5 = 0
 // 1.0 = 255

  return abs((input - 512.f) / 512.f * 255.f);
}

void updateMotorMovement(Direction dir, int value){

  if (dir == FORWARD){
    analogWrite(m1Forward, value);
    analogWrite(m1Backward, PWMLOW);

    analogWrite(m2Forward, value);
    analogWrite(m2Backward, PWMHIGH);
  }

  if (dir == BACKWARD){
    analogWrite(m1Forward, value);
    analogWrite(m1Backward, PWMHIGH);

    analogWrite(m2Forward, value);
    analogWrite(m2Backward, PWMLOW);
  }

  if (dir == LEFT){
    analogWrite(m1Forward, value);
    analogWrite(m1Backward, PWMLOW);

    analogWrite(m2Forward, value);
    analogWrite(m2Backward, PWMLOW);
  }

  if (dir == RIGHT){
    analogWrite(m1Forward, value);
    analogWrite(m1Backward, PWMHIGH);

    analogWrite(m2Forward, value);
    analogWrite(m2Backward, PWMHIGH);
  }
  
}

void stop(){
  updateMotorMovement(FORWARD, 0);
}

void updateMotors(int value, bool backward){
  analogWrite(m1Forward, value);
  analogWrite(m1Backward, backward * 255);

  analogWrite(m2Forward, value);
  analogWrite(m2Backward, (!backward) * 255);
}

void setup() {
  Serial.begin(9600);
  
  pinMode(m1Forward, OUTPUT);
  pinMode(m1Backward, OUTPUT);

  pinMode(m2Forward, OUTPUT);
  pinMode(m2Backward, OUTPUT);
}

void loop() {
  int inputY = 1024 - analogRead(joyY);
  int inputX = 1024 - analogRead(joyX);
  
  delay(50);
  int yValue = mapper(inputY);
  int xValue = mapper(inputX);

  bool backward = false;
  bool left = false;

  bool xDeadzone = false;
  bool yDeadzone = false;


  // Y - forward/backward
  if (yValue < 550 && yValue > 490){
    yDeadzone = true;
  }

  if (yValue < 480){
    backward = true;
  }
  
  if (xValue < 550 && xValue > 490){
    xDeadzone = true;
  }

  if (xValue < 480){
    left = true;
  }

  //float motorSpeed = abs((jvalue - 512.f) / 512.f * 255.f);

  Direction dir;

  if (xDeadzone){
    if (backward){
      dir = BACKWARD;
    } else {
      dir = FORWARD;
    }
    updateMotorMovement(dir, mapper(yValue));
  }

  if (yDeadzone){
    if (left){
      dir = LEFT;
    } else {
      dir = RIGHT;
    }
    updateMotorMovement(dir, mapper(xValue));
  }
}
