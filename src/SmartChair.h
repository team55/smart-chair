#ifndef SmartChair_h
#define SmartChair_h

#include <Arduino.h>
#include "EnergyBar.h"
#include "FirebaseDatabaseSC.h"
#include <Ultrasonic.h>

//Интерфейсы оборудования
#include <IVibration.h>

class SmartChair {
public:
  SmartChair(
      uint8_t pinBarDclk,
      uint8_t pinBarDi,
      uint8_t pinUltrasonicRangeFinder,
      uint8_t pinVibrationMotor,
      uint8_t sittingDurationInSec,
      uint8_t restorationDurationInSec,
      uint8_t rangeThreshold);

  SmartChair(
          IVibration::SetState *callback,
          uint8_t pinBarDclk,
          uint8_t pinBarDi,
          uint8_t pinUltrasonicRangeFinder,
          uint8_t pinVibrationMotor,
          uint8_t sittingDurationInSec,
          uint8_t restorationDurationInSec,
          uint8_t rangeThreshold);

  void init();

  /*Set the resolution in the duration of the sitting position and the duration
  of needed rest*/
  void setDurations(uint8_t sittingDurationInSec,
      uint8_t restorationDurationInSec);

  //get Current energy status
  uint8_t getEnergy();

  //run all necessary checks
  void check();

private:
  uint8_t _energy;
  uint8_t _pinVibrationMotor;
  bool vibrationState;
  uint32_t _secittingDurationInSec;
  uint32_t _restorationDurationInSec;
  uint64_t _lastSitCheck_ms;
  uint64_t _nextConfigCheck_ms;
  boolean _isPersonSitting;
  uint16_t _durationPerPoint_ms;
  uint8_t _rangeThreshold;

  EnergyBar _energyBar;
  FirebaseDatabaseSC _firebaseDatabaseSC;
  Ultrasonic _ultrasonic;

  void saveEnergy();
  void checkConfig();
};

#endif
