#include <math.h>
#include <Wire.h>
#include "filters.h"
#include "FinalEMG.h"

#define INTERRUPT_RATE  2000 

const int analogInPin = A2; 
const int WINDOW_SIZE  = 100;
const float cutoff_freq   = 0.0003;  
const float sampling_time = 2000;

IIR::ORDER  order = IIR::ORDER::OD2; 
Filter lowpass(cutoff_freq, sampling_time, order);

char pattern_num = 0;

int pattern_tmp = 0;
int patternFlag = 0;
int emgsigCount = 0;

/* --- EMG --- */
long rawdata[2] = {0, 0}; 
long emgdata[2] = {0, 0}; 
long fildata[2] = {0, 0}; 

int readIndex = 0; 

int smooth1[WINDOW_SIZE];      
int sum1, aver1 = 0;      
int smooth2[WINDOW_SIZE];      
int sum2, aver2 = 0;

/* --- FMG --- */
float fmgValue = 0;

void Intan_Setup()
{
  SendWriteCommand(0x00, 0xDE); 
  SendWriteCommand(0x01, 0x60);
  SendWriteCommand(0x02, 40);
  SendWriteCommand(0x03, 0x00);
  SendWriteCommand(0x04, 0xF6);
  SendWriteCommand(0X05, 0x00);
  SendWriteCommand(0X06, 0x00);
  SendWriteCommand(0X07, 0x00);

  SendWriteCommand(8, 46);
  SendWriteCommand(9, 2);
  SendWriteCommand(10, 30);
  SendWriteCommand(11, 3);
  
  uint8_t R12, RL, RLDAC1, R13, ADCaux3en, RLDAC3, RLDAC2;
 
  RL = 0;
  RLDAC1 = 8;

  ADCaux3en = 0;
  RLDAC3 = 0;
  RLDAC2 = 3;

  R12 = ((RL << 7) | RLDAC1);
  R13 = (ADCaux3en << 7) | (RLDAC3 << 6) | RLDAC2;

  SendWriteCommand(12, R12);
  SendWriteCommand(13, R13);

  SendWriteCommand(14, 3); //CH4 -> 15
  SendWriteCommand(15, 0);
  SendWriteCommand(16, 0);
  SendWriteCommand(17, 0);

  Calibrate(); 
  
  SendConvertCommandH(0);
  SendConvertCommand(0);
  SendConvertCommandH(1);
  SendConvertCommand(1);
}

void setup()
{
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT); 

  Serial.begin(115200);
  Serial1.begin(115200);

  SPI.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE0));
  delay(250); 

  Intan_Setup();

  for(int i=0 ; i< WINDOW_SIZE ; i++)
  {
    smooth1[i] = 0;
    smooth2[i] = 0;
  }

  pattern_num = '0';
}

void loop()
{
  process_signal();
  smooth_lowpass();
  
  /* >>> Method <<< */
  Pattern_EMG();
  
  if(pattern_tmp == 1)
  {
    pattern_num = '1'; // Grasping Posture
    Serial1.write('1');
  }
  
  if(pattern_tmp == 2)
  {
    pattern_num = '2'; // Grasping Posture
    Serial1.write('2');
  }

  if(pattern_tmp == 3)
  {
    pattern_num = '3'; // Tripod Posture
    Serial1.write('3');
  }
  /* >>> SCI PA <<< */  
  Serial.print(aver1);Serial.print("\t"); 
  Serial.print(fmgValue);Serial.print("\t");
  Serial.print(pattern_num);Serial.print("\t");
  Serial.print(emgsigCount);Serial.println("");
}

void process_signal()
{
  // EMG
  for(int i=0 ; i<2 ; i++)
  {
    rawdata[i] = SendConvertCommand(i);
    emgdata[i] = (int) (rawdata[i] * 0.195);
    fildata[i] = lowpass.filterIn(emgdata[i]);
  }
  // FMG
  fmgValue = analogRead(analogInPin);
}

void smooth_lowpass()
{  
  sum1 = sum1 - smooth1[readIndex];
  sum2 = sum2 - smooth2[readIndex];
  
  smooth1[readIndex] = fildata[0];
  smooth2[readIndex] = fildata[1];
  
  sum1 = sum1 + smooth1[readIndex];
  sum2 = sum2 + smooth2[readIndex];
  
  readIndex = readIndex +1;

  if(readIndex >= WINDOW_SIZE)
  {
    readIndex = 0;
  }
  aver1 = sum1 / WINDOW_SIZE;
  aver2 = sum2 / WINDOW_SIZE;
}

void Pattern_EMG()
{ 
  if(fmgValue >= 60 && aver1 < 50)
  {
    patternFlag = 1;
  }
  
  if(aver1 >= 100)
  {
    patternFlag = 2;
    emgsigCount = emgsigCount + 1;
  }

  if(patternFlag == 1)
  {
    pattern_tmp = 1;
  }
  
  if(patternFlag == 2 && aver1 < 50)
  {
    if(emgsigCount > 100 && emgsigCount < 450)
    {
      pattern_tmp = 2;
    }

    else if(emgsigCount >= 500)
    {
      pattern_tmp = 3;
    }
    
    emgsigCount = 0;
  }
}

void Pattern_EF()
{
  if(fmgValue < 80 && aver1 < 50)
  {
    pattern_tmp = 1;
  }

  else if(fmgValue >= 80 && aver1 >= 50)
  {
    pattern_tmp = 2;
  }

  else if(fmgValue >= 80 && aver1 < 50)
  {
    pattern_tmp = 3;
  }
}
