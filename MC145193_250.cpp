#include <MC145193_250.h>

/*********************** Scrittura registri MC145193 **************************/

#define DATA  A0  
#define CLK   A1
#define N_ENB A2

#define MSB   1
#define LSB   0
 // Registro C: opzioni generiche
#define C B01101101          // B01101101
#define Fref 15000           // Passo 

void MC145193_::begin(void)
{
char B;
pinMode(N_ENB,OUTPUT);      //  ENB (alto default))
digitalWrite(N_ENB,HIGH);
pinMode(CLK,OUTPUT);        //  CLK basso 
digitalWrite(CLK, LOW);
pinMode(DATA,OUTPUT);       //  Data basso
digitalWrite(DATA, LOW);
delay(1); 
digitalWrite(N_ENB,LOW);    //  ENB basso per iserire i dati
// Registro di SETUP
B=C;
shiftOutput(DATA,CLK,MSB,B); 
digitalWrite(DATA,LOW);
digitalWrite(N_ENB,HIGH);
} 

void MC145193_::divider_R(unsigned int R)
{
char Byte_0, Byte_1;
Byte_0=lowByte(R);
Byte_1=highByte(R);
Byte_1=Byte_1 | B00100000;  // Abilitato il quarzo (1->b001)
// Invio i 2 byte di R (16 bit) 
digitalWrite(N_ENB,LOW);    // /ENB basso per iserire i dati
shiftOutput(DATA,CLK,MSB,Byte_1);
shiftOutput(DATA,CLK,MSB,Byte_0); 
digitalWrite(DATA, LOW); 
digitalWrite(N_ENB,HIGH);
}

void MC145193_::imposta_f(unsigned long int f)
{
unsigned long int G; unsigned int B; char A;    // f=Fref(64*B + A) = 64*Fref*B+Fref*A
G=(uint32_t)64*Fref;    // = 960 000
B=(unsigned int)(f/G);
//
A=(f%G)/Fref;
divider_A_B(A,B);
}

void MC145193_::divider_A_B(char A, unsigned int B)       // con F=250MHz -> B=325;  A= 17,11111
{
char B_0, B_1;
// B contiene 12 bit significativi  
B_0=lowByte(B);  // solo i 8 bit - signif. di B
B_1=highByte(B); // solo i 4 bit + signif. di B
B_1=B_1 | B01110000;  // 0111 XXXX di B
// Invio di 2 byte con B (16 bit)
digitalWrite(N_ENB,LOW);     // /ENB basso per iserire i dati
shiftOutput(DATA,CLK,MSB,B_1);
shiftOutput(DATA,CLK,MSB,B_0); 
digitalWrite(DATA, LOW); 
// Invio 1 byte di A 
shiftOutput(DATA,CLK,MSB,A);
digitalWrite(DATA, LOW);
digitalWrite(N_ENB,HIGH);    // /ENB alto per terminare il setup
}

void MC145193_::shiftOutput(int pinData, int pinClock, char bitOrder, char valByte)
{
char n,valBit; byte mask;

digitalWrite(pinClock,LOW);
delayMicroseconds(500);
if (bitOrder==MSB)  // parte dal MSB
   { 
   mask=B10000000;
   for(n=0;n<=7;n++)
      {
      valBit=(char)(valByte & mask);
      if (valBit!=0)
          digitalWrite(pinData,HIGH); 
      else
          digitalWrite(pinData,LOW);
      delayMicroseconds(150);   
      digitalWrite(pinClock,HIGH);
      delayMicroseconds(500);
      digitalWrite(pinClock,LOW);
      delayMicroseconds(150);
      mask=mask>>1;
      } 
   }
else              // parte dal LSB
   {
   mask=B00000001;
   for (n=0;n<=7;n++)
      {
      valBit=(char)(valByte & mask);
      if (valBit!=0)
         digitalWrite(pinData,HIGH);
      else
         digitalWrite(pinData,LOW); 
      delayMicroseconds(150); 
      digitalWrite(pinClock,HIGH);  
      delayMicroseconds(500);
      digitalWrite(pinClock,LOW);
      mask=mask<<1;
      }
   }
delayMicroseconds(100);   
digitalWrite(pinData,LOW); 
delay(1);
}

MC145193_ MC145193;