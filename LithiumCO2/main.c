#include "EO3100I_API.h"
#include "EO3100I_CFG.h"
//#include "EO3100I_API_DBG.h"
#include "DolphinSTM.h"
#include <stdio.h>
#include <string.h>
#include "ram0.h"
#include "LRNtelegram.h"

//#define DEBUG 1 // for EOP350 test
code uint8 VERSION_APP[] = {0xE0,'V','E','R','S','I','O','N',
                            0,0,0,8,                              	// change APP Version here  
                            'A',
							'L','i','t','h','i','u','m','C','O','2',
                            0x00,0xE0};

// defines for DEBUG and function control
#define TX_OUTPUT 0

//#define CO2TIMEOUT 2000 //--NG
//#define CO2TIMEOUT 4000 //--NG
#define CO2TIMEOUT  50000 //-- OK
//#define CO2TIMEOUT  20000 -- NG
//#define DEBUG 1
//#define SEND_TELEGRAM 1
//#define LRN_DEBUG 1
#define DEBUG_CALIB_TELEGRAM 1

// I/O Configuration overview
//
//      Pin :  Interface      Direction   Pull   InitValue    Interrupt
// SCSEDIO0 :  Digital I/O    In          Up     0            No     
// SCLKDIO1 :  Digital I/O    In          Up     0            No     
// WSDADIO2 :  Digital I/O    In          Up     0            No     
// RSDADIO3 :  Digital I/O    In          Up     0            No     
// ADIO0    :  Digital I/O    In          Up     0            No     
// ADIO1    :  Digital I/O    In          Up     0            No     
// ADIO2    :  Digital I/O    In          Up     0            No     
// ADIO3    :  Digital I/O    Out         None   0            No     
// ADIO4    :  Digital I/O    In          Up     0            No     
// ADIO5    :  Digital I/O    In          Up     0            No     
// ADIO6    :  UART           In          Up     0            No     
// ADIO7    :  UART           Out         None   0            No     
// WXIDIO   :  Digital I/O    In          None   0            No     
// WXODIO   :  Von            Out         None   0            No     
// WAKE0    :  Digital I/O    In          None   0            No     
// WAKE1    :  Digital I/O    In          None   0            No    

//*********************IO PARAM******************************
uint8 code io_param_sensorOff[]  = { 
   0x07,   //IDX_GPIO_CONF
   0x00,   //IDX_GPIO0_CONF
   0x0F,   //IDX_GPIO0_PULL_CONF
   0x00,   //IDX_GPIO0_DIR
   0x00,   //IDX_GPIO1_AN
   0x00,   //IDX_GPIO1_CONF0
   0xC0,   //IDX_GPIO1_CONF1
   0x03,   //IDX_GPIO1_DIG_CONF 
   0x88,   //IDX_GPIO1_DIR
   0x77,   //IDX_GPIO1_PULL
   0x02,   //IDX_GPIO2_CONF	//WXODIO_VON//
   0x03,   //IDX_GPIO2_DIR
   0x00,   //IDX_GPIO0
   0x00,   //IDX_GPIO1
   0x00,   //IDX_GPIO2
};

uint8 code io_param_sensorOn[]  = { 
   0x07,   //IDX_GPIO_CONF
   0x00,   //IDX_GPIO0_CONF
   0x0F,   //IDX_GPIO0_PULL_CONF
   0x00,   //IDX_GPIO0_DIR
   0x00,   //IDX_GPIO1_AN
   0x00,   //IDX_GPIO1_CONF0
   0xC0,   //IDX_GPIO1_CONF1
   0x03,   //IDX_GPIO1_DIG_CONF 
   0x88,   //IDX_GPIO1_DIR
   0x77,   //IDX_GPIO1_PULL
   0x02,   //IDX_GPIO2_CONF	//WXODIO_VON//
   0x03,   //IDX_GPIO2_DIR
   0x00,   //IDX_GPIO0
   0x00,   //IDX_GPIO1
   0x01,   //IDX_GPIO2
};

uint8 code io_param_sensorOff_VOFF[]  = { 
   0x07,   //IDX_GPIO_CONF
   0x00,   //IDX_GPIO0_CONF
   0x0F,   //IDX_GPIO0_PULL_CONF
   0x00,   //IDX_GPIO0_DIR
   0x00,   //IDX_GPIO1_AN
   0x00,   //IDX_GPIO1_CONF0
   0xC0,   //IDX_GPIO1_CONF1
   0x03,   //IDX_GPIO1_DIG_CONF 
   0x88,   //IDX_GPIO1_DIR
   0x77,   //IDX_GPIO1_PULL
   0x00,   //IDX_GPIO2_CONF //WXOD_IO-OFF//
   0x03,   //IDX_GPIO2_DIR
   0x00,   //IDX_GPIO0
   0x00,   //IDX_GPIO1
   0x00,   //IDX_GPIO2
};

uint8 code io_param_sensorOn_VOFF[]  = { 
   0x07,   //IDX_GPIO_CONF
   0x00,   //IDX_GPIO0_CONF
   0x0F,   //IDX_GPIO0_PULL_CONF
   0x00,   //IDX_GPIO0_DIR
   0x00,   //IDX_GPIO1_AN
   0x00,   //IDX_GPIO1_CONF0
   0xC0,   //IDX_GPIO1_CONF1
   0x03,   //IDX_GPIO1_DIG_CONF 
   0x88,   //IDX_GPIO1_DIR
   0x77,   //IDX_GPIO1_PULL
   0x00,   //IDX_GPIO2_CONF //WXOD_IO-OFF//
   0x03,   //IDX_GPIO2_DIR
   0x00,   //IDX_GPIO0
   0x00,   //IDX_GPIO1
   0x01,   //IDX_GPIO2
};

// ------------------------------------------------------------------
//#define WAKE_INTERVAL        600ul // s                              // wakeup configuration
//#define WAKE_INTERVAL        60ul //=>4m12s s                              // wakeup configuration
//#define WAKE_INTERVAL        30ul //=>3m43s s                              // wakeup configuration

#ifdef DEBUG
#define WAKE_INTERVAL        15ul                                   // wakeup configuration (debug)
#else
#define WAKE_INTERVAL        600ul                                  // wakeup configuration 10 minutes (default)
//#define WAKE_INTERVAL        15ul									  // for Test
#endif

#ifdef DEBUG
#define CALIB_COUNT          6    // every 6 times, debug 
#else
#define CALIB_COUNT          (7ul*(24ul*60ul*60ul)/WAKE_INTERVAL)   // 1 week, calibration interval configuration 
#endif

#define DEFAULT_CALIBRATION_CLEAN_LEVEL 450                    // clean air assumes as 450 ppm

#define CALIBRATION_BUTTON_SEC 2 // 2 sec

#define SENSOR_TIMEOUT       3 // s                            // sensor timeout (max time from power on to first result)

#define VALID_CHARGE_LEVEL	 ((uint8)(1))                      // 0.5% -- default Valid ChargeLevel

// ------------------------------------------------------------------
#define ALLOW_LRN_BTN        (WAKE1_PIN_CFG_WAKE_FLAG)
#define ALLOW_SENSOR         (WAKE0_PIN_CFG_WAKE_FLAG)

#define WAKE_SENSOR          (WAKE_PIN0_RESET)
#define WAKE_LRN             (WAKE_PIN1_RESET)
// ------------------------------------------------------------------
#define sensorPwrOn()      (io_setDigital(WXID_IO, 1))         // power up sensor
#define sensorPwrOff()     (io_setDigital(WXID_IO, 0))         // power down sensor

#define ADJUST_TIMER(p) \
	do { \
		pwr_enableXTAL(DEFAULT_DELAY); \
		radio_init_mod(CFG_ALL); \
		pwr_selectXTAL(); \
	} while(0)
// ------------------------------------------------------------------
// prototypes
void measureTemperature(sint16 *s16value);
uint8 scaleTemperature(sint16 s16value); 
void measureVDD(uint16 *u16Value);
void measureChargeLevel(uint8 *u8Level);
void DeepSleep(uint16 u16Sleeptime, uint8 u8WakeSrc);
void parse_uart(uint16 *u16Value);

//
/****************************Variables***********************************/
//
DolphinSTM_CFG		code	unigcfg _at_ CFG_ADDR;
//
//
//
void main()
{
	//! Reset reason
	RESET_TYPE data u8ResetSrc;
	TEL_RADIO_TYPE2			rTel2;
	uint8 u8PinState;
	uint8 u8ChargeLevel;         // 0..200 (0..100%)
	sint16 s16Temp;
	uint16 u16CO2value;
	uint8 u8Len, u8Msg[40];
	uint16 WakeInterval;
	uint16 CalibCount;
	uint16 CalibCleanLevel;
	uint8  ValidChargeLevel;
	uint8  ControlWXODIO;
	uint8  ForceCalibration;

 	pwr_getSystemReset(&u8ResetSrc);

	// Control WXIDIO_OFF condition
	ControlWXODIO = unigcfg.u8AD_0_Threshold; //0: always OFF, 1: Depend on Charge Level, Other:always ON

	// need initial settings ASAP //
	switch(u8ResetSrc)
	{
		case WAKE_LRN:
		case WATCHDOG_RESET:
			io_getDigital(WAKE_1, &u8PinState);	// Get WAKE_1 pin state here!
			ram0Read();
			io_init(ControlWXODIO == 0 || aRam0.u8LastVOFF ? io_param_sensorOff_VOFF : io_param_sensorOff);
			mainInit();
			break;

		case VDD_RESET:
		case PIN_RESET:
			ram0Init();
			ram0Write();
			io_init(ControlWXODIO == 0 ? io_param_sensorOff_VOFF : io_param_sensorOff);
			mainInit();
			break;

		case WAKE_SENSOR:                                                 // WAKE0 -> UART activity
			ram0Read();
			io_init(ControlWXODIO == 0 || aRam0.u8LastVOFF ? io_param_sensorOn_VOFF : io_param_sensorOn);
			mainInit();                                                   // important that uart_init hits the stop bit of the first char (0x20)
			break;
	}

	// Wakeup interval
	if (unigcfg.u16WakeCycle == FLASH_ERASED_16)
	{
		WakeInterval = WAKE_INTERVAL;	
	}
	else
	{
		WakeInterval = unigcfg.u16WakeCycle;
		if (WakeInterval == 0)
			WakeInterval = WAKE_INTERVAL;
	}

	// CalibCount
	if (unigcfg.u8TCycleMin == FLASH_ERASED_8 && unigcfg.u8TCycleMax == FLASH_ERASED_8)
	{
		CalibCount = CALIB_COUNT;
	}
	else
	{
		CalibCount = unigcfg.u8TCycleMin * unigcfg.u8TCycleMax;
	}

	// Compare Charge Level
	if (unigcfg.u8AD_2_Threshold == FLASH_ERASED_8)
	{
		ValidChargeLevel = VALID_CHARGE_LEVEL;	
	}
	else
	{
		ValidChargeLevel = unigcfg.u8AD_2_Threshold;
	}

	// Compare Calibration Clean Level
	if (unigcfg.u8AD_1_Threshold == FLASH_ERASED_8)
	{
		CalibCleanLevel = DEFAULT_CALIBRATION_CLEAN_LEVEL;	
	}
	else
	{
		CalibCleanLevel = unigcfg.u8AD_1_Threshold * 10;
	}

#ifdef DEBUG2
	u8Len = sprintf(u8Msg, "W:%u C:%u V:%u\r\n", WakeInterval, CalibCount, (uint16)ValidChargeLevel);
#endif

	switch(u8ResetSrc)
	{
		case WAKE_LRN:                                                    // WAKE1 -> LRN button
#ifdef LRN_DEBUG
			u8Len = sprintf(u8Msg, "WAKE_LRN PinState=%u\r\n", (uint16) u8PinState);
			uart_sendBuffer(u8Msg, u8Len);
			do
			{ 
				uart_getTxStatus(&u8Len);
			}
			while(u8Len > 0);
#endif
			if(u8PinState == 0)
			{                                                             // pressed  -> send learn telegram 
				SendLrnTelegram2();
#ifdef LRN_DEBUG
				u8Len = sprintf(u8Msg, "Sent LrnTelegram\r\n");
				uart_sendBuffer(u8Msg, u8Len);
				do
				{ 
					uart_getTxStatus(&u8Len);
				}
				while(u8Len > 0);

				///////////////////////
				DeepSleep(CALIBRATION_BUTTON_SEC, ALLOW_LRN_BTN);
				///////////////////////
#endif
			}
			DeepSleep(0, ALLOW_LRN_BTN);                                  // continue sleeping (0)
			break;

		case VDD_RESET:
		case PIN_RESET:
#ifdef LRN_DEBUG
			u8Len = sprintf(u8Msg, "VDD or PIN Reset=%u\r\n", (uint16) u8ResetSrc);
			uart_sendBuffer(u8Msg, u8Len);
			do
			{ 
				uart_getTxStatus(&u8Len);
			}
			while(u8Len > 0);
#endif
			DeepSleep(WakeInterval, ALLOW_LRN_BTN);
			break;

		case WATCHDOG_RESET:
#ifdef LRN_DEBUG
			u8Len = sprintf(u8Msg, "WAKE_WDG PinState=%u\r\n", (uint16) u8PinState);
			uart_sendBuffer(u8Msg, u8Len);
			do
			{ 
				uart_getTxStatus(&u8Len);
			}
			while(u8Len > 0);
#endif
			ForceCalibration = u8PinState == 0;

			if (aRam0.u8CycleMin != unigcfg.u8TCycleMin
		  	 || aRam0.u8CycleMax != unigcfg.u8TCycleMax)
			{
				aRam0.u8CycleMin = unigcfg.u8TCycleMin;
				aRam0.u8CycleMax = unigcfg.u8TCycleMax;
				aRam0.u16WakeupCnt = 0;
			}

			if(aRam0.u8State == S_IDLE)
			{                                  
				aRam0.u16WakeupCnt++;                                       // store multiple of 10mins for calibration
				measureChargeLevel(&u8ChargeLevel);
				aRam0.u8ChargeLevel = u8ChargeLevel;                        // store value for radio transmission

#if TX_OUTPUT
				do
				{
					int i;
					for(i = 0; i < 10; i++)
					{
						while(uart_sendBuffer("UZUZUZUZUZUZUZUZUZUZ\r\n", 22) ==  BUFF_FULL)
							;
						do
						{
							uart_getTxStatus(&u8Len);
						}
						while(u8Len != OK);
					}
				}
				while(0);
				time_wait(500);	// 500ms
				ADJUST_TIMER();
#endif
#ifdef DEBUG
				u8Len = sprintf(u8Msg, "W:%u C:%u V:%u L:%u\r\n",
					WakeInterval, CalibCount, (uint16)ValidChargeLevel, (uint16) u8ChargeLevel);

				while(uart_sendBuffer(u8Msg, u8Len) ==  BUFF_FULL)
							;
				do
				{
					uart_getTxStatus(&u8Len);
				}
				while(u8Len != OK);
#endif
				if (ControlWXODIO != 0 || ValidChargeLevel != 0)
				{
					// low energy check
					if(u8ChargeLevel <= ValidChargeLevel)
					{
						// if chargelevel is below 20% stop here and sleep 
						aRam0.u8State = S_IDLE;
						sensorPwrOff();
	
						////////////////////////////////
						//for(i = 0; i < 7; i++)
						//{
						//	io_setDigital(ADIO_3,(bit1)1);
						//	time_wait(100);
						//	io_setDigital(ADIO_3,(bit1)0);
						//	time_wait(100);
						//}
						////////////////////////////////
	
						if (ControlWXODIO == 1)
						{
							aRam0.u8LastVOFF = 1;	           // mark it to Low // 
							io_init(io_param_sensorOff_VOFF);  // set WXODIO to low //
							mainInit();
						}
						DeepSleep(WakeInterval, ALLOW_LRN_BTN);
					}
					else if (aRam0.u8LastVOFF)
					{
						aRam0.u8LastVOFF = 0;
						io_init(io_param_sensorOff);
						mainInit();
					}
				}

				if(aRam0.u8CycleMin != 0 && aRam0.u8CycleMax != 0           // Calibration enabled ?
				  && aRam0.u16WakeupCnt >= CalibCount || ForceCalibration)  // or Force Calibration needed ?
				{
				    // need to recalibrate
					sensorPwrOn();                                          // power up sensor
					aRam0.u16WakeupCnt = 0;
					// possibly some sanity checks about  u16MinCO2Level should be performed?
					u8Len = sprintf(u8Msg, "F %u %u\r\n",
						aRam0.u16MinCO2Level, CalibCleanLevel);  // prepare F command
					time_wait(100);
					ADJUST_TIMER();
					while(uart_sendBuffer(u8Msg, u8Len) ==  BUFF_FULL)
						;                          // send F command
					do
					{ 
						uart_getTxStatus(&u8Len);
					}
					while(u8Len > 0);                                       // wait till send
					time_wait(90);									    
					sensorPwrOff();                                         // power up sensor
					// no error handling implemented here !!!!
#ifdef DEBUG_CALIB_TELEGRAM
					rTel2.raw.bytes[0]	=	0x22; 	// Header: Only 32 bit Originator-ID, no extended Header, 4BS telegram
					rTel2.raw.bytes[1]	=	0;		// Id	
					rTel2.raw.bytes[2]	=	0;		// Id	
					rTel2.raw.bytes[3]	=	0;		// Id	
					rTel2.raw.bytes[4]	=	0;		// Id
					rTel2.raw.bytes[5]	= aRam0.u8ChargeLevel;
					rTel2.raw.bytes[6]  = (uint8)(aRam0.u16MinCO2Level / 10); //** Debug Value **//
					rTel2.raw.bytes[7]  = 0; //scaleTemperature(s16Temp);
					rTel2.raw.bytes[8]  = 0x0F;
					rTel2.raw.u8Length	= 	10;

					radio_sendTelegram2(&rTel2, NULL);
#endif
					aRam0.u16MinCO2Level = 1000;
					DeepSleep(0, ALLOW_LRN_BTN);
				}
				else
				{
					sensorPwrOn();                                          // start measurement
					aRam0.u8State = S_MEASURING;
					DeepSleep(SENSOR_TIMEOUT, ALLOW_SENSOR);                // LRN button is disabled during measurement
				}
			}
			else  // (aRam0.u8State != S_IDLE)
			{                                                         // every 10 mins
				sensorPwrOff();                                       // Timeout !!
				aRam0.u8State = S_IDLE;
#ifdef DEBUG
				u8Len = sprintf(u8Msg, "Timeout ? %u\r\n", (uint16) aRam0.u8State);
				uart_sendBuffer(u8Msg, u8Len);
				do
				{ 
					uart_getTxStatus(&u8Len);
				}
				while(u8Len > 0);
#endif
#ifdef SEND_TELEGRAM
				rTel2.raw.bytes[0]	=	0x22; 	// Header: Only 32 bit Originator-ID, no extended Header, 4BS telegram
				rTel2.raw.bytes[1]	=	0;		// Id	
				rTel2.raw.bytes[2]	=	0;		// Id	
				rTel2.raw.bytes[3]	=	0;		// Id	
				rTel2.raw.bytes[4]	=	0;		// Id
				rTel2.raw.bytes[5]	= aRam0.u8ChargeLevel;
				rTel2.raw.bytes[6]  = 1; //** Debug Value **//
				rTel2.raw.bytes[7] = scaleTemperature(s16Temp); //** this is original **//
				rTel2.raw.bytes[8] = 0x08|0x02;
				rTel2.raw.u8Length	= 	10;

				radio_sendTelegram2(&rTel2, NULL);
#endif
				DeepSleep(WakeInterval, ALLOW_LRN_BTN);
			}
			break;

		case WAKE_SENSOR:                                             // WAKE0 -> UART activity
			sensorPwrOn(); // PowerON
			measureTemperature(&s16Temp);                             // measure temperature as early after powerup/wakeup as possible to avoid self heating
#ifdef DEBUG
			u16CO2value = (uint16) 999;
			while(uart_sendBuffer("SensorON\r\n", 10) ==  BUFF_FULL)
				;
			do
			{
				uart_getTxStatus(&u8Len);
			}
			while(u8Len != OK);
			time_wait(100);
			ADJUST_TIMER();
#else
			parse_uart(&u16CO2value);
#endif                                         
			sensorPwrOff();
  
			if((u16CO2value<aRam0.u16MinCO2Level)&&(u16CO2value!=0))      // CO2 = 0 -> timeout
				aRam0.u16MinCO2Level = u16CO2value;                       // store minimum of all CO2 results for calibration
        
			rTel2.raw.bytes[0]	=	0x22; 	// Header: Only 32 bit Originator-ID, no extended Header, 4BS telegram
			rTel2.raw.bytes[1]	=	0;		// Id	
			rTel2.raw.bytes[2]	=	0;		// Id	
			rTel2.raw.bytes[3]	=	0;		// Id	
			rTel2.raw.bytes[4]	=	0;		// Id

			rTel2.raw.bytes[5]	= aRam0.u8ChargeLevel;

			if(u16CO2value > 2550)
			{
				rTel2.raw.bytes[6] = 255;
			}
			else
			{
				rTel2.raw.bytes[6] = u16CO2value / 10; //** this is original **//
				////rTel2.raw.bytes[6] = 11;  // test value for CO2
			}

			rTel2.raw.bytes[7] = scaleTemperature(s16Temp); //** this is original **//
			////rTel2.raw.bytes[7] = 22;  // test value for temperatrue
			rTel2.raw.bytes[8] = 0x08|0x02;
			rTel2.raw.u8Length	= 	10;

			radio_sendTelegram2(&rTel2, NULL);

			aRam0.u8State = S_IDLE;  
			DeepSleep(WakeInterval, ALLOW_LRN_BTN); 
			break;
	}

	// We should not get here	
	while(1) {io_togDigital(ADIO_3); time_wait(300);};
}

 
// ------------------------------------------------------------------
// Function:    measureTemperature
// Parameters:  none
//              
//              
// Returns:     ps16value  raw value of temperature measurement
// Description: enables analog interface for measurement
//              measures voltage on T-sensor
//              disables analog interface 
// ------------------------------------------------------------------
void measureTemperature(sint16 *ps16value)  {
#if 1
	io_enableAnalog(5);                                             // VBG is configured as ADC reference to measure the internal temperature sensor analogue signal.
	io_ulpMeasAnalog(VTEMP, RVSS, ps16value);                       // measurements of temperature sensor
	io_enableAnalog(0);
#else // not worked, Chip temperature sensor calibration is needed
#define MEASURE_TIMES 8

    sint32  s32TempSum = 0;
    sint16  s16TemperatureAD[8];            // Temperature sensor A/D values
	uint8 u8i;

    // ******** Temperature Internal Sensor Measurement  ********
    io_enableAnalog(1);                                     		// activation of analogue measurement
                                                            
    for(u8i = 0; u8i < MEASURE_TIMES; u8i++)
	{
		io_ulpMeasAnalog(VTEMP, RVSS,  &s16TemperatureAD[u8i]);     // measurements of temperature sensor, stored in s16gTemperatureAD[]
	}

    // *********** Proccessing the informations ****************************
    for(u8i = 0; u8i < MEASURE_TIMES; u8i++)                      // The temperature A/D value is sampled repeatedly to obtain better precision.
    {
        s32TempSum += (sint32)(s16TemperatureAD[u8i]);
    }
    // Divide between 8 to average the 8 temperature measurements.
    *ps16value = (sint16) (s32TempSum / MEASURE_TIMES);

#undef MEASURE_TIMES
#endif
}
// ------------------------------------------------------------------
// Function:    scaleTemperature
// Parameters:  s16value  raw value of temperature measurement
//              
//              
// Returns:     uint8 result
// Description: scales the raw value to an uint8  
//              scaling requires calibration values in CFG area
// ------------------------------------------------------------------
uint8 scaleTemperature(sint16 s16value)  {
  float fTemp;

  // T =  Tcal  +  K*(D-Dcal) converts the Temperature A/D value in T[°C]
  fTemp = gModArea.val.f32CalibTemperature  +  gModArea.val.f32TemperatureSlope * (s16value - gModArea.val.s16CalibADTemperature);
  if(fTemp<0.0)           
    return 0;
  else if(fTemp>51.0)     
    return 255;
  else                    
    return (uint8)(fTemp/51.0 * 255.0);
}
// ------------------------------------------------------------------
// Function:    measureVDD
// Parameters:  none
//              
//              
// Returns:     pu16Value  scaled VDD value (10 bit resolution)
// Description: enables analog interface for measurement
//              measures VDD/4 voltage and returns a scaled VDD
//              disables analog interface    
// ------------------------------------------------------------------
void measureVDD(uint16 *pu16Value)  {
  sint16 s16Ref, s16Value;

  io_enableAnalog(1);
  io_ulpMeasAnalog(RVSS, RVDD, &s16Ref);                                   
  io_ulpMeasAnalog(VDD_4, RVSS, &s16Value);                           // measure VDD/4
  io_enableAnalog(0);
  io_ulpScaleAnalog(s16Ref, s16Value, 10, pu16Value);
  *pu16Value <<= 2;                                                   // VDD = 4*VDD_4                                                  // VDD = 4*VDD_4
} 
// ------------------------------------------------------------------
// Function:    measureChargeLevel
// Parameters:  none
//              
//              
// Returns:     pu8Level  charge level 0..100% (0..200) 
// Description: measures VDD and calculates a charge level 
//              the range from 1.8V .. 4.5V  is level 0 .. 100 %
//              
// ------------------------------------------------------------------
// Vdd/1.8V * (2^N-1)   ; N=10
#define CAPACITY_0            ((uint16)(2.4/1.8 * 1024))              // = 1365
#define CAPACITY_100          (CAPACITY_0 + 1200)                     // ca. 4.5V used 1200 to easy scale to 0..100% (see charge level)
// ------------------------------------------------------------------
void measureChargeLevel(uint8 *pu8Level)  {
  uint16 u16VDD;
  
  measureVDD(&u16VDD);
  if(u16VDD<=CAPACITY_0)            *pu8Level = 0;                     // 0%
  else if (u16VDD>=CAPACITY_100)    *pu8Level = 200;                   // 100%
  else                              *pu8Level = (u16VDD-CAPACITY_0)/6; // range is 1200/6 = 200
}
// ------------------------------------------------------------------
// Function:    DeepSleep
// Parameters:  u16Sleeptime    time to sleep in s
//                              if 0 than current watchdog time is used
//              u8WakeSrc       enables wake sources (WAKE0/1)
// Returns:     nonthing
// Description: sets the watchdog time
//              stores the ram0 data
//              enters deep sleep mode   
// ------------------------------------------------------------------
void DeepSleep(uint16 u16Sleeptime, uint8 u8WakeSrc)  {
  uint32 u32WatchdogPeriode;
  uint8 u8Stable;

  if(u16Sleeptime!=0)  {
    u32WatchdogPeriode = (uint32)((uint32)CONV_TIME_TO_COUNTER(u16Sleeptime, SEC,WATCHDOG_CLK)*gModArea.val.u16CalibWatchdog)/WATCHDOG_CALIB_CONST;
    pwr_setSleepTimer(WATCHDOG_TIMER, u32WatchdogPeriode,0);
  }

  ram0Write();                                                        // use the waiting time to store data to ram0

  do 
    pwr_getSleepTimerStable(WATCHDOG_TIMER, &u8Stable);  
  while (u8Stable == FALSE);                                          // wait till shortterm timer runs stable and all registers are captured
  pwr_timerSleep(u8WakeSrc, NULL);                                    // enable wakeup on WAKE0/1 and sleep
}

// ------------------------------------------------------------------
// Function:    parse_uart
// Parameters:  none
//              
//              
// Returns:     pu16Value   CO2 level 
//              0           in case of timeout (counter based)
// Description: parses the byte stream received from the sensor and
//              extracts CO2 level
//              Important the uart has to be enabled during the STOP 
//              bit of the first space char. 
//
//              sensor sends 
//               Z 00880 ....
//              xxxxxxxxx
//              252333332
//              0A0008800  
// ------------------------------------------------------------------
void parse_uart(uint16 *pu16Value)  {
  uint8 u8State;
  uint8 u8Byte;
  uint8 u8Buf[10];
  uint16 i;
  uint8 j;

  i=0; j=0;
  *pu16Value = 0;
  u8State = 0;      
  do {
    switch(u8State)  {
      case 0:                                                         // find sync
          if(uart_getByte(&u8Byte)==OK)  { 
            u8Buf[j++] = u8Byte;
            if(u8Byte=='Z')  u8State = 1;
          }
          break;
      case 1:                                                         // ignore space
        if(uart_getByte(&u8Byte)==OK)  {
          u8State = 2;
          u8Buf[j++] = u8Byte;
        }
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        if(uart_getByte(&u8Byte)==OK)  {
          *pu16Value = *pu16Value * 10+(u8Byte-'0');                  // error handling???
          u8State++;
          u8Buf[j++] = u8Byte;
        }
        break;
      case 7:
        i = CO2TIMEOUT+1;                                                     // force exit
        continue;
        break;
    }
    if(j>=sizeof(u8Buf)) j = sizeof(u8Buf)-1;

  } while(i++<=CO2TIMEOUT);

  if(i==CO2TIMEOUT)  *pu16Value = 0;                                        // timeout

}
