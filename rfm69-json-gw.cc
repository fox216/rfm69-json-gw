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

char SerialBuffer[SERIAL_BUFFER_SZ];

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
  		/*
			Serial Msg Template
			{
				node: "<target Node ID>", <byte>
				meth: "<Message Type",  <byte>
				type: "<Data Type>", <byte>
				data: [
					<Message Type Specific Content>
				]
			}

  		*/
  		// Encode data from serial port to an RFM69 message
  		nodeMsg.NodeID = json_in["node"];
  		nodeMsg.Method = json_in["meth"];
  		nodeMsg.PayloadType = json_in["type"];

		Serial.print("Node: ");
		Serial.println(nodeMsg.NodeID);
		Serial.print("Method: ");
		Serial.println(nodeMsg.Method);
		Serial.print("Type: ");
		Serial.println(nodeMsg.PayloadType);

		switch(nodeMsg.PayloadType) {
		case 1:
			msg_1.key = json_in["payload"][0];
			msg_1.value = json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_1.key);
			Serial.print("value: ");
			Serial.println(msg_1.value);
		break;
		case 2:
			msg_2.key = json_in["payload"][0];
			msg_2.value = json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_2.key);
			Serial.print("value: ");
			Serial.println(msg_2.value);
		break;
		case 3:
			msg_3.key = json_in["payload"][0];
			msg_3.value = (bool)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_3.key);
			Serial.print("value: ");
			Serial.println(msg_3.value);
		break;
		case 4:
			msg_4.key = json_in["payload"][0];
			msg_4.value = (int)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_4.key);
			Serial.print("value: ");
			Serial.println(msg_4.value);
		break;
		case 5:
			msg_5.key = json_in["payload"][0];
			msg_5.value = (const char*)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_5.key);
			Serial.print("value: ");
			Serial.println(msg_5.value);
		break;
		case 6:
			msg_6.key = json_in["payload"][0];
			msg_6.value = (long)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_6.key);
			Serial.print("value: ");
			Serial.println(msg_6.value);
		break;
		case 7:
			msg_7.key = json_in["payload"][0];
			msg_7.value = (double)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_7.key);
			Serial.print("value: ");
			Serial.println(msg_7.value);
		break;
		case 8:
			msg_8.key = json_in["payload"][0];
			msg_8.value = (float)json_in["payload"][1];
			Serial.print("key: ");
			Serial.println(msg_8.key);
			Serial.print("value: ");
			Serial.println(msg_8.value);
		break;
		default:
			StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
			JsonObject& json_out = jsonBuffer.createObject();
			json_out["GWError"] = 2;
			json_out["Type"] = nodeMsg.PayloadType;
			json_out.printTo(Serial);
			Serial.println();
		break;
		}



  	} else {
  		// Process incoming RFM69 data
  		// Parse RFM69 Message and send to Serial
  		StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		JsonObject& json_out = jsonBuffer.createObject();
		// if ( radio.receiveDone() ) {
		// 	// Data received from radio
		// 	// Process date / forward to serial port
		// 	payload = *(Payload*)radio.DATA;

		// }
  	}
}


