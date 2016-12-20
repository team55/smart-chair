#include <Arduino.h>
#include "SmartChair.h"


const int kPinUltrasonicRangeFinder = 14;
const int kSittingDurationInSec = 2*60;
const int kRestorationDuration_s = 30;
const int kRangeThreshold = 10;
const int kPinVibrationMotor = 5;

#if defined(NodeMCU)

  //todo поменять конструктор

  const int kPinBarDclk = 2;
  const int kPinBarDi = 13;
  SmartChair smartChair(kPinBarDclk, kPinBarDi, kPinUltrasonicRangeFinder,
    kPinVibrationMotor, kSittingDurationInSec, kRestorationDuration_s, kRangeThreshold);


#else
  const int kPinBarDclk = 2;
  const int kPinBarDi = 13;

  SmartChair smartChair(kPinBarDclk, kPinBarDi, kPinUltrasonicRangeFinder,
      kPinVibrationMotor, kSittingDurationInSec, kRestorationDuration_s, kRangeThreshold);

#endif

void setup() {

  #if defined(NodeMCU)

  #else
    //Turn on GPIO on Wio Board
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);
  #endif


  //Start serial port
  Serial.begin(28800);

  //Init SmartChair
  smartChair.init();
  Serial.println("setup finished");
}

void loop() {
  smartChair.check();
}
