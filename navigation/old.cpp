#include <Arduino.h>

 //Declaring Motor Control Pins
const int ENA = 10; //pin capable of Pulse Width Modulation
const int IN1 = 36;
const int IN2 = 37;
const int IN3 = 33;
const int IN4 = 32;
const int ENB = 11; //pin capable of Pulse Width Modulation

// Declaring IR sensor Pins 
const int LEFT_SENSOR = 40;
const int MIDDLE_SENSOR = 41;
const int RIGHT_SENSOR = 42;

// Speed Settings
const int MAX_SPEED = 200; 

// Declaring junctions that might be encountered:
int rightJunction = 0;
int leftJunction = 0;

//Declaring boolean to pause or resume:
bool paused = false;

//Declaring a string for us to know the current task:
String currentTask = " ";

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); // Stop motor by setting PWM to 0
  analogWrite(ENB, 0); // Stop motor by setting PWM to 0
}
void moveForward() {
  digitalWrite(IN1, HIGH); // Left Forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // Right Forward
  digitalWrite(IN4, LOW);
  analogWrite(ENA, MAX_SPEED);  //analogWrite for speed control
  analogWrite(ENB, MAX_SPEED);  //analogWrite for speed control
}
void turnLeft() {
  digitalWrite(IN1, LOW);  // Left Stop/Reverse
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); // Right Forward
  digitalWrite(IN4, LOW);
  analogWrite(ENA, MAX_SPEED / 2); // Reduce left speed
  analogWrite(ENB, MAX_SPEED);    
}

void turnRight() {
  digitalWrite(IN1, HIGH); // Left Forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  // Right Stop/Reverse
  digitalWrite(IN4, HIGH); 
  analogWrite(ENA, MAX_SPEED);     
  analogWrite(ENB, MAX_SPEED / 2); // Reduce right speed
}

//The 6 vital functions:
void GO_TO_TABLE1() {
  //task performded:
  currentTask = "TABLE1";

  //boolean value for the do-while condition:
   bool arrived = false;

  do{
    if (paused){
      return;
    }
    //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
    bool left = !digitalRead(LEFT_SENSOR);
    bool middle = !digitalRead(MIDDLE_SENSOR);
    bool right = !digitalRead(RIGHT_SENSOR);

    if (middle == HIGH && left == LOW && right == LOW) {
      moveForward();
    }
    else if (middle == HIGH && right == HIGH) {
      turnRight();
      delay(500);
    } 
    else if (middle == HIGH && left == HIGH) {
      stopMotors();
      delay(30000);
      arrived = true;
    }
    else {
      stopMotors();
    }
  }while(!arrived);
  RETURN_HOMEBASE();
}
void GO_TO_TABLE2() {
  //task performded:
  currentTask = "TABLE2";

  //boolean value for the do-while condition:
   bool arrived = false;
  do{
    if (paused){
      return;
    }
   //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
   bool left = !digitalRead(LEFT_SENSOR);
   bool middle = !digitalRead(MIDDLE_SENSOR);
   bool right = !digitalRead(RIGHT_SENSOR);

    if (middle == HIGH && left == LOW && right == LOW) {
      moveForward();
    }
    else if (middle == HIGH && right == HIGH) {
      rightJunction += 1;
      if(rightJunction < 2) {
        turnRight();
        delay(500);
      }
      else if (rightJunction == 2){
        stopMotors();
        delay(30000);
        arrived = true;
      }
      else {
        stopMotors();
      }
    }
     
    else if (middle == HIGH && left == HIGH) {
      leftJunction += 1;
      if (leftJunction >= 2) {
        turnLeft();
        delay(500);
      }
    }
  }while(!arrived);
  rightJunction = 0;
  leftJunction = 0;
  RETURN_HOMEBASE();
}
void GO_TO_TABLE3() {
  //task performded:
  currentTask = "TABLE3";

  //boolean value for the do-while condition:
   bool arrived = false;

  do{
    if (paused){
      return;
    }
   //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
   bool left = !digitalRead(LEFT_SENSOR);
   bool middle = !digitalRead(MIDDLE_SENSOR);
   bool right = !digitalRead(RIGHT_SENSOR);

    if (middle == HIGH && left == LOW && right == LOW) {
      moveForward();
    }
    else if (middle == HIGH && right == HIGH) {
       rightJunction += 1;
      if (rightJunction <= 3) {
        turnRight();
        delay(500);
      }
    }

    else if (middle == HIGH && left == HIGH) {
      leftJunction += 1;
      if (leftJunction >= 2) {
        turnLeft();
        delay(500);
      }
    }
    else if (middle == HIGH && right == HIGH && left == HIGH) {
      stopMotors();
      delay(30000);
      arrived = true;
    }
    else{
      stopMotors();
    }
  }while(!arrived);
    rightJunction = 0;
    leftJunction = 0;
  RETURN_HOMEBASE();
}


void RETURN_HOMEBASE() {

}
void PAUSE() {
  stopMotors();
  paused = true;
}
void RESUME() {
  paused = false;

  if (currentTask == "TABLE1") {
    GO_TO_TABLE1();
  } else if (currentTask == "TABLE2") {
    GO_TO_TABLE2();
  } else if (currentTask == "TABLE3") {
    GO_TO_TABLE3();
  } else if (currentTask == "RETURN") {
    RETURN_HOMEBASE();
  }
}
void setup() {
  //Declaring Motor Pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Declaring Sensor Pins
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(MIDDLE_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT); 
}
void loop() {

  
}