#ifndef MC145193_250_h
#define MC145193_250_h 

// library interface description
#include <Arduino.h>

class MC145193_
{
public: 
     void begin(void);
     void divider_R(unsigned int R);
     void divider_A_B(char A, unsigned int B);
     void imposta_f(unsigned long int f); 
     void shiftOutput(int pinData, int pinClock, char bitOrder, char valByte);
private: 
};

extern MC145193_ MC145193;

#endif