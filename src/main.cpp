#include <Arduino.h>
#include "SmartChair.h"

const int kSittingDurationInSec = 2*60;
const int kRestorationDuration_s = 30;
const int kRangeThreshold = 10;
const int kPinVibrationMotor = 5;

#if defined(NodeMCU)

  //todo поменять конструктор
  //создавать энержибар интерфейс-объект передавать в конструктор укзатель
  //установка значения будет для объекта интерфейса а не конкретной имплементации

  iEnergyBar EnergyBar(1,1,1);
  SmartChair smartChair(kSittingDurationInSec, kRestorationDuration_s, kRangeThreshold);
  const int kPinBarDclk = 2;
  const int kPinBarDi = 13;
  const int kPinUltrasonicRangeFinder = 14;

  SmartChair smartChair(kPinBarDclk, kPinBarDi, kPinUltrasonicRangeFinder,
    kPinVibrationMotor, kSittingDurationInSec, kRestorationDuration_s, kRangeThreshold);


#else
  const int kPinBarDclk = 2;
  const int kPinBarDi = 13;
  const int kPinUltrasonicRangeFinder = 14;

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
