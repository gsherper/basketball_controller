/*
    basketball_controller.cpp
   
    https://github.com/gsherper

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <Arduino.h>
#include "pitches.h"  // must include open source pitches.h found online in libraries folder
#include "TM1637.h"

//game control
const int startButton = 8;
boolean gameInProgress = false;

//timer
unsigned long startMillis;
unsigned long currentMillis;
unsigned long elapsedMillis;
unsigned long countDownMillis;
unsigned long allowedGameTime = 60000; //60000ms = 1 min
const int clkGameTimerPin = 6;
const int dataGameTimerPin = 7;
TM1637 gameTimer(clkGameTimerPin, dataGameTimerPin);


//scoreboard 
const int clkScoreboardPin = 4;
const int dataScoreboardPin = 5;
int score = 0;

TM1637 scoreboard(clkScoreboardPin, dataScoreboardPin);

//speaker
const int speakerPin = 9;

//basket photo detector 
int RedLedPin = 13;                     
int RedLedPin2 = 12;                     
int RedLedPin3 = 11;                     

int senRead = 0;                  
int senRead2 = 1;                  
int senRead3 = 2;                  

int SenseRate = 1010;

void setup() {  

  pinMode(startButton, INPUT);
  digitalWrite(startButton, HIGH);       // turn on pullup resistors

  pinMode(RedLedPin, OUTPUT);

  digitalWrite(RedLedPin, LOW);

  scoreboard.init();
  scoreboard.set(BRIGHT_TYPICAL);  

  gameTimer.init();
  gameTimer.set(BRIGHT_TYPICAL);
  
}  

void playCoinSound() {
  // Play coin sound   
  tone(speakerPin,NOTE_B5,100);
  delay(100);
  tone(speakerPin,NOTE_E6,450);
  delay(800);
  noTone(speakerPin);
}

void playGameOverSound() {
  tone(speakerPin,NOTE_G4,250);
  delay(100);
  tone(speakerPin,NOTE_C4,500);
  delay(800);
  noTone(speakerPin);
}

void updateScore(boolean reset=false) {
  if (reset) {
    score = 0;
  } else {
    score = score + 1;
  }
  scoreboard.displayNum(score);
}

boolean detectBasketball(int senReadNum, int redPin, boolean force=false) {

  int val=analogRead(senReadNum);    

  if(val <= SenseRate || force) {
    digitalWrite(redPin, HIGH);
    return true;
  } 
  digitalWrite(redPin, LOW);
  return false;
  
}

void showZeroGameTime() {
  gameTimer.point(true);
  int8_t displayText[4] = {0x0, 0x0, 0x0, 0x0};
  gameTimer.display(displayText);
  delay(1000);
}

void showBlankGameTime() {
  gameTimer.point(false);
  gameTimer.clearDisplay();
  delay(1000);  
}

void blinkGameOverTime(int interval) {
  for (int i=0; i < interval; i++) {
    showZeroGameTime();
    if (i==0) {
      playGameOverSound();
    }
    showBlankGameTime();
  }
}

boolean isGameInProgress() {
  if (gameInProgress) {
    currentMillis = millis();
    elapsedMillis = (currentMillis - startMillis);
    countDownMillis = allowedGameTime - elapsedMillis;
    
    if (countDownMillis <= 1000) {
      blinkGameOverTime(3);
      gameInProgress = false;
    } else {
      unsigned long durSS = (countDownMillis/1000)%60;    //Seconds
      unsigned long durMM = (countDownMillis/(60000))%60; //Minutes
      gameTimer.point(true);
      int8_t displayText[4] = {durMM / 10, durMM % 10, durSS / 10, durSS % 10};
      gameTimer.display(displayText);
    }
  } 
  return gameInProgress;
}

void loop() {  

  if(isGameInProgress()) {
    //detect basket 1
    if (detectBasketball(senRead, RedLedPin)) {
      playCoinSound();
      updateScore();
    }
    //detect basket 2
    if (detectBasketball(senRead2, RedLedPin2)) {
      playCoinSound();
      updateScore();
    }
    //detect basket 3
    if (detectBasketball(senRead3, RedLedPin3)) {
      playCoinSound();
      updateScore();
    }
   } else {
     int startGame = !digitalRead(startButton);
     if (startGame) {
       updateScore(true);
       gameInProgress = true;
       startMillis = millis();
     }
   }
}