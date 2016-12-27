/*********** GATEWAY MESSAGE STRUCTURE *******************
Gateways structure is a simplified, abstracted, message 
type used to relay messages between host computer and 
wireless sensor network. 

Gateway message format 
HEADER
	- MESSAGE PAYLOAD

Function:
Gateway relays messages by inspecting only the message 
header. Payload forwarded to target node intact...

MOTEINO MESSAGE FORMAT
----------------------------------

MAX_MESSAGE_SIZE = 66
MESSAGE FRAME SIZES
------------------------
PAYLOAD = 1
ADDRESS_BYTE = 1
FROM_ADDRESS = 1
CONTROL_BYTE = 1
NETWORK_ID = 1
MAX_PAYLOAD = 66 - 5 = 61 BYTES!!!
_____________________________________
Moteino References 
https://github.com/LowPowerLab/RFM69
http://lowpowerlab.com/blog/2013/06/20/rfm69-library/#more-917

Developer Notes
1. Payload is not inspected by the gateway. 
2. Header inspected by Gateway, used to route message
3. Target node is responsible for processing payload.
4. ASSUMPTION = AVR(Big Endian) RPI(Little Endian)
5. NodeID + MsgID used together to create unique index for xmit.
*/

#ifndef GatewayMsg_h
#define GatewayMsg_h

#if (ARDUINO < 100)
#include "WProgram.h"
#else
#include <Arduino.h>
#endif
#define FRAME_BUFFER_SIZE 5
#define MAX_SERIAL_SIZE 70 
#define MAX_NETWORK_SIZE 61
#define WATCHDOG_DEFAULT 10000 // 10 seconds used by node
#define SERIAL_HEADER_SIZE 3 // number of bytes in the header
/*---------------------------------------------
|	!! PKG Types !!
|	Structure written to pkg[] array...
|
*/


// Gateway Message Structure 
typedef struct {
  byte 				NodeID; // Address of target sensor (00 for echo to gateway)
  byte 				SerialPayloadSize; // Length structure to read
  byte  			SerialPayload[MAX_NETWORK_SIZE];
} SerialMsg;
SerialMsg mMsg;	// serial Message
SerialMsg nrMsg;	// node Message (rfm - radio)

typedef struct {
  	byte 			MsgType; 	//[1] Message type - Struct Decode (10)
  	byte 			msg[59]; 	// Allocate remaining msg space for possible payload. Payload is node specific
} Payload;
Payload payload;



#endif