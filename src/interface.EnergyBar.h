#ifndef INTERFACE_ENERGY_BAR
#define INTERFACE_ENERGY_BAR
#ifndef INTERFACE_ENERGY_BAR
#define INTERFACE_ENERGY_BAR

class iEnergyBar
{
    public:
      //virtual ~iEnergyBar(){}

      virtual void init();
      virtual void setEnergy(uint8_t energy);

};
#endif




// class EnergyBar {
// public:
//   //Create object with Grove_LED_Bar on specified ports
//   EnergyBar(uint8_t pinDclk, uint8_t pinDi, uint8_t direction);
//
//   //Init Grove_LED_Bar object
//   void init();
//
//   //SetEnergy in proportion to the energy variable (limits from 0 to 255)
//   void setEnergy(uint8_t energy);
//
// private:
//   Grove_LED_Bar _ledBar;
//
// };
// #endif
