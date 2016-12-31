//avr-json Proof of Concept tesing using ArduinoJson for Wireless Sensor Network Comminication
/*
Change Log
2016-12-23: Test JSON Output
2016-12-24: Add Serial input

*/
// Encode data from serial port to an RFM69 message
/*
// Serialized exmaple removing map declaration
echo '{"node":"14","meth":2,"type":6,"data":[10,2,41,250445,42,321.45,42,88.45]}' > /dev/ttyLPL			

*/
#include <ArduinoJson.h>
#include <RFM69.h>
#include <SPI.h>
#include <NodeMsg.h>
#include <GatewayConf.h>
// Uncomment for debugging 
// #define DEBUG_SERIAL
RFM69 radio;
// Note:
// SERIAL_BUFFER_SZ & JSON_BUFFER_SZ defined in GatewayConf.h
char SerialBuffer[SERIAL_BUFFER_SZ];


void setup() {
	// Setup Serial port
	Serial.begin(SERIAL_BAUD);
	// Wait for serial to start
	while( ! Serial ) {}
	// //Setup RFM69 Radio
	radio.initialize(FREQUENCY, NODEID, NETWORKID);
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
		StaticJsonBuffer<JSON_BUFFER_SZ> jsonInBuffer;
		JsonObject& json_in = jsonInBuffer.parseObject(SerialBuffer);
		// Raise error if data received can not be parsed into json.
		if (!json_in.success()) {
    		Serial.println("parseObject() failed");
    		return;
  		}

  		nodeMsg.NodeID = json_in["node"];
  		nodeMsg.Method = json_in["meth"];
  		nodeMsg.TypeID = json_in["type"];
  		// nodeMsg.PayloadDataMap = json_in["map"];

  		#ifdef DEBUG
		Serial.print("Node: ");
		Serial.println(nodeMsg.NodeID);
		Serial.print("Method: ");
		Serial.println(nodeMsg.Method);
		Serial.print("Type: ");
		Serial.println(nodeMsg.TypeID);
		#endif
		int msgPayloadOffset = 0;
		byte JsonDataElement = 0;
		bool bufferHit = false;
		 do {
		 	// fail test
		 	bufferHit = false;
		 	
  			#ifdef DEBUG
		 	Serial.print("JsonDataElement = ");
			Serial.print(JsonDataElement);
			Serial.print(", Control = ");
			Serial.println((byte)json_in["data"][JsonDataElement]);
			#endif

		 	if (json_in["data"][JsonDataElement] == _byte_) {
		 		nodeMsg.MsgPayload[msgPayloadOffset] = (byte)json_in["data"][JsonDataElement];
		 		msgPayloadOffset += 1;

		 		// JsonDataElement +=1;
		 		bufferHit = true;
		 		// Parse Byte
		 		#ifdef DEBUG
 				Serial.print("Json_Data Byte = ");
				Serial.println((byte)json_in["data"][JsonDataElement +1]);
				#endif

		 		nodeMsg.MsgPayload[msgPayloadOffset] = (byte)json_in["data"][JsonDataElement +1];
		 		msgPayloadOffset += 1;
		 	} else if (
		 		json_in["data"][JsonDataElement] == _int_ or 
		 		json_in["data"][JsonDataElement] == _uint_ or 
		 		json_in["data"][JsonDataElement] == _word_ 
		 		) {
		 		// JsonDataElement +=1;
		 		nodeMsg.MsgPayload[msgPayloadOffset] = (byte)json_in["data"][JsonDataElement];
		 		msgPayloadOffset += 1;
		 		bufferHit = true;
		 		#ifdef DEBUG
		 		Serial.println("Decode Int,Uint,Word = ");
		 		#endif
		 		if (json_in["data"][JsonDataElement] == _int_ ) { 
		 			#ifdef DEBUG
		 			Serial.print("Json_Data Int = ");
					Serial.println((int)json_in["data"][JsonDataElement +1]);
		 			#endif
		 			ByteConvert.i = (int)json_in["data"][JsonDataElement +1];
		 		} else if (json_in["data"][JsonDataElement] == _uint_ ) { 
		 			#ifdef DEBUG
		 			Serial.print("Json_Data UInt = ");
					Serial.println((unsigned int)json_in["data"][JsonDataElement +1]);
		 			#endif
		 			ByteConvert.I = (unsigned int)json_in["data"][JsonDataElement +1];
		 		} else if (json_in["data"][JsonDataElement] == _word_ ) {
		 			#ifdef DEBUG
		 			Serial.print("Json_Data word = ");
					Serial.println((word)json_in["data"][JsonDataElement +1]);
		 		 	#endif
		 		 	ByteConvert.w = (word)json_in["data"][JsonDataElement +1];
		 		}
		 		for (int x = 0; x < 2; x++) {
					nodeMsg.MsgPayload[msgPayloadOffset +x] = ByteConvert.b[x];
				} 		
				msgPayloadOffset += 2;	
		 	} else if (
		 		json_in["data"][JsonDataElement] == _long_ or 
		 		json_in["data"][JsonDataElement] == _ulong_ or 
		 		json_in["data"][JsonDataElement] == _float_ or 
		 		json_in["data"][JsonDataElement] == _double_ 
		 		) {
		 		// JsonDataElement +=1;
		 		nodeMsg.MsgPayload[msgPayloadOffset] = (byte)json_in["data"][JsonDataElement];
		 		msgPayloadOffset += 1;
		 		bufferHit = true;
		 		if (json_in["data"][JsonDataElement] == _long_ ) {
		 			#ifdef DEBUG
		 			Serial.print("Json_Data long = ");
		 			#endif
		 			ByteConvert.l = (long)json_in["data"][JsonDataElement+1];
		 		} else if (json_in["data"][JsonDataElement] == _ulong_ ) {
		 			#ifdef DEBUG
		 			Serial.print("Json_Data ulong = ");
		 			#endif
		 			ByteConvert.L = (unsigned long)json_in["data"][JsonDataElement+1];
		 		} else if (json_in["data"][JsonDataElement] == _float_ ) {
		 			#ifdef DEBUG
		 			Serial.print("Json_Data float = ");
		 			#endif
		 			ByteConvert.f = (float)json_in["data"][JsonDataElement +1];
		 		} else if (json_in["data"][JsonDataElement] == _double_ ) {
		 			#ifdef DEBUG
		 			Serial.print("Json_Data double = ");
		 			#endif
		 			ByteConvert.d = (double)json_in["data"][JsonDataElement+1];
		 		}
		 		for (int x = 0; x < 4; x++) {
					nodeMsg.MsgPayload[msgPayloadOffset +x] = ByteConvert.B[x];
				} 		
				msgPayloadOffset += 4;	
		 	}

		 	// Read onle even numbered json_in["data"] elements
		 	JsonDataElement += 2;
		 	#ifdef DEBUG
		 	Serial.print("msgPayloadOffset: ");
		 	Serial.print(msgPayloadOffset);
		 	Serial.println();
		 	#endif

		 } while (bufferHit);
		// Send network encode message to target
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
			//json_out["node"] = nodeMsg.NodeID;
			json_out["node"] = radio.SENDERID;
			json_out["meth"] = nodeMsg.Method;
			json_out["type"] = nodeMsg.TypeID;

			// Copy contents of node msg into array for processing. 
			// Note this step could be skipped to save memory later...
			// memcpy(payloadBuffer, &nodeMsg.MsgPayload, MAX_PAYLOAD_SIZE);

			// Create json data array for storing decoded contents of payload
			JsonArray& data = json_out.createNestedArray("data");
			
			// Decode types from MsgPayload, add to json array 
			byte buffPos = 0;
			bool bufferHit = true;
			do {
				bufferHit = false;
				if (nodeMsg.MsgPayload[buffPos] == _byte_ or
					nodeMsg.MsgPayload[buffPos] == _char_ or
					nodeMsg.MsgPayload[buffPos] == _uchar_
					) {
					bufferHit = true;
					data.add((byte)nodeMsg.MsgPayload[buffPos +1]);
					data.add((byte)nodeMsg.MsgPayload[buffPos]);
					// move buffPos to next MsgDataType control position
					buffPos =+ 2;
				} else if (
					nodeMsg.MsgPayload[buffPos] == _int_ or
					nodeMsg.MsgPayload[buffPos] == _uint_ or
					nodeMsg.MsgPayload[buffPos] == _word_
					) {
					//byte twoByte[2]; 
					bufferHit = true;
					data.add((byte)nodeMsg.MsgPayload[buffPos]);
					memcpy(ByteConvert.b, &nodeMsg.MsgPayload[buffPos +1], 2);
					if (nodeMsg.MsgPayload[buffPos] == _int_) {
						data.add(ByteConvert.i);
					} else if (nodeMsg.MsgPayload[buffPos] == _uint_) {
						data.add(ByteConvert.I);
					} else if (nodeMsg.MsgPayload[buffPos] == _word_) {
						data.add(ByteConvert.w);
					}
					buffPos +=3;
				} else if ( 
					nodeMsg.MsgPayload[buffPos] == _long_ or 
					nodeMsg.MsgPayload[buffPos] == _ulong_ or 
					nodeMsg.MsgPayload[buffPos] == _float_ or 
					nodeMsg.MsgPayload[buffPos] == _double_ 
					) {
					bufferHit = true;
					data.add((byte)nodeMsg.MsgPayload[buffPos]);
					memcpy(ByteConvert.B, &nodeMsg.MsgPayload[buffPos +1], 4);
					if (nodeMsg.MsgPayload[buffPos] == _long_) {
						data.add(ByteConvert.l);
					} else if (nodeMsg.MsgPayload[buffPos] == _ulong_) {
						data.add(ByteConvert.L);
					} else if (nodeMsg.MsgPayload[buffPos] == _float_) {
						data.add(ByteConvert.f);
					} else if (nodeMsg.MsgPayload[buffPos] == _double_) {
						data.add(ByteConvert.d);
					}
					buffPos +=5;
				}
			} while (bufferHit); 
			json_out.printTo(Serial);
			Serial.println();
			Blink(LED, 10);
			delay(100);
			Blink(LED, 10);
			delay(100);
			Blink(LED, 10);
		} // End Radio Receive
  	}
}


