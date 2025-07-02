#include <Arduino.h>
#include "waiter.h"

bool arrived=false;

//Declaring boolean to pause or resume:
bool paused = false;
waiter robot;

void setup() {
  Serial.begin(115200);
  robot.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(arrived)
  {
    // send arrive msg
    // type:status
    // status:arrived
    // location:(ID!=0)?"table":"HB"
    Serial.println("type:arrived");
    // send location; (ID!=0)?"table":"HB"
    Serial.println("location:"+(robot.getID()!=0)?"table":"HB");
    delay(30000);
    arrived = false;
    if(robot.getID()!=0)
    {
      // if arrived?
      // type:status
      // status:returning
      // location:"HB"

      // go to home base
      arrived = robot.RETURN_HOMEBASE();
      //ID=0;
    }
  }
  
}