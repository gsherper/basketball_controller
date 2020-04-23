
#include <Arduino.h>
#include "pitches.h"  // must include open source pitches.h found online in libraries folder
#include "TM1637.h"

//scoreboard 
const int clkScoreboardPin = 6;
const int dataScoreboardPin = 7;
int score = 0;

TM1637 tm1637(clkScoreboardPin, dataScoreboardPin);

//speaker
const int speakerPin = 9;

//basket photo detector 
int photoDiodePin = 2;                      
int RedLedPin = 13;                     
int senRead = 0;                  
// int SenseRate = 1000;
int SenseRate = 1005;

void setup() {  

  pinMode(photoDiodePin, OUTPUT);  
  pinMode(RedLedPin, OUTPUT);

  digitalWrite(photoDiodePin, HIGH);       
  digitalWrite(RedLedPin, LOW);

  tm1637.init();
  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.set(BRIGHT_TYPICAL);  
  
  Serial.begin(9600);           

}  

void playCoinSound() {

  // Play coin sound   
  tone(speakerPin,NOTE_B5,100);
  delay(100);
  tone(speakerPin,NOTE_E6,450);
  delay(800);
  noTone(8);

}

void updateScore(boolean reset=false) {
  if (reset) {
    score = 0;
  } else {
    score = score + 1;
  }
  tm1637.displayNum(score);
}

boolean detectBasketball(boolean force=false) {

  int val=analogRead(senRead);    
  
  Serial.println(val);   
  if(val <= SenseRate || force) {
    Serial.println("Detected");
    digitalWrite(RedLedPin, HIGH);
    return true;
  } 
  return false;
  
}

void loop() {  

  if (detectBasketball(true)) {
    playCoinSound();
    updateScore();
  }

  digitalWrite(RedLedPin, LOW);
  delay(20);  
}  