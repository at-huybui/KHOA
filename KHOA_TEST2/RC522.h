#ifndef _RC522_H_
#define _RC522_H_

#include <Arduino.h>



#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

void initRFID();
String getID() ;

#endif /* _RC522_H_ */
