#include <Arduino.h>
#include <Keyboard.h>

int iGreenPin = 5;
int iRedPin = 6;
boolean bEStopOn = false;
boolean bKeyPressed = false;
unsigned long lOnStartMillis;
unsigned long lCurrentMillis;      
const unsigned long lKeyHoldMillis = 100;
const unsigned long lKeyDebounce = 50;
char cEStopKey = ' ';
const String kESTOP = "estop\r\n";
const String kCLEAR = "clear\r\n";


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.setTimeout(50);
  delay(500);
  pinMode(iGreenPin, OUTPUT);
  pinMode(iRedPin, OUTPUT);
  Keyboard.begin();
}

void loop() {
  if (Serial1.available() > 0) {
    String sSignalIn = Serial1.readString();
    Serial.print(sSignalIn);
    if (sSignalIn == kESTOP) {
      bEStopOn = true;
      lOnStartMillis = millis() + lKeyHoldMillis;
      Keyboard.press(cEStopKey);
      bKeyPressed = true;

    } else {
      if (sSignalIn.substring(0) == kCLEAR) {
        bEStopOn = false;
      }
    }
    sSignalIn = "";
  }

  if (bEStopOn) {
    digitalWrite(iGreenPin, LOW);
    digitalWrite(iRedPin, HIGH);
    if ((lOnStartMillis < millis()) && bKeyPressed) {
      Keyboard.releaseAll();
      bKeyPressed = false;
    } 
  }
  else {
    if (bKeyPressed) {
      Keyboard.releaseAll();
      bKeyPressed = false;
    }
    digitalWrite(iGreenPin, HIGH);
    digitalWrite(iRedPin, LOW);
  }

}