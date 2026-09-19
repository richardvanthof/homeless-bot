#include <MeMCore.h>
#include <Arduino.h>
#include <string.h>

MeUltrasonicSensor ultrasonic(PORT_3);
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeInfraredReceiver infraredReceiver(PORT_6);

int motorSpeed[2] = { 150, 150 };
const int avoidDist = 40; // cm
const int maxDist = 400; // cm
const int minSpaceDist = 100; // cm
const unsigned long changeDirectionInterval = 8000; // ms
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
  Serial.println("Distance: " + String(distance) + " cm");
  Serial.println("Mode: " + mode);

  if (mode == "driving") {

    if (distance < avoidDist) {
      decideTurnDirection();
      driveBackward();
      delay(random(500, 1000));
      
      mode = "avoiding";
    } else if (millis() - startMillis >= changeDirectionInterval) {
      decideTurnDirection();
      
      turnDuration = random(minTurnTime, maxTurnTime + 1);
      driveBackward();
      delay(random(1000, 2000));
      turnStartMillis = millis();
      mode = "exploring";
      startMillis = millis();
    } else {
      driveForward(); 
    }
  } else if (mode == "avoiding") {
    if (turnDirection == 'L') {
      turnLeft();
    } else {
      turnRight();
    }
    
    distance = ultrasonic.distanceCm();
    // Serial.println("Distance: " + String(distance) + " cm");
    if (distance >= minSpaceDist) {
      startMillis = millis();
      setRandomDrivingSpeed();
      mode = "driving";
    } else {
      return;
    }
  } else if (mode == "exploring") {
    const unsigned long elapsedTime = millis() - turnStartMillis;
    if (distance >= exploreDist || elapsedTime >= turnDuration) {
      startMillis = millis();
      //setRandomDrivingSpeed();
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
  const uint8_t code = infraredReceiver.getCode();

  if (code == IR_BUTTON_A) {
    Serial.println("IR:START");
    robotRunning = true;
  } else if (code == IR_BUTTON_C) {
    Serial.println("IR:STOP");
    robotRunning = false;
    stopMotors();
  }
}

void decideTurnDirection() {
  const long randDir = random(0, 4);
  if (randDir < 3) {
    turnDirection = 'L';
  } else {
    turnDirection = 'R';
  }
}

void setRandomDrivingSpeed() {
  motorSpeed[0] = random(140,200);
  motorSpeed[1] = random(140,200);
}

void driveForward() {
  motor1.run(motorSpeed[0]);
  motor2.run(-motorSpeed[1]);
}

void driveBackward() {
  motor1.run(-motorSpeed[0]);
  motor2.run(motorSpeed[1]);
}

void turnLeft() {
  motor1.run(-motorSpeed[0]);
  motor2.run(-motorSpeed[1]);
}

void turnRight() {
  motor1.run(motorSpeed[0]);
  motor2.run(motorSpeed[1]);
}


void stopMotors() {
  motor1.stop();
  motor2.stop();
}