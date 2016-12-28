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
RFM69 radio;

// Note:
// SERIAL_BUFFER_SZ & JSON_BUFFER_SZ defined in GatewayConf.h

char SerialBuffer[SERIAL_BUFFER_SZ];
byte MsgPayloadBuffer[MAX_PAYLOAD_SIZE];
int MsgPayloadSz = 0;

void setup() {
	// Setup Serial port
	Serial.begin(SERIAL_BAUD);
	// Wait for serial to start
	while( ! Serial ) {}
	// Setup RFM69 Radio
	// radio.initialize(FREQUENCY, NODEID, NETWORKID);
	// // Wait for Radio to start
	// while(! radio) {} // Unknown 
	// // Setup RFM69 Encryption
	// radio.encrypt(KEY);
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

  		*/

  		nodeMsg.NodeID = json_in["node"];
  		nodeMsg.Method = json_in["meth"];
  		nodeMsg.TypeID = json_in["type"];
  		nodeMsg.PayloadDataMap = json_in["map"];


		Serial.print("Node: ");
		Serial.println(nodeMsg.NodeID);
		Serial.print("Method: ");
		Serial.println(nodeMsg.Method);
		Serial.print("Type: ");
		Serial.println(nodeMsg.TypeID);
		Serial.print("Map: ");
		Serial.println(nodeMsg.PayloadDataMap);

		// Decode data from json data array 
		MsgPayloadSz = sizeof(json_in["data"]);
		Serial.print("Json Payload Size: ");
		Serial.println(MsgPayloadSz);

		// read json metadata (PayloadDataMap) write objects to payload byte array
		for (int x = 0; x < strlen(nodeMsg.PayloadDataMap); x++) {
			char thisDataMap = nodeMsg.PayloadDataMap[x];
			Serial.print("Map Inspect -> ");
			Serial.println(thisDataMap);
			
			if ( thisDataMap == 'b') {
				Serial.print("Byte Data: ");
				Serial.println((byte)json_in["data"][x]);
				nodeMsg.MsgPayload[x] = (byte)json_in["data"][x];
			} 
			else if (thisDataMap == 'f' or thisDataMap == 'L' or thisDataMap == 'l' or thisDataMap == 'd') {
				if ( thisDataMap == 'L') {
					Serial.print("uLong Data: ");
					Serial.println((unsigned long)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (unsigned long)json_in["data"][x];
				} else if ( thisDataMap == 'l') {
					Serial.print("Long Data: ");
					Serial.println((long)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (long)json_in["data"][x];
				} else if ( thisDataMap == 'f') {
					Serial.print("Float Data: ");
					Serial.println((float)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (float)json_in["data"][x];
				} else if ( thisDataMap == 'd') {
					Serial.print("Double Data: ");
					Serial.println((double)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (double)json_in["data"][x];
				} 			
			} else if (thisDataMap == 'i' or thisDataMap == 'I' or thisDataMap == 'w') {
				if ( thisDataMap == 'i') {
					Serial.print("int Data: ");
					Serial.println((int)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (int)json_in["data"][x];
				} else if (thisDataMap == 'I') {
					Serial.print("uInt Data: ");
					Serial.println((unsigned int)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (unsigned int)json_in["data"][x];					
				} else if (thisDataMap == 'w') {
					Serial.print("Word Data: ");
					Serial.println((word)json_in["data"][x]);
					nodeMsg.MsgPayload[x] = (word)json_in["data"][x];					
				}
			}
		}
		// switch(nodeMsg.TypeID) {
		// case 1:
		// 	msg_1.key = json_in["payload"][0];
		// 	msg_1.value = (byte)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_1.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_1.value);
		// break;
		// case 2:
		// 	msg_2.key = json_in["payload"][0];
		// 	msg_2.value = json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_2.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_2.value);
		// break;
		// case 3:
		// 	msg_3.key = json_in["payload"][0];
		// 	msg_3.value = (int)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_3.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_3.value);
		// break;
		// case 4:
		// 	msg_4.key = json_in["payload"][0];
		// 	msg_4.value = (const char*)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_4.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_4.value);
		// break;
		// case 5:
		// 	msg_5.key = json_in["payload"][0];
		// 	msg_5.value = (long)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_5.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_5.value);
		// break;
		// case 6:
		// 	msg_6.key = json_in["payload"][0];
		// 	msg_6.value = (double)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_6.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_6.value);
		// break;
		// case 7:
		// 	msg_7.key = json_in["payload"][0];
		// 	msg_7.value = (float)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_7.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_7.value);
		// break;
		// case 8:
		// 	msg_8.key = json_in["payload"][0];
		// 	msg_8.value = (float)json_in["payload"][1];
		// 	Serial.print("key: ");
		// 	Serial.println(msg_8.key);
		// 	Serial.print("value: ");
		// 	Serial.println(msg_8.value);
		// break;
		// default:
		// 	StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		// 	JsonObject& json_out = jsonBuffer.createObject();
		// 	json_out["GWError"] = 2;
		// 	json_out["Type"] = nodeMsg.PayloadType;
		// 	json_out.printTo(Serial);
		// 	Serial.println();
		// break;
		// }



  	//} 
  // 	else {
  // 		// Process incoming RFM69 data
  // 		// Parse RFM69 Message and send to Serial
  // 		StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		// JsonObject& json_out = jsonBuffer.createObject();
		// if ( radio.receiveDone() ) {
		// 	// Data received from radio
		// 	// Process date / forward to serial port
		// 	payload = *(Payload*)radio.DATA;

		// 	StaticJsonBuffer<200> jsonBuffer;

		// 	JsonObject& json_out = jsonBuffer.createObject();


		// 	msg.NodeID = 123;
		// 	msg.Method = 2;
		// 	msg.TypeID = 200;
		// 	msg.DataMap = "bbLfiIL"; // Byte, Byte, Unsigned Long, float, integer, Unsigned Integer

		// 	json_out["node"] = msg.NodeID;
		// 	json_out["meth"] = msg.Method;
		// 	json_out["type"] = msg.TypeID;
		// 	json_out["map"] = msg.DataMap;

		// 	sampleData.b1 = 42;
		// 	sampleData.b2 = 43;
		// 	sampleData.uL1 = 999999;
		// 	sampleData.f1 = 175.06;
		// 	sampleData.i1 = -678;
		// 	sampleData.uI1 = loopCount;
		// 	sampleData.uL2 = millis();

		// 	msg.MsgSize = sizeof(sampleData);
		// 	Serial.print("Message Size: ");
		// 	Serial.println(msg.MsgSize);

		// 	memcpy(payloadBuffer, &sampleData, (int)msg.MsgSize);
		// 	Serial.print("Data Map Size: ");
		// 	Serial.println( strlen(msg.DataMap) );

		// 	Serial.print("Data Map Value: ");
		// 	Serial.println( msg.DataMap );
		// 	int bufferPosition = 0;

		// 	JsonArray& data = json_out.createNestedArray("data");
		// 	for (int x = 0; x < strlen(msg.DataMap); x++) {
		// 		//Serial.print("DataMap[");
		// 		// Serial.print(x);
		// 		// Serial.print("]: ");
		// 		// Serial.println(msg.DataMap[x]);

		// 		// Serial.print("BufferPosition: ");
		// 		// Serial.println(bufferPosition);
		// 		// Serial.println(payloadBuffer[bufferPosition], HEX);
		// 		char thisDataMap = msg.DataMap[x];

		// 		if (thisDataMap == 'b') {
		// 			data.add((byte)payloadBuffer[bufferPosition]);
		// 			bufferPosition += 1;
		// 		}
		// 		else if (thisDataMap == 'f' or thisDataMap == 'L' or thisDataMap == 'l' or thisDataMap == 'd') {
		// 			byte fourByte[4];
		// 			memcpy(fourByte, &payloadBuffer[bufferPosition], 4);

		// 			if (thisDataMap == 'f') {
		// 				float castFloat;
		// 				castFloat = * (float *) fourByte;
		// 				data.add(castFloat);
		// 			}
		// 			if (thisDataMap == 'L') {
		// 				unsigned long castUlong;
		// 				castUlong = * (unsigned long *) fourByte;
		// 				data.add(castUlong);
		// 			}
		// 			if (thisDataMap == 'l') {
		// 				long castLong;
		// 				castLong = * (long *) fourByte;
		// 				data.add(castLong);
		// 			}
		// 			if (thisDataMap == 'd') {
		// 				double castDouble;
		// 				castDouble = * (double *) fourByte;
		// 				data.add(castDouble);
		// 			}
		// 			bufferPosition += 4;

		// 		}
		// 		else if (thisDataMap == 'i' or thisDataMap == 'I' or thisDataMap == 'w') {
		// 			byte twoByte[2];
		// 			memcpy(twoByte, &payloadBuffer[bufferPosition], 2);

		// 			if (thisDataMap == 'i') {
		// 				int castInt;
		// 				castInt = * (int *) twoByte;
		// 				data.add(castInt);
		// 			}
		// 			if (thisDataMap == 'I') {
		// 				unsigned int castUint;
		// 				castUint = * (unsigned int *) twoByte;
		// 				data.add(castUint);
		// 			}
		// 			if (thisDataMap == 'w') {
		// 				word castWord;
		// 				castWord = * (word *) twoByte;
		// 				data.add(castWord);
		// 			}

		// 			bufferPosition += 2;
		// 		} else {
		// 			Serial.print("Undefined: ");
		// 			Serial.println(msg.DataMap[x]);
		// 		}	

		// }

		// {
		// 	"NodeID": Byte[1],
		// 	"Method": Byte[1],
		// 	"PayloadType": Byte[1];
		// 	"MsgSize": Byte[1]
		// 	"Data": 
		// 		[{1}
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 			[{1} "dataType": Byte[1], "Data": Byte[Max4]],
		// 		]
		// } Max Long = 
		// {
		// 	"NodeID": Byte[1],
		// 	"Method": Byte[1],
		// 	"PayloadType": Byte[1];
		// 	"MsgSize": Byte[1]
		// 	"Data": 
		// 		[{1}
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],					
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],
		// 			"dataType": Byte[1], 
		// 			"Data": Byte[Max4],
		// 		]
		// }



  	}
}


