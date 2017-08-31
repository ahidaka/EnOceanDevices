//! \file DolphinSTM.h
//! \brief
//! Include file of DolphinSTM software

#ifndef _DolphinSTM_H_INCLUDED
#define _DolphinSTM_H_INCLUDED
 
/****************************Defines*************************************/

//! Value of erased flash 8 bit value
#define FLASH_ERASED_8			((uint8)0xff)
//! Value of erased flash 16 bit value
#define FLASH_ERASED_16			((uint16)0xffff)
//! Value of erased flash 32 bit value
#define FLASH_ERASED_32			((uint32)0xffffffff)
//! Value in CFG edge configuration for transmit on rising edge
#define EDGE_WAKE0_RISING_ONLY	((uint8)1)
//! Value in CFG edge configuration for transmit on falling edge
#define EDGE_WAKE0_FALLING_ONLY	((uint8)2)
//! Value in CFG edge configuration for transmit on rising edge
#define EDGE_WAKE1_RISING_ONLY	((uint8)4)
//! Value in CFG edge configuration for transmit on falling edge
#define EDGE_WAKE1_FALLING_ONLY	((uint8)8)
//! Wakeup-Cycle for NEVER
#define NO_WAKEUP				((uint8)0)
//! Value in RAM0 watchdog cell for watchdog from cfg
#define WATCHDOG_CFG			((uint8)4)
//! Mask in RAM0 watchdog cell for watchdog from HW
#define WATCHDOG_HW_CFG			((uint8)3)
//! Mask in RAM0 redundand retransmission from HW
#define REDUND_RETRANS_HW_CFG	((uint8)0x0C)
//! Mask of DI_3 indicating learn or data
#define MASK_DI_3				((uint8)0x08)
//! Mask of LRN-Type-Bit in Teach-In-Telegram
#define MASK_LRN_TYPE			((uint8)0x80)
//! Mask of digital measurement values
#define DIGITAL_DATA_MASK 		((uint8)0x0F)
//! Default thresholds of AD_0
#define DEFAULT_THRESHOLD_AD_0	((uint8)6)
//! Default thresholds of AD_1
#define DEFAULT_THRESHOLD_AD_1	((uint8)5)
//! Default thresholds of AD_2
#define DEFAULT_THRESHOLD_AD_2	((uint8)14)
//! Mask for CW input pin combination NC NC of GPIO0
#define MASK_CW_NC_NC			((uint8)0x03)
//! Mask for CW input pin combination NC GND of GPIO0
#define MASK_CW_NC_GND			((uint8)0x02)
//! Mask for CW input pin combination GND NC of GPIO0
#define MASK_CW_GND_NC			((uint8)0x01)
//! Mask for CW input pin combination GND GND of GPIO0
#define MASK_CW_GND_GND			((uint8)0x00)
//! Wakeup cycle when CW combination NC NC [sec]
#define CYC_CW_NC_NC			((uint8)1)
//! Wakeup cycle when CW combination NC GND	[sec]
#define CYC_CW_NC_GND			((uint8)100)
//! Wakeup cycle when CW combination GND NC	[sec]
#define CYC_CW_GND_NC			((uint8)10)
//! Value for no wakeup when CW combination GND GND
#define CYC_CW_GND_GND			((uint8)0)
//! Mask for CP input pin combination NC NC of GPIO0
#define MASK_CP_NC_NC			((uint8)0x0C)
//! Mask for CP input pin combination NC GND of GPIO0
#define MASK_CP_NC_GND			((uint8)0x08)
//! Mask for CP input pin combination GND NC of GPIO0
#define MASK_CP_GND_NC			((uint8)0x04)
//! Mask for CP input pin combination GND GND of GPIO0
#define MASK_CP_GND_GND			((uint8)0x00)
//! Minimum value of retransmission wakeups when CP combination NC NC
#define T_CYC_MIN_CP_NC_NC		((uint8)1)
//! Maximum value of retransmission wakeups when CP combination NC NC
#define T_CYC_MAX_CP_NC_NC		((uint8)1)
//! Minimum value of retransmission wakeups when CP combination NC GND
#define T_CYC_MIN_CP_NC_GND		((uint8)70)
//! Maximum value of retransmission wakeups when CP combination NC GND
#define T_CYC_MAX_CP_NC_GND		((uint8)140)
//! Minimum value of retransmission wakeups when CP combination GND NC
#define T_CYC_MIN_CP_GND_NC		((uint8)7)
//! Maximum value of retransmission wakeups when CP combination GND NC
#define T_CYC_MAX_CP_GND_NC		((uint8)14)
//! Value for no retransmission when CP combination GND GND
#define T_CYC_MIN_CP_GND_GND	((uint8)0)
//! Value for no retransmission when CP combination GND GND
#define T_CYC_MAX_CP_GND_GND	((uint8)0)
//! Macro for switching external sensor hardware ON
#define SCO_ON if (unigcfg.u8SCO_Polarity == FLASH_ERASED_8) {io_setDigital(WXID_IO, (bit1)1);} else {io_setDigital(WXID_IO, (bit1)0);}
//! Macro for switching external sensor hardware ON
#define SCO_OFF if (unigcfg.u8SCO_Polarity == FLASH_ERASED_8) {io_setDigital(WXID_IO, (bit1)0);} else {io_setDigital(WXID_IO, (bit1)1);}
//! Amount of shortterm clock ticks in 	1ms. 1 ST clock period = 10µs
#define ST_CLKS_PER_1MS            		((uint16)100)
//! Antibounce time [1ms] with shorterm calibration correction calculation
#define SUBTEL_PAUSE(u8Time)			( (uint16)(  ((uint32)(u8Time) * ST_CLKS_PER_1MS          * gModArea.val.u16CalibShortTerm) / SHORT_TERM_CALIB_CONST) )

/****************************Typedefs*************************************/

//! \struct DolphinSTM_CFG
//! \brief Structure of software configuration bytes in application section of CFG area\n
//! Located in CFG-Area starting address 0x00
typedef struct
{
	//! Resolution of AD inputs \n
	//! Located in CFG-Area 0x00 \n
	//! Default 0xff: 8/8/8 Bit, all other values: 10/6/8 Bit
	uint8	u8ADResolution;
	//! Threshold of AD_0 for significant change \n
	//! Located in CFG-Area 0x01 \n
	//! Default 0x00: 6 digits, all other values are the number of digits (max. 254)
	//! 0xff: Ignore any change
	uint8	u8AD_0_Threshold;
	//! Threshold of AD_1 for significant change	\n
	//! Located in CFG-Area 0x02 \n
	//! Default 0x00: 5 digits, all other values are the number of digits (max. 254)
	//! 0xff: Ignore any change
	uint8	u8AD_1_Threshold;
	//! Threshold of AD_2 for significant change	\n
	//! Located in CFG-Area 0x03 \n
	//! Default 0x00: 14 digits, all other values are the number of digits (max. 254)
	//! 0xff: Ignore any change
	uint8	u8AD_2_Threshold;
	//! Digital mask to unmask digital input pins D0, D1 and D2 from significant change \n
	//! Located in CFG-Area 0x04 \n
	//! Default 0xff: all bits register changes, 0bxxxxx000...0bxxxxx111 = Bitmask    Bit=0: ignore changes, Bit=1: register changes
	uint8	u8DigMask;
	//! Minimum cycle number of redundant retransmissions \n
	//! Located in CFG-Area 0x05 \n
	//! Default 0xff: use HW-Settings, 0x00: no redundant retransmission, 1...254 minimum cycle numbers
	uint8	u8TCycleMin;
	//! Maximum cycle number of redundant retransmissions \n
	//! Located in CFG-Area 0x06 \n
	//! 0x00: no redundant retransmission, 1...255 maximum cycle numbers
	uint8	u8TCycleMax;
	//! Edge of wake pins to perform actions\n
	//! Located in CFG-Area 0x07 \n
	//! Default 0xf7: Wake0 both edges, Wake1 on falling edge \n
	//! Bit0 and Bit1 configures Wake0
	//! Bit2 and Bit3 configures Wake1
	//! 01: Only rising edge, 10: Only falling edge, 11: Both edges, 00: Ignore wake pin
	uint8	u8TEdge;
	//! Wakeup cycle \n
	//! Located in CFG-Area 0x08 \n
	//! Default 0xffff: use HW-Settings, 0: No wakeup, 1...65535: wakeup cycle [sec]
	uint16	u16WakeCycle;
	//! Enable saving of logging information in application flash area \n
	//! Located in CFG-Area 0x0A \n
	//! Default 0xff: saving disabled, all other values: saving enabled
	uint8	u8EnLogSave;
	//! 3 bytes of device data, containing profile, type and manufacturer id \n
	//! Located in CFG-Area 0x0B...0x0D	\n
	//! Default manufacturer ID 0x00: do not send a teach-in telegram
	uint8	u8DeviceData[3];
	//! Reserved for future use \n
	//! Located in CFG-Area 0x8E \n
	uint8	u8Reserved;
	//! SCO pin active period for switching on external sensor hardware \n
	//! Located in CFG-Area 0x0F \n
	//! Default 0xff: 2ms, 0: No wait, 1...254: wait time [n*2ms]
	uint8	u8SCO_Active_Time;
	//! Mode of AD_2 pin
	//! Located in CFG-Area 0x10 \n
	//! Default 0xff: Measure AD_2 as input; all other values: Measure Quarter of Unregulated supply voltage input internaly and put the value in telegram instead of AD_2 input.
	uint8	u8AD_2_Mode;
	//! Polarity of SCO pin (sensor control)
	//! Located in CFG-Area 0x11 \n
	//! Default 0xff: Active high; all other values: Active low
	uint8	u8SCO_Polarity;	
} DolphinSTM_CFG;

//! \struct DEFAULT_RESOLUTION
//! \brief Structure containing measurement values using the default resolution of analog data of 8 bits
typedef struct
{
	//! Analog measurement value of AD_2
	uint8	u8AD_2;
	//! Analog measurement value of AD_1
	uint8	u8AD_1;
	//! Analog measurement value of AD_0
	uint8	u8AD_0;
	//! Digital measurement values of DI_0...DI_3
	uint8	u8DigitalData;
} DEFAULT_RESOLUTION;

//! \struct OPTIONAL_RESOLUTION
//! \brief Structure containing measurement values using the optional resolution of analog data of 10/6/8 bits
typedef struct
{
	//! Analog measurement value of AD_2
	uint8	u8AD_2;
	//! Analog measurement value of AD_0
	uint16	u10AD_0 : 10;
	//! Analog measurement value of AD_1
	uint16	u6AD_1 : 6;
	//! Digital measurement values of DI_0...DI_3
	uint8	u8DigitalData;
} OPTIONAL_RESOLUTION;

//! \union MEASUREMENT_DATA
//! \brief Union containing measurement values overlaying the default and optional resolution of analog data 
typedef union 
{
	//! Default resolution of measurement values
	DEFAULT_RESOLUTION  u32DefaultRes;
	//! Optional resolution of measurement values
	OPTIONAL_RESOLUTION u32OptionalRes;
} MEASUREMENT_DATA;

//! \enum WAKEUP_TYPE
//! \brief Enumeration of STM300 relevant wakeup types
typedef enum
{
	//! Wakeup by watchdog, power on or reset
	WAKEUP_WATCHDOG,
	//! Wakeup by Wake0 pin
	WAKEUP_WAKE0,
	//! Wakeup by Wake1 pin
	WAKEUP_WAKE1,
	//! Wrong edge of wake pins
	WAKEUP_WRONGEDGE
} WAKEUP_TYPE;

//! \enum TELEGRAMTYPE
//! \brief Enumeration of telegram types depending on wakeup type and retransmission counter
typedef enum 
{
	//! Send a data telegram
	DATATELEGRAM = 0,
	//! Send a teach in telegram
	TEACHINTELEGRAM,
	//! Do not send any telegram
	NOTELEGRAM
} TELEGRAMTYPE;

//! \struct RAM0_SHADOW
//! \brief Structure of shadow of RAM0 contents
typedef struct 
{	
	//! last watchdog cycle (0..3 = HW-Cfg, 4 = Flash-Cfg)
	uint8	u8Watchdog;	
	//! last configured retransmission (0..3)		
	uint8	u8Retransmission;	
	//! Counter of wakeups since last transmission
	uint8	u8RetransCtr;
	//! Random value of wakeups till next redundand retransmission		
	uint8	u8RetransRnd;	
	//! last measured digital and analog data
	MEASUREMENT_DATA u32Data;   
} RAM0_SHADOW;


/****************************Variables***********************************/

//! Structure for access to configuration data in config area
extern DolphinSTM_CFG 	code 	unigcfg;
//! Structure storing data of ram0 as a shadow
extern RAM0_SHADOW 		data 	sctgram0;
//! Actual measured data
extern MEASUREMENT_DATA data 	unigactualData;
//! The wakeup reason of last wakeup from deep sleep
extern WAKEUP_TYPE 		data 	enugwakeupReason;
//! Telegram structure to prepare telegram data for sending
extern TEL_RADIO_TYPE 	data	sctgrTel;
//! Variable to indicate a wake0 interrupt, will be initialized before use
extern 					bit 	bgwake0Interrupt;
//! Internal measurement values
extern sint16 s16gref; // will be used also for random value generation


/****************************Functions**********************************/


//module main.c

/************************************************************************************************************************//*!
\brief
This is the main function of the STM300 software.
               
\param  	none
\return 	none 	

\note
Here is a activity diagram of the main function: \n

\image html "STM300 Flow Chart.jpg"
**************************************************************************************************************************/
void main(void);

/************************************************************************************************************************//*!
The module will wake up from deepsleep by the wake pins or watchdog timer.

\brief
The function forces the STM300 module to go to the deepsleep mode.
                
\param  	none
\return 	none 	
**************************************************************************************************************************/
void deepsleep(void);

// module init.c

/************************************************************************************************************************//*!
The function initializes the Dolphin chip hardware and global variables. It reads the contents
of the RAM0 variables.

\brief
Initialisation of the Dolphin chip
                
\param  	none
\return 	none 	
**************************************************************************************************************************/
void init(void);

/************************************************************************************************************************//*!
The function initializes the flash page which is reserved to contain the calibrated short term sleep periods for 40 ms

\brief
Initialisation of the short term periods flash page
                
\param  	none
\return 	none 	
**************************************************************************************************************************/
void initShortTermFlash(void);

/************************************************************************************************************************//*!
The function initializes the flash page which is reserved to contain the calibrated short term sleep periods for 40 ms

\brief
Initialisation of the short term periods flash page
                
\param  	none
\return 	none 	
**************************************************************************************************************************/
void saveLog(void);

/************************************************************************************************************************//*!
Saves the log information to flash memory
        
\brief
Saves the log information to flash memory
        
\param  	none
\return 	none 	
**************************************************************************************************************************/
void powerUpInitialization(void);

// modules radio_init_asm_xxx.a51
/************************************************************************************************************************//*!
The function initialises the radio hardware with specific configuration. It was generated by Dolphin Studio.
        
\brief
Radio initialisation
        
\param  	u8PowerUp	Dummy value, will not be checked
\return 	none 	

\note
Dependent of the project target, the files radio_init_asm_868.a51 (for 868 Mhz) or
radio_init_asm_315.a51 (for 315 Mhz) will be linked.
**************************************************************************************************************************/
void radio_init_asm(uint8 u8PowerUp); //lint !e762

// module wakeup.c

/************************************************************************************************************************//*!
Sets the callback address of wake0 callback function. When wake0 pin changes, an interrupt
will be triggered, which calls the callback function #callBackWake0.
        
\brief
Sets the callback address of wake0 callback function #callBackWake0
        
\param  	none
\return 	none

\note
This functionality is for industrial applications, where it is possible that the wake0 pin
changes inside 7 ms and should retrigger measurement and telegram sending.	
**************************************************************************************************************************/
void setWake0Callback(void);

/************************************************************************************************************************//*!
When wake0 pin changes an interrupt will be triggered and calls this function.\n
It checks, if the configured edge is correct, and if yes, a flag will be set. The actual telegram
transmission sequence will be interrupted (the actual sub telegram will be send completely) and a
new measurement will be done.
        
\brief
Callback function of wake0 pin interrupt
        
\param  	none
\return 	none
**************************************************************************************************************************/
void callBackWake0(void);

/************************************************************************************************************************//*!
Checks the edge of the changed wake pin with the configured (or default) edge and returns
whether an action is necessary.
        
\brief
Checks, if the edge of the changed wake pin should lead to any action
        
\param  	u8WakePin	WAKE0_PIN_CFG_WAKE_FLAG or WAKE1_PIN_CFG_WAKE_FLAG
\return 	FALSE: edge is wrong, no action necessary
			TRUE:  edge is valid, action necessary
**************************************************************************************************************************/
uint8 checkEdge(WAKE_CFG_TYPE u8WakePin);

/************************************************************************************************************************//*!
Reduces all possible wake types to STM300 reduced wakeup possibilities.
        
\brief
Calculates a wakeup type with STM300 reduced wakeup possibilities
        
\param  	none
\return 	changes global variable enugwakeupReason

\note
Here is the activity diagram of this function: \n

\image html "Determine wakeup reason.jpg"
**************************************************************************************************************************/
void determineWakeupReason(void);

/************************************************************************************************************************//*!
If the wakeup cycle is different to the last wakeup it updates the watchdog timer.
        
\brief
Checks the wakeup cycle
        
\param  	none
\return 	FALSE: watchdog wakeup is necessary, watchdog timer is adjusted correctly
			TRUE:  no wakeup is configured

\note
Here is the activity diagram of this function: \n

\image html "Update wakeup-cycle.jpg"
**************************************************************************************************************************/
bit wakeup_cycle(void);

// module retransmission.c

/************************************************************************************************************************//*!
When wakeup counter reaches the configured or random value, then a redundant retransmission of the actual
measurement values has to be performed. This function checks, if this has to be done, and calculates
a new random value for next retransmission.
        
\brief
Checks if a redundant retransmission has to be done
        
\param  	none
\return 	FALSE: no redundant retransmission to do \n
			TRUE:  now a redundant retransmission has to be done
**************************************************************************************************************************/
uint8 redundant_retransmission(void);

/************************************************************************************************************************//*!
A random value will be calculated
        
\brief
Calculates a random value within the given minimum and maximum values
        
\param  	u8min	Minimum value of random number
\param 		u8max	Maximum value of random number
\return 	random number, which is inside the given borders
**************************************************************************************************************************/
uint8 getRandom(uint8 u8min, uint8 u8max);

// module sending.c

/************************************************************************************************************************//*!
Depending of wake type and stored manufacturer code this functions determines the telegram type to send.        

\brief
Determines, what telegram should be sent
        
\param  	none
\return 	#TELEGRAMTYPE

\note
Here is a activity diagram of this function: \n

\image html "Determine Telegram Type.jpg"
**************************************************************************************************************************/
TELEGRAMTYPE DetermineTelegramType(void);

/************************************************************************************************************************//*!
This function checks, if a manufacturer code is stored

\brief
Check of manufacturer code
        
\param  	none
\return 	FALSE: no manufacturer code in CFG area stored \n
			TRUE:  a manufacturer code is in CFG area stored
**************************************************************************************************************************/
uint8 isManufacturerCode(void);

/************************************************************************************************************************//*!
This function sends the computed telegram to the air. It performs a short term sleep between
the 1st and 2nd sub telegram and makes all the necessary initialisation of hardware dependent
blocks.
If wake0 pin is changed while sending the sub telegrams or while shortterm sleep, the function
can return, when input parameter bBreakable is set. When using teach in telegram, then the parameter
is not set and the 3 sub telegrams will be sent completely.

\brief
Sends the complete telegram with short term sleeps between the 3 sub telegrams
        
\param  	bBreakable	TRUE: telegram can be aborted, FALSE: telegram cannot be aborted
\return 	none
**************************************************************************************************************************/
//void sendMyTelegram(bit bBreakable);

/************************************************************************************************************************//*!
This function sends a TeachIn telegram using #sendMyTelegram function.

\brief
Prepares a TeachIn telegram and sends it
        
\param  	none
\return 	none
**************************************************************************************************************************/
void sendTeachInTelegram(void);

/************************************************************************************************************************//*!
This function sends a Data telegram using #sendMyTelegram function.

\brief
Prepares a Data telegram and sends it
        
\param  	none
\return 	none
**************************************************************************************************************************/
void sendDataTelegram(void);

// module measurement.c

/************************************************************************************************************************//*!
This function makes the measurement of the analog input pins with configured resolution.

\brief
Measurement of the analog inputs
        
\param  	none
\return 	none
**************************************************************************************************************************/
void MeasureAnalog(void);

/************************************************************************************************************************//*!
This function makes the measurement of the digital input pins.
\brief
Measurement of the digital inputs
        
\param  	none
\return 	none
**************************************************************************************************************************/
void MeasureDigital(void);

/************************************************************************************************************************//*!
This function checks if the digital and analog input pins have a significant change since the
last sent data telegram. The values of a significant change can be adjusted in CFG area, if the
default values do not fit the needs. There is also the possibilty to mask digital input pins from
checking.

\brief
Checks a significant change of the measurement values
        
\param  	none
\return 	none
**************************************************************************************************************************/
uint8 isSignificantChange(void);

/************************************************************************************************************************//*!
This function waits the configured time before measurement to be sure, that the external sensor HW is ready.

\brief
Waits for configured time to the external sensor HW to be ready
        
\param  	none
\return 	none
**************************************************************************************************************************/
void Wait4ExternalSensorHW(void);

#endif
