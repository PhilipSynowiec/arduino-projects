#include <Servo.h>

int value = 1200; // set values you need to zero
int input = 0;

Servo firstESC, secondESC; //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time

void setup() {

  firstESC.attach(9);    // attached to pin 9 I just do this with 1 Servo
  Serial.begin(9600);    // start serial at 9600 baud

}

void loop() {

//First connect your ESC WITHOUT Arming. Then Open Serial and follo Instructions

  firstESC.writeMicroseconds(value);

  if(Serial.available()) {
    input = Serial.parseInt();
    if (input != 0){
      value = input;
    }
  }

  Serial.print(value);
  Serial.print("\n");
}
