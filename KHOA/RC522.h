#ifndef _RC522_H_
#define _RC522_H_

#include "DEFINE.h"

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

void RF_init();
String RF_getID() ;
bool RF_matchId(String id);

#endif /* _RC522_H_ */
