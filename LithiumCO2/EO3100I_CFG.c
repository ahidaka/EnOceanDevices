// Generated on 2013-06-07 15:41:51 by DolphinAPIConfigurator 1.1.0.20
#include "EO3100I_API.h"
#include "EO3100I_CFG.h"

//*********************API INIT*************************** 
//Note: Function is called from startup.a51. Global variables are not yet initialized!
void startupInit()
{
	pwr_enableXTAL(DEFAULT_DELAY);
	//io_init(io_param);
}

void mainInit()
{
	radio_init_mod(CFG_ALL);
    radio_init(RADIO_BUFF_NUM, RADIO_MATURITY_TIME);
	UART_INIT(uart_param, SET_ALL_PARAM);
    misc_init();
    pwr_selectXTAL();
}

//*********************UART PARAM***************************
//! UART Rx/Tx buffers allocation
volatile uint8 xdata	u8RxRing[RX_RING_SIZE];
volatile uint8 xdata	u8TxRing[TX_RING_SIZE];

uint8 uart_param[]	= {
 	BAUD_9600,	  //IDX_BAUD 
	RX_RING_SIZE, //IDX_RX_RING_SIZE
	TX_RING_SIZE, //IDX_TX_RING_SIZE
};

//*********************RADIO PARAM***************************
//! Radio buffers allocation
volatile RADIO_BUFFER_TYPE xdata gRadioBuff[RADIO_BUFF_NUM];

//*********************FILTER PARAM***************************
//! Filter buffer allocation for value of filter
volatile uint32 xdata u32gFilterValue[FILTER_NUM];
//! Filter buffer allocation containing FILTER_TYPE (Bit 0..1 and FILTER_KIND Bit 7)
volatile uint8  xdata u8gFilterCfg[FILTER_NUM];

#if 0
//*********************IO PARAM******************************
uint8 code io_param[]  = { 
   0x07,   //IDX_GPIO_CONF
   0x00,   //IDX_GPIO0_CONF
   0x0F,   //IDX_GPIO0_PULL_CONF
   0x00,   //IDX_GPIO0_DIR
   0x07,   //IDX_GPIO1_AN
   0x00,   //IDX_GPIO1_CONF0
   0x00,   //IDX_GPIO1_CONF1
   0x03,   //IDX_GPIO1_DIG_CONF 
   0x80,   //IDX_GPIO1_DIR
   0x7F,   //IDX_GPIO1_PULL
   0x0C,   //IDX_GPIO2_CONF
   0x00,   //IDX_GPIO2_DIR
   0x00,   //IDX_GPIO0
   0x00,   //IDX_GPIO1
   0x00,   //IDX_GPIO2
};
#endif
// I/O Configuration overview
//
//      Pin :  Interface      Direction   Pull   InitValue    Interrupt
// SCSEDIO0 :  Digital I/O    In          Up     0            No     
// SCLKDIO1 :  Digital I/O    In          Up     0            No     
// WSDADIO2 :  Digital I/O    In          Up     0            No     
// RSDADIO3 :  Digital I/O    In          Up     0            No     
// ADIO0    :  Analogue I/O   In          Up     0            No     
// ADIO1    :  Analogue I/O   In          Up     0            No     
// ADIO2    :  Analogue I/O   In          Up     0            No     
// ADIO3    :  Digital I/O    In          Up     0            No     
// ADIO4    :  Digital I/O    In          Up     0            No     
// ADIO5    :  Digital I/O    In          Up     0            No     
// ADIO6    :  Digital I/O    In          Up     0            No     
// ADIO7    :  Digital I/O    Out         None   0            No     
// WXIDIO   :  Digital I/O    In          Up     0            No     
// WXODIO   :  Digital I/O    In          Up     0            No     
// WAKE0    :  Digital I/O    In          None   0            No     
// WAKE1    :  Digital I/O    In          None   0            No     
