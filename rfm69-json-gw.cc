//avr-json Proof of Concept tesing using ArduinoJson for Wireless Sensor Network Comminication
/*
Change Log
2016-12-23: Test JSON Output
2016-12-24: Add Serial input

*/
#include <ArduinoJson.h>
#include <RFM69.h>
#include <SPI.h>
// #include <GatewayMsg.h> // Load Gateway Message structure:Task
#include <NodeMsg.h>
#include <GatewayConf.h>
// Initialize Radio
#define DEBUG_SERIAL

RFM69 radio;

// Note:
// SERIAL_BUFFER_SZ & JSON_BUFFER_SZ defined in GatewayConf.h

char SerialBuffer[SERIAL_BUFFER_SZ];
byte MsgPayloadBuffer[MAX_PAYLOAD_SIZE];
int MsgPayloadSz = 0;
byte payloadBuffer[MAX_PAYLOAD_SIZE];

void setup() {
	// Setup Serial port
	Serial.begin(SERIAL_BAUD);
	// Wait for serial to start
	while( ! Serial ) {}
	// //Setup RFM69 Radio
	radio.initialize(FREQUENCY, NODEID, NETWORKID);
	// // Wait for Radio to start
	// //while(! radio) {} // Unknown 
	// // Setup RFM69 Encryption
	radio.encrypt(KEY);
}

void Blink(byte PIN, int DELAY_MS) {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

void loop() {
  	if ( Serial.available() > 0 ) {
  		// Process incoming serial data
  		// Parse json and send to target node
		Serial.readBytesUntil('\n', SerialBuffer, SERIAL_BUFFER_SZ -1 );
		StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		JsonObject& json_in = jsonBuffer.parseObject(SerialBuffer);
		// Raise error if data received can not be parsed into json.
		if (!json_in.success()) {
    		Serial.println("parseObject() failed");
    		return;
  		}

  		// Encode data from serial port to an RFM69 message
  		/*
		{
			"node":123,
			"meth":2,
			"type":200,
			"map":"bbLfiIL",
			"data":[42,43,999999,175.06,-678,48197,48197000]
			}
			// Sample test
			echo '{"node":"14","meth":2,"type":6,"map":"bLff","data":[2,250445,321.45,88.45]}' > /dev/ttyLPL

  		*/

  		nodeMsg.NodeID = json_in["node"];
  		nodeMsg.Method = json_in["meth"];
  		nodeMsg.TypeID = json_in["type"];
  		nodeMsg.PayloadDataMap = json_in["map"];

  		#ifdef DEBUG
		Serial.print("Node: ");
		Serial.println(nodeMsg.NodeID);
		Serial.print("Method: ");
		Serial.println(nodeMsg.Method);
		Serial.print("Type: ");
		Serial.println(nodeMsg.TypeID);
		Serial.print("Map: ");
		Serial.println(nodeMsg.PayloadDataMap);
		#endif
		// Decode data from json data array 
		// MsgPayloadSz = sizeof(json_in["data"]);
		// Serial.print("Json Payload Size: ");
		// Serial.println(MsgPayloadSz);
		int msgPayloadOffset = 0;
		byte outBuffer[MAX_PAYLOAD_SIZE];
		// read json metadata (PayloadDataMap) write objects to payload byte array
		for (int x = 0; x < strlen(nodeMsg.PayloadDataMap); x++) {
			char thisDataMap = nodeMsg.PayloadDataMap[x];
  			#ifdef DEBUG
				Serial.print("Map Inspect -> ");
				Serial.println(thisDataMap);
			#endif
			if ( thisDataMap == 'b') {
  				#ifdef DEBUG
					Serial.print("Byte Data: ");
					Serial.println((byte)json_in["data"][x]);
				#endif
				nodeMsg.MsgPayload[msgPayloadOffset] = (byte)json_in["data"][x];
				msgPayloadOffset += 1;
			} 
			else if (thisDataMap == 'f' or thisDataMap == 'L' or thisDataMap == 'l' or thisDataMap == 'd') {
				if ( thisDataMap == 'L') {
					#ifdef DEBUG
					Serial.print("uLong Data: ");
					Serial.println((unsigned long)json_in["data"][x]);
					#endif
					ByteConvert.L = (unsigned long)json_in["data"][x];
				} else if ( thisDataMap == 'l') {
					#ifdef DEBUG
						Serial.print("Long Data: ");
						Serial.println((long)json_in["data"][x]);
					#endif
					ByteConvert.l = (long)json_in["data"][x];
				} else if ( thisDataMap == 'f') {
					#ifdef DEBUG
						Serial.print("Float Data: ");
						Serial.println((float)json_in["data"][x]);
					#endif
					ByteConvert.f = (float)json_in["data"][x];
				} else if ( thisDataMap == 'd') {
					#ifdef DEBUG
						Serial.print("Double Data: ");
						Serial.println((double)json_in["data"][x]);
					#endif
					ByteConvert.d = (double)json_in["data"][x];
				}
				// Write data to payload
				for (int x = 0; x < 4; x++) {
					nodeMsg.MsgPayload[msgPayloadOffset +x] = ByteConvert.B[x];
				} 		
				msgPayloadOffset += 4;	
			} else if (thisDataMap == 'i' or thisDataMap == 'I' or thisDataMap == 'w') {
				if ( thisDataMap == 'i') {
					#ifdef DEBUG
						Serial.print("int Data: ");
						Serial.println((int)json_in["data"][x]);
					#endif
					ByteConvert.i = (int)json_in["data"][x];
					//nodeMsg.MsgPayload[msgPayloadOffset] = (int)json_in["data"][x];
				} else if (thisDataMap == 'I') {
					#ifdef DEBUG
						Serial.print("uInt Data: ");
						Serial.println((unsigned int)json_in["data"][x]);
					#endif
					ByteConvert.I = (unsigned int)json_in["data"][x];
					// nodeMsg.MsgPayload[msgPayloadOffset] = (unsigned int)json_in["data"][x];					
				} else if (thisDataMap == 'w') {
					#ifdef DEBUG
						Serial.print("Word Data: ");
						Serial.println((word)json_in["data"][x]);
					#endif
					ByteConvert.w = (word)json_in["data"][x];
					// nodeMsg.MsgPayload[msgPayloadOffset] = (word)json_in["data"][x];					
				}
				for (int x = 0; x < 2; x++) {
					nodeMsg.MsgPayload[msgPayloadOffset +x] = ByteConvert.b[x];
				} 
				msgPayloadOffset +=2;
			}
		}
		// Send data to node

		radio.send(nodeMsg.NodeID, (const void*)(&nodeMsg), MAX_NETWORK_SIZE);
		Blink(LED, 10);
		delay(100);
		Blink(LED, 10);

		// (DEBUG_SERIAL) Print input received from serial port 
		#ifdef DEBUG_SERIAL
		json_in.printTo(Serial);
		Serial.println();
		delay(10);
		#endif
  	} else {
  		// Process incoming RFM69 data
  		// Parse RFM69 Message and send to Serial
  		StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		JsonObject& json_out = jsonBuffer.createObject();
		if ( radio.receiveDone() ) {
			// Data received from radio
			// Process date / forward to serial port
			// Copy contents of message into _NodeMsg structure (See NodeMsg.h)
			nodeMsg = *(_NodeMsg*)radio.DATA;

			// Clear / Create JSON parsing buffer for output
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& json_out = jsonBuffer.createObject();

			// Encode Message content tp JSON
			json_out["node"] = nodeMsg.NodeID;
			json_out["meth"] = nodeMsg.Method;
			json_out["type"] = nodeMsg.TypeID;
			json_out["map"] = nodeMsg.PayloadDataMap;

			// Copy contents of node msg into array for processing. 
			// Note this step could be skipped to save memory later...
			memcpy(payloadBuffer, &nodeMsg.MsgPayload, MAX_PAYLOAD_SIZE);
			
			// Debug statements
			#ifdef DEBUG
			Serial.print("Data Map Size: ");
			Serial.println( strlen(nodeMsg.PayloadDataMap) );
			Serial.print("Data Map Value: ");
			Serial.println( nodeMsg.PayloadDataMap );
			#endif
			
			// payloadBuffer (MsgPayload) array position pointer///
			int bufferPosition = 0;

			// Create json data array for storing decoded contents of payload
			JsonArray& data = json_out.createNestedArray("data");
			for (int x = 0; x < strlen(nodeMsg.PayloadDataMap); x++) {
				#ifdef DEBUG
				Serial.print("DataMap[");
				Serial.print(x);
				Serial.print("]: ");
				Serial.println(nodeMsg.DataMap[x]);
				Serial.print("BufferPosition: ");
				Serial.println(bufferPosition);
				Serial.println(payloadBuffer[bufferPosition], HEX);
				#endif
				
				// Store current mapping character
				char thisDataMap = nodeMsg.PayloadDataMap[x];

				if (thisDataMap == 'b') {
					data.add((byte)payloadBuffer[bufferPosition]);
					bufferPosition += 1;
				}
				else if (thisDataMap == 'f' or thisDataMap == 'L' or thisDataMap == 'l' or thisDataMap == 'd') {
					byte fourByte[4];
					memcpy(fourByte, &payloadBuffer[bufferPosition], 4);

					if (thisDataMap == 'f') {
						float castFloat;
						castFloat = * (float *) fourByte;
						data.add(castFloat);
					}
					if (thisDataMap == 'L') {
						unsigned long castUlong;
						castUlong = * (unsigned long *) fourByte;
						data.add(castUlong);
					}
					if (thisDataMap == 'l') {
						long castLong;
						castLong = * (long *) fourByte;
						data.add(castLong);
					}
					if (thisDataMap == 'd') {
						double castDouble;
						castDouble = * (double *) fourByte;
						data.add(castDouble);
					}
					bufferPosition += 4;
				}
				else if (thisDataMap == 'i' or thisDataMap == 'I' or thisDataMap == 'w') {
					byte twoByte[2];
					memcpy(twoByte, &payloadBuffer[bufferPosition], 2);

					if (thisDataMap == 'i') {
						int castInt;
						castInt = * (int *) twoByte;
						data.add(castInt);
					}
					if (thisDataMap == 'I') {
						unsigned int castUint;
						castUint = * (unsigned int *) twoByte;
						data.add(castUint);
					}
					if (thisDataMap == 'w') {
						word castWord;
						castWord = * (word *) twoByte;
						data.add(castWord);
					}
					bufferPosition += 2;
				} else {
					Serial.print("Undefined: ");
					Serial.println(nodeMsg.PayloadDataMap[x]);
				}	
			}
		}
  	}
}


