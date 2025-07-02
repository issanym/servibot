/** @file waiter.h
 * 
 * @brief waiter robot class/navigation class
 *
 */ 

#ifndef WAITER_H
#define WAITER_H

#include <Arduino.h>

class waiter
{
private:
    const int ENA = 32; //pin capable of Pulse Width Modulation
    const int IN2 = 5;
    const int IN1 = 18;
    const int IN3 = 23;
    const int IN4 = 19;
    const int ENB = 33; //pin capable of Pulse Width Modulation

    // Declaring IR sensor Pins 
    const int LEFT_SENSOR = 13;
    const int MIDDLE_SENSOR = 12;
    const int RIGHT_SENSOR = 14;
    int ID;
public:
    waiter();
    void init();
    int getID(){return ID;}
    void stopMotors();
    void moveForward();
    void turnLeft();
    void turnRight();

    bool GO_TO_TABLE1();
    bool GO_TO_TABLE2();
    bool GO_TO_TABLE3();
    bool RETURN_HOMEBASE();
};

waiter::waiter(/* args */)
{ Serial.println("Waiter created");
}

void waiter::init(){
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
    Serial.println("waiter initialised");
}

void waiter::stopMotors(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0); // Stop motor by setting PWM to 0
    analogWrite(ENB, 0); // Stop motor by setting PWM to 0
}
void waiter::moveForward(){
    digitalWrite(IN1, HIGH); // Left Forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); // Right Forward
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 200);  //analogWrite for speed control
    analogWrite(ENB, 200);  //analogWrite for speed control
}
void waiter::turnLeft(){
    digitalWrite(IN1, LOW);  // Left Stop/Reverse
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, HIGH); // Right Forward
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 200 / 2); // Reduce left speed
    analogWrite(ENB, 200);
}
void waiter::turnRight(){
    digitalWrite(IN1, HIGH); // Left Forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Right Stop/Reverse
    digitalWrite(IN4, HIGH); 
    analogWrite(ENA, 200);     
    analogWrite(ENB, 200 / 2); // Reduce right speed
}

bool waiter::GO_TO_TABLE1(){
    bool left = !digitalRead(LEFT_SENSOR);
    bool middle = !digitalRead(MIDDLE_SENSOR);
    bool right = !digitalRead(RIGHT_SENSOR);

    do{

        //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
        left = !digitalRead(LEFT_SENSOR);
        middle = !digitalRead(MIDDLE_SENSOR);
        right = !digitalRead(RIGHT_SENSOR);
        bool defaultLine = (middle == HIGH && left == LOW && right == LOW);
        bool table2 = (left == LOW && middle == LOW  && right == HIGH);
        bool table3 = (left == HIGH && middle == LOW  && right == HIGH);
        bool homeBase = (left == LOW && middle == LOW  && right == LOW);

        if (defaultLine || table2 || table3 || homeBase) {
            moveForward();
        }
        else if (left==LOW && middle == HIGH && right == HIGH) {
            turnRight();
            delay(500);
        }
        else if (left==HIGH && middle == HIGH && right == LOW) {
            turnLeft();
            delay(500);
        }
    }while(!(left == HIGH && middle == LOW  && right == LOW));

    stopMotors();
    //delay(30000);
    ID = 1;
    //paused = false;

    return true;
}
bool waiter::GO_TO_TABLE2(){
    bool left = !digitalRead(LEFT_SENSOR);
    bool middle = !digitalRead(MIDDLE_SENSOR);
    bool right = !digitalRead(RIGHT_SENSOR);

    do{

        //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
        left = !digitalRead(LEFT_SENSOR);
        middle = !digitalRead(MIDDLE_SENSOR);
        right = !digitalRead(RIGHT_SENSOR);
        bool defaultLine = (middle == HIGH && left == LOW && right == LOW);
        bool table1  = (left == HIGH && middle == LOW  && right == LOW);
        bool table3 = (left == HIGH && middle == LOW  && right == HIGH);
        bool homeBase = (left == LOW && middle == LOW  && right == LOW);

        if (defaultLine || table1 || table3 || homeBase) {
            moveForward();
        }
        else if (left==LOW && middle == HIGH && right == HIGH) {
            turnRight();
            delay(500);
        }
        else if (left==HIGH && middle == HIGH && right == LOW) {
            turnLeft();
            delay(500);
        }
    }while(!(left == LOW && middle == LOW  && right == HIGH));

    stopMotors();
    //delay(30000);
    ID = 2;
    //paused = false;

    return true;
}
bool waiter::GO_TO_TABLE3()
{
    bool left = !digitalRead(LEFT_SENSOR);
    bool middle = !digitalRead(MIDDLE_SENSOR);
    bool right = !digitalRead(RIGHT_SENSOR);

    do{

        //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
        left = !digitalRead(LEFT_SENSOR);
        middle = !digitalRead(MIDDLE_SENSOR);
        right = !digitalRead(RIGHT_SENSOR);
        bool defaultLine = (middle == HIGH && left == LOW && right == LOW);
        bool table1  = (left == HIGH && middle == LOW  && right == LOW);
        bool table2 = (left == LOW && middle == LOW  && right == HIGH);
        bool homeBase = (left == LOW && middle == LOW  && right == LOW);

        if (defaultLine || table1 || table2 || homeBase) {
            moveForward();
        }
        else if (left==LOW && middle == HIGH && right == HIGH) {
            turnRight();
            delay(500);
        }
        else if (left==HIGH && middle == HIGH && right == LOW) {
            turnLeft();
            delay(500);
        }
    }while(!(left == HIGH && middle == LOW  && right == HIGH));

    stopMotors();
    //delay(30000);
    ID = 3;
    //paused = false;

    return true;
}
bool waiter::RETURN_HOMEBASE(){
    bool left = !digitalRead(LEFT_SENSOR);
    bool middle = !digitalRead(MIDDLE_SENSOR);
    bool right = !digitalRead(RIGHT_SENSOR);

    do{

        //IR sensor reading (since we are using active-low sensor:line detected = LOW, no line = HIGH)
        left = !digitalRead(LEFT_SENSOR);
        middle = !digitalRead(MIDDLE_SENSOR);
        right = !digitalRead(RIGHT_SENSOR);
        bool defaultLine = (middle == HIGH && left == LOW && right == LOW);
        bool table1  = (left == HIGH && middle == LOW  && right == LOW);
        bool table2 = (left == LOW && middle == LOW  && right == HIGH);
        bool table3 = (left == HIGH && middle == LOW  && right == HIGH);

        if (defaultLine || table1 || table2 || table3) {
            moveForward();
        }
        else if (left==LOW && middle == HIGH && right == HIGH) {
            turnRight();
            delay(500);
        }
        else if (left==HIGH && middle == HIGH && right == LOW) {
            turnLeft();
            delay(500);
        }
    }while(!(left == LOW && middle == LOW  && right == LOW));

    stopMotors();
    //delay(30000);
    ID = 0;
    //paused = false;

    return true;
}



#endif /* WAITER_H */

/*** end of file ***/