   #include<AFMotor.h>

AF_DCMotor motor1(1);

AF_DCMotor motor2(2);

AF_DCMotor motor3(3);

AF_DCMotor motor4(4);

char command;

void setup()

{

  Serial.begin(9600);

}

void loop() {

  if (Serial.available()) {
    command = Serial.read();
    Serial.println(command);
    Stop();
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S' :
        Stop();
        break;
    }
  }
}
void right()
{
  motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(255);
  motor3.run(BACKWARD);
  motor4.setSpeed(255);
  motor4.run(BACKWARD);
}

void left()
{
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
  motor3.setSpeed(255);
  motor3.run(FORWARD);
  motor4.setSpeed(255);
  motor4.run(FORWARD);
}
void forward()
{
  motor4.setSpeed(255);
  motor4.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(255);
  motor3.run(FORWARD);
  motor1.setSpeed(255);
  motor1.run(FORWARD);
}

void back()
{
  motor3.setSpeed(255);
  motor3.run(BACKWARD);
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor4.setSpeed(255);
  motor4.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
}
void Stop()
{
  motor1.setSpeed(0);
  motor2.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(0);
  motor4.run(FORWARD);
  motor4.setSpeed(0);
  motor4.run(FORWARD);
}

