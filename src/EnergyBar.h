#ifndef EnergyBar_h
#define EnergyBar_h

#include <Arduino.h>
#include <Grove_LED_Bar.h>
#include <interface.Energybar.h>

class EnergyBar : public iEnergyBar {
public:
  //Create object with Grove_LED_Bar on specified ports
  EnergyBar(uint8_t pinDclk, uint8_t pinDi, uint8_t direction);

  //Init Grove_LED_Bar object
  void init();

  //SetEnergy in proportion to the energy variable (limits from 0 to 255)
  void setEnergy(uint8_t energy);

private:
  Grove_LED_Bar _ledBar;

};
#endif
