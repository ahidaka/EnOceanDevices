#ifndef _LRNTELEGRAM_H_
#define _LRNTELEGRAM_H_

#include "EO3100I_API.h"

// ------------------------------------------------------------------
// EnOcean Equipment Profiles
#define RORG              0xA5                                        // 4BS
#define FUNC              0x09                                        // gas sensor        
#define TYPE              0x04                                        // CO2
#define MANUFACTURER      0x000B                                      // EnOcean

// ------------------------------------------------------------------
// EEP: A5-09-04  
// offset             1          2          3
//         01234567 89012345 67890123 45678901
//         HHHHHHHH                             rel. humidity            0..200 -> 0..100%
//                  CCCCCCCC                    CO2                      0..255 -> 0..2550ppm
//                           TTTTTTTT           temp                     0..255 -> 0..+51.0°C
//                                        L     Lrn bit 
//                                         h    rel. humidity available 
//                                          t   temperature available 
// ------------------------------------------------------------
 
// ------------------------------------------------------------------
// public funcions    
void SendLrnTelegram(void);
void SendLrnTelegram2(void);


#endif  //_LRNTELEGRAM_H_
