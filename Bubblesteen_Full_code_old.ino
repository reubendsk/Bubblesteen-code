#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *DCMotor = AFMS.getMotor(2);

#include <Servo.h>

Servo transportServo;
Servo bubbleServo;

int pos = 0;  

void setup (){
  AFMS.begin();
  DCMotor->setSpeed(255); 
  transportServo.attach(10); //the two servo spots are nine and ten 
  bubbleServo.attach(9); 
}

void loop (){
  DCMotor->run(BACKWARD); // DC motor always turns 
  
  for (pos = 0; pos <= 90; pos += 1) { 
    transportServo.write(pos);   
    bubbleServo.write(180);           
    delay(15);
  }// Move transportServo into dip position
   
  delay(100);
  
  for (pos = 0; pos <= 180; pos += 1) { 
    bubbleServo.write(pos);              
    delay(15);
  }// unDip bubbleServo 
  
  delay(100);
  
  for (pos = 70; pos >= 0; pos -= 1) { 
    transportServo.write(pos);
    bubbleServo.write(180);             
    delay(15);         
  }// Move transportServo back 

  transportServo.write(7);
  delay(1000); // Wait FOR BUBBLE

// repeat
}




