#include "SmartChair.h"

SmartChair::SmartChair(
    uint8_t pinBarDclk,
    uint8_t pinBarDi,
    uint8_t pinUltrasonicRangeFinder,
    uint8_t pinVibrationMotor,
    uint8_t sittingDurationInSec,
    uint8_t restorationDurationInSec,
    uint8_t rangeThreshold) :
      _energyBar (pinBarDclk, pinBarDi, 0),
      _ultrasonic (pinUltrasonicRangeFinder) {

  _energy = 255;
  setDurations(sittingDurationInSec, restorationDurationInSec);
  _rangeThreshold = rangeThreshold;
  _lastSitCheck_ms = 0;
  _nextConfigCheck_ms = 0;
  _pinVibrationMotor = pinVibrationMotor;
  pinMode(_pinVibrationMotor, OUTPUT);
  vibrationState = LOW;
}

void SmartChair::init() {
  _energyBar.init();
  _firebaseDatabaseSC.init();
  checkConfig();
}

void SmartChair::setDurations(uint8_t sittingDurationInSec,
    uint8_t restorationDurationInSec) {
  _secittingDurationInSec = sittingDurationInSec;
  _restorationDurationInSec = restorationDurationInSec;
}

uint8_t SmartChair::getEnergy() {
  return _energy;
}

void SmartChair::check() {
  if (_lastSitCheck_ms + 1000 < millis()) {
    int range = _ultrasonic.MeasureInCentimeters();
    Serial.print("ultrasonic = ");
    Serial.println(range);

    if (range < _rangeThreshold) {
      if (_isPersonSitting) {
        _isPersonSitting = 0;
        Serial.println("sat down");
        _firebaseDatabaseSC.sitDown();
      } else {
        int energyChange = (millis() - _lastSitCheck_ms) /
            ((uint64_t)_secittingDurationInSec * 1000 / 256);
        if (_energy > energyChange) {
          _energy -= energyChange;
          saveEnergy();
        } else if (_energy != 0) {
          _energy = 0;
          saveEnergy();
        } else {
          digitalWrite(_pinVibrationMotor, !vibrationState);
          vibrationState = !vibrationState;
        }
      }
    } else {
      if (!_isPersonSitting) {
        _isPersonSitting = 1;
        Serial.println("got up");
        digitalWrite(_pinVibrationMotor, LOW);
        _firebaseDatabaseSC.standUp();
      } else {
        int energyChange = (millis() - _lastSitCheck_ms) /
            ((uint64_t)_restorationDurationInSec * 1000 / 256);
        if (_energy + energyChange < 255) {
          _energy += energyChange;
          saveEnergy();
        } else if (_energy != 255) {
          _energy = 255;
          saveEnergy();
        }
      }
    }
    _lastSitCheck_ms = millis();
  }

  if (millis() > _nextConfigCheck_ms) {
    checkConfig();
    _nextConfigCheck_ms += 20000;
  }
}

void SmartChair::saveEnergy() {
  _energyBar.setEnergy(_energy);
  _firebaseDatabaseSC.setEnergy(_energy);
  Serial.print("energy = ");
  Serial.println(_energy);
}

void SmartChair::checkConfig() {
  /*Firebase arduino lib has method for streaming pobject from FirebaseDatabase
  but there is issue that it's not possible to write to base during streaming
  */
  Serial.println("checkConfig");

  int sittingDurationFbInSec = _firebaseDatabaseSC.getFbSittingDurationInSec();
  if (sittingDurationFbInSec == 0) {
    Serial.println("SittingDuration in incorrect");
    return;
  }

  int restorationDurationFbInSec = _firebaseDatabaseSC.getFbRestoreDurationInSec();
  if (restorationDurationFbInSec == 0) {
    Serial.println("RestorationDuration in incorrect");
    return;
  }

  if (sittingDurationFbInSec != _secittingDurationInSec &&
    restorationDurationFbInSec != _restorationDurationInSec) {
      setDurations(sittingDurationFbInSec, restorationDurationFbInSec);
      Serial.print("PomodoroSittingDuration set to ");
      Serial.println(_secittingDurationInSec / 60);
      Serial.print("PomodoroRestoreDuration set to ");
      Serial.println(_restorationDurationInSec / 60);
    }
}
