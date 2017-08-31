// Generated on 2013-06-07 15:41:32 by DolphinAPIConfigurator 1.1.0.20
#ifndef _EO3100I_CFG_H_INCLUDED
#define _EO3100I_CFG_H_INCLUDED

//! Function Prototypes
void startupInit();
void mainInit();

//*********************UART PARAM***************************
//! UART Rx/Tx buffers allocation
#define RX_RING_SIZE 34
#define TX_RING_SIZE 34
extern volatile uint8 xdata u8RxRing[RX_RING_SIZE];
extern volatile uint8 xdata u8TxRing[TX_RING_SIZE];

extern uint8 uart_param[];

//*********************RADIO PARAM***************************
#define RADIO_BUFF_NUM 10
#define RADIO_MATURITY_TIME 100

//! Radio buffers allocation
extern volatile RADIO_BUFFER_TYPE xdata gRadioBuff[RADIO_BUFF_NUM];

//*********************FILTER PARAM***************************
//! Max. number of filters
#define FILTER_NUM	1
//! Filter buffer allocation for value of filter
extern volatile uint32 xdata u32gFilterValue[FILTER_NUM];
//! Filter buffer allocation containing FILTER_TYPE (Bit 0..1 and FILTER_KIND Bit 7)
extern volatile uint8  xdata u8gFilterCfg[FILTER_NUM];

//*********************IO PARAM******************************
//extern uint8 code io_param[];

#endif //_EO3100I_CFG_H_INCLUDED
