// !!!!  JSON GATEWAY CONFIGURATION SETTINGS  !!!!
#ifndef GatewayConf_h
#define GatewayConf_h

#if (ARDUINO < 100)
#include "WProgram.h"
#else
#include <Arduino.h>
#endif

/*---------------------------------------------
|
|	Gateway Configuration Settings
|	
*/

#define NODEID      1
#define NETWORKID   50 // 50 for testing... change to production (100)
#define FREQUENCY   RF69_915MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define KEY         "thisIsEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         9
#define SERIAL_BAUD 115200
#define ACK_TIME    30  // # of ms to wait for an ack
#define WAIT_TIME   1000
#define PASS_LED 17
#define	FAIL_LED 15
#define SERIAL_BUFFER_SZ 200
#define	JSON_BUFFER_SZ 200


#endif