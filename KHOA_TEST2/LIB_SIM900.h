#ifndef _LIB_SIM900A_H_
#define _LIB_SIM900A_H_

#include <Arduino.h>

bool Sim900_init();
bool Sim900_sendSMS(char *text);
String Sim900_getSMS();


#endif /* _LIB_SIM900A_H_ */
