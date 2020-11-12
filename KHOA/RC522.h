#ifndef _RC522_H_
#define _RC522_H_

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

void RF_init();
String RF_getID() ;
bool RF_matchId(String id);

#endif /* _RC522_H_ */
