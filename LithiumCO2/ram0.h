#ifndef _RAM0_H_
#define _RAM0_H_

#include "EO3100I_API.h"

// ------------------------------------------------------------------
#define S_IDLE                 0
#define S_MEASURING            1
// ------------------------------------------------------------------
// Structure of RAM0 area
typedef struct 
{	
  uint8 u8State;                                                      // used to store system state during deep sleep
  uint8 u8ChargeLevel;                                                // used to store charge level during sleep 
  uint16 u16MinCO2Level;                                              // used for dynamic sensor calibration
  uint16 u16WakeupCnt;
  
  uint16 u16WakeInterval;
  uint8 u8CycleMin;
  uint8 u8CycleMax;                                                   // calibration timings
  uint8 u8LastVOFF;                                                   // control WXODIO to low
} tRam0;
// ------------------------------------------------------------------
// public vars
extern tRam0 aRam0;
// ------------------------------------------------------------------
// public funcions    
void ram0Init(void);
void ram0Read(void);
void ram0Write(void);



#endif  //_RAM0_H_
