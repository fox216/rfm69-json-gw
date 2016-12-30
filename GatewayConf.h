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
#define NETWORKID   60 // 60 for testing... change to production (100)
#define FREQUENCY   RF69_915MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define KEY         "thisIsEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         9
#define SERIAL_BAUD 115200
#define SERIAL_BUFFER_SZ 200
#define	JSON_BUFFER_SZ 200

// 4. Payload Data Types
// byte          = 10
// char          = 11
// unsigned char = 12
// int           = 20
// unsigned int  = 21
// word          = 22
// long          = 40
// unsigned long = 41
// float         = 42
// double        = 43
// Update for marshalling data in array. Encoding types as bytes
// Serialized exmaple removing map declaration
// echo '{"node":"14","meth":2,"type":6,"data":[10,2,41,250445,42,321.45,42,88.45]}' > /dev/ttyLPL 

typedef union {
	unsigned long 	L;
	long			l;
	float			f;
	double			d;
	unsigned int	I;
	int				i;
	word			w;
	byte 	B[4];
	byte 	b[2];
} _ByteConvert;
_ByteConvert ByteConvert;

#endif