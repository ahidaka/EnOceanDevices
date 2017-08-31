#include "ram0.h"
#include "DolphinSTM.h"

// ------------------------------------------------------------------
// global
tRam0 aRam0;

// ------------------------------------------------------------------
// Function:    ram0Init
// Parameters:  none
//              
//              
// Returns:     nothing
// Description: initializes aRam0 structure to default values  
//              
// ------------------------------------------------------------------
void ram0Init(void)  {

  aRam0.u8State = S_IDLE;
  aRam0.u16MinCO2Level = 1000;
  aRam0.u16WakeupCnt = 0;
  aRam0.u8ChargeLevel = 0;

  aRam0.u16WakeInterval = unigcfg.u16WakeCycle; 
  aRam0.u8CycleMin = unigcfg.u8TCycleMin;
  aRam0.u8CycleMax = unigcfg.u8TCycleMax;
  aRam0.u8LastVOFF = 0;
}
// ------------------------------------------------------------------
// Function:    ram0Read
// Parameters:  none
//              
//              
// Returns:     nothing
// Description: stores aRam0 structure in RAM0 
//              
// ------------------------------------------------------------------
void ram0Read(void)  {
  mem_readRAM0((uint8 *)&aRam0, 0, sizeof(aRam0)); 
}
// ------------------------------------------------------------------
// Function:    ram0Write
// Parameters:  none
//              
//              
// Returns:     nothing
// Description: restores aRam0 structure from RAM0  
//              
// ------------------------------------------------------------------
void ram0Write(void)  {
  mem_writeRAM0((uint8 *)&aRam0, 0, sizeof(aRam0)); 
}


