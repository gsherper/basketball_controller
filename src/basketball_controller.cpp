
#include <Arduino.h>
#include "pitches.h"  // must include open source pitches.h found online in libraries folder
#include "TM1637.h"

const int CLK = 6;
const int DIO = 7;
TM1637 tm1637(CLK, DIO);
int score = 0;

int photoDiode=2;                      
int GreenLed=13;                     
int senRead=0;                  
int SenseRate=1000;                   
 void setup()    
 {  
  pinMode(photoDiode,OUTPUT);  
  pinMode(GreenLed,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(photoDiode,HIGH);       
  digitalWrite(GreenLed,LOW);

      tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;      
  Serial.begin(9600);           
 }  
 void loop()  
 {  
  int val=analogRead(senRead);    
//  Serial.println(val);            
  if(val <= SenseRate)               
  {
   Serial.println("Detected");
   digitalWrite(12,HIGH);  
   digitalWrite(GreenLed,LOW);    
      delay(20);  

    // Play coin sound   
  tone(9,NOTE_B5,100);
  delay(100);
       digitalWrite(12,LOW);

  tone(9,NOTE_E6,450);
  score = score + 1;
  tm1637.displayNum(score);

  delay(800);
  noTone(8);
 
  }  
  else if(val > SenseRate)            
  {  
   digitalWrite(12,LOW);
   digitalWrite(GreenLed,HIGH);       
   delay(20);  
  }  
 }  