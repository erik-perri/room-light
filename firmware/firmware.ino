#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin = 14;
const uint16_t kButtonPin = 12;
const uint16_t kLdrPin = A0;

const int LIGHT_THRESHOLD = 18; 

// ON = 9C
const uint32_t CODE_ON = 0x01FE39C6; 

// OFF = DD
const uint32_t CODE_OFF = 0x01FEBB44; 

IRsend irsend(kIrLedPin);

void setup() {
  Serial.begin(115200);
  irsend.begin();
  pinMode(kButtonPin, INPUT_PULLUP);
}

void loop() {
  int currentBrightness = analogRead(kLdrPin);
  bool isLightOn = (currentBrightness > LIGHT_THRESHOLD);

  if (digitalRead(kButtonPin) == LOW) {
    if (isLightOn) {
      Serial.println("Sensor sees LIGHT -> Sending OFF (Code DD)...");

      irsend.sendNEC(CODE_OFF, 32, 3);
      
    } else {
      Serial.println("Sensor sees DARK -> Sending ON (Code 9C)...");

      irsend.sendNEC(CODE_ON, 32, 3);
    }
    
    while(digitalRead(kButtonPin) == LOW) {
      delay(10);
    }
    delay(1000); 
  }
}
