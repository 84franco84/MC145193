
/*******************************************************************************
  PLL 55-75 MHz V 1.1 con MC145193, ERA3 e BF988
*******************************************************************************/

#include <MC145193.h>
#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);     // UNO

void setup() 
  { 
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("- START -");
  MC145193.begin();
  delay(1);
  }

void loop() 
  {
  unsigned long int f;
  delay(1);
  /* da 55 a 74 ->    (65 a 84 in ricezione) */
  /* da 132 a 163 ->  (136 a 170 in ricezione) */
  f=163000000;
  MC145193.imposta_f(f);
  /*
  do
  {
  for(f=52000000;f<72000000;f=f+12500)
    MC145193.imposta_f(f);
  } while(true);
  */
  delay(500);
  lcd.clear();
  lcd.print("F = "+String(f));
  while(true){};
  }






