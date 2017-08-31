#include "EO3100I_API.h"
#include "LRNtelegram.h"

// ------------------------------------------------------------------
// Function:    SendLrnTelegram
// Parameters:  none
//              
//              
// Returns:     nothing
// Description: sends a 4BS learn telegram with EEP  
//              
// ------------------------------------------------------------------

//! Telegram structure to prepare telegram data for sending
TEL_RADIO_TYPE2		data	rTel2;

void SendLrnTelegram2(void)
{
	rTel2.raw.bytes[0]	=	0x62; 	// Header: Only 48 bit Originator-ID, no extended Header, 4BS telegram
	rTel2.raw.bytes[1]	=	0;		// Id	
	rTel2.raw.bytes[2]	=	0;		// Id	
	rTel2.raw.bytes[3]	=	0;		// Id	
	rTel2.raw.bytes[4]	=	0;		// Id	
	rTel2.raw.bytes[5]	=	0;		// Id	
	rTel2.raw.bytes[6]	=	0;		// Id	
	rTel2.raw.bytes[7]	=	((FUNC&0x3F)<<2) | ((TYPE&0x60)>>5);	
	rTel2.raw.bytes[8]	=	((TYPE&0x1F)<<3) | ((MANUFACTURER&0x300)>>8);	
	rTel2.raw.bytes[9]	=	(MANUFACTURER&0xFF);	
	rTel2.raw.bytes[10]	=	0x80; //(unigactualData.u32DefaultRes.u8DigitalData | MASK_LRN_TYPE)& ~MASK_DI_3;	
	rTel2.raw.u8Length	= 	12;

	//io_setDigital(ADIO_3,(bit1)1); // switch LED on for indicating sending
	radio_sendTelegram2(&rTel2, NULL);
	//io_setDigital(ADIO_3,(bit1)0); // switch LED on for indicating sending
}
