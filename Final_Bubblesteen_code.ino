 #include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

#include <Servo.h>
Servo transportServo;
Servo bubbleServo;       

int pos = 0;  

int i;
int latchPin = 7; 
int dataPin = 6;
int clockPin = 8;

int seconds = 0;
int minutes = 0;
int hours = 0;
int ones = 0;
int tens = 0;
int hundreds = 0;
int thousands = 0;
int numArray[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 103};
int numArrayThousands[] = {0, 6};


void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT); //serial 
  pinMode(7, OUTPUT); //latch 
  pinMode(8, OUTPUT); // push bit

  AFMS.begin();
  transportServo.attach(10); //the two servo spots are nine and ten 
  bubbleServo.attach(9); 
  pinMode(13, OUTPUT);

  pinMode(12, OUTPUT);
}

void loop() {
  DateTime now = RTC.now(); 
  seconds = now.second();
  minutes = now.minute();
  hours = now.hour();
  ones = minutes%10;
  tens = (minutes-ones)/10;
  hundreds = hours%10;
  thousands = (hours-hundreds)/10;
  delay(3);
  outputNumber(5, ones);
  delay(3);
  outputNumber(4, tens);
  delay(3);
  outputNumber(3, hundreds);
  delay(3);
  outputNumberThousands(thousands);
  //Serial.print(thousands);
  //Serial.print(hundreds);
  //Serial.print(tens);
  //Serial.println(ones);
  Serial.println(seconds);
  delay(10);

  //if (hours == 15 && minutes == 54 && seconds <= 20){
  if (seconds <= 10){
  runBubbleBlow();
  }
}

int runBubbleBlow (){
  tone(12, 400, 1000);
  digitalWrite(13, HIGH);
  for (pos = 0; pos <= 90; pos += 1) { 
    transportServo.write(pos);   
    bubbleServo.write(180);           
    delay(15);
  }// Move transportServo into dip position
  
  tone(12, 400, 1000); 
  
  delay(100);
  
  for (pos = 0; pos <= 180; pos += 1) { 
    bubbleServo.write(pos);              
    delay(15);
  }// unDip bubbleServo 

  tone(12, 400, 1000);
  
  delay(100);
  
  for (pos = 70; pos >= 0; pos -= 1) { 
    transportServo.write(pos);
    bubbleServo.write(180);             
    delay(15);         
  }// Move transportServo back 

  transportServo.write(7);
  
  tone(12, 400, 1000);
  
  delay(1000); // Wait FOR BUBBLE

}

void outputNumber(int place, int value){
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(place, LOW);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numArray[value]);
  digitalWrite(latchPin, HIGH);
}

void outputNumberThousands(int value){
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, LOW);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numArrayThousands[value]);
  digitalWrite(latchPin, HIGH);
}
