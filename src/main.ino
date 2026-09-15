#include <MeMCore.h>
#include <Arduino.h>
#include <string.h>

MeUltrasonicSensor ultrasonic(PORT_3);
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeInfraredReceiver infraredReceiver(PORT_6);

int motorSpeed[2] = { 100, 100 };
const int avoidDist = 40; // cm
const int minSpaceDist = 100; // cm
const unsigned long changeDirectionInterval = 15000; // ms
const int maxTurnTime = 3000; // ms
const int minTurnTime = 500; // ms
const int exploreDist = 500; // 5 meters, if the sensor can measure that far

String mode = "driving";
unsigned long startMillis;
unsigned long turnStartMillis;
unsigned long turnDuration;
char turnDirection;
bool robotRunning = true;

void driveForward();
void driveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void decideTurnDirection();
void handleRemote();
void setRanomDrivingSpeed();

void setup() {
  Serial.begin(9600);
  infraredReceiver.begin();
  randomSeed(analogRead(A0));
  startMillis = millis();
}

void loop() {
  handleRemote();
  if (!robotRunning) {
    stopMotors();
    return;
  }

  float distance = ultrasonic.distanceCm();

  if (mode == "driving") {
    if (distance < avoidDist) {
      driveBackward();
      delay(random(500, 1500));
      decideTurnDirection();
      mode = "avoiding";
    } else if (millis() - startMillis >= changeDirectionInterval) {
      startMillis = millis();
      decideTurnDirection();
      turnStartMillis = millis();
      turnDuration = random(minTurnTime, maxTurnTime + 1);
      driveBackward();
      delay(random(1000, 2000));
      mode = "exploring";
    } else {
      driveForward(); 
    }
  } else if (mode == "avoiding") {
    if (turnDirection == 'L') {
      turnLeft();
    } else if (turnDirection == 'R') {
      turnRight();
    }
    delay(200);
    distance = ultrasonic.distanceCm();
    if (distance >= minSpaceDist) {
      startMillis = millis();
      mode = "driving";
    }
  } else if (mode == "exploring") {
    setRanomDrivingSpeed();
    if (distance >= exploreDist || millis() - turnStartMillis >= turnDuration) {
      startMillis = millis();
      mode = "driving";
    } else {
      if (turnDirection == 'L') {
        turnLeft();
      } else if (turnDirection == 'R') {
        turnRight();
      }
    }
  }
}

void handleRemote() {
  infraredReceiver.loop();
  if (!infraredReceiver.available()) {
    return;
  }

  const uint8_t command = infraredReceiver.read();
  if (command == IR_BUTTON_A) {
    robotRunning = true;
    mode = "driving";
    startMillis = millis();
  } else if (command == IR_BUTTON_B) {
    robotRunning = false;
    mode = "driving";
    stopMotors();
  }
}

void decideTurnDirection() {
  const float rand_dir = random(0, 1);
  if (rand_dir < .75) {
    turnDirection = 'L';
  } else {
    turnDirection = 'R';
  }
}

void setRanomDrivingSpeed() {
  motorSpeed[0] = random(98,100);
  motorSpeed[1] = random(98,100);
}

void turnLeft() {
  motor1.run(-motorSpeed[0]);
  motor2.run(-motorSpeed[1]);
}

void turnRight() {
  motor1.run(motorSpeed[0]);
  motor2.run(motorSpeed[1]);
}

void driveForward() {
  motor1.run(motorSpeed[0]);
  motor2.run(-motorSpeed[1]);
}

void driveBackward() {
  motor1.run(-motorSpeed[0]);
  motor2.run(motorSpeed[1]);
}

void stopMotors() {
  motor1.stop();
  motor2.stop();
}