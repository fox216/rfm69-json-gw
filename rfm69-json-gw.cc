//avr-json Proof of Concept tesing using ArduinoJson for Wireless Sensor Network Comminication
/*
Change Log
2016-12-23: Test JSON Output
2016-12-24: Add Serial input

*/

#include <ArduinoJson.h>
#include <RFM69.h>
#include <SPI.h>
#include <GatewayMsg.h> // Load Gateway Message structure:Task
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
	radio.initialize(FREQUENCY, NODEID, NETWORKID);
	// Wait for Radio to start
	while(! radio) {} // Unknown 
	// Setup RFM69 Encryption
	radio.encrypt(KEY);

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



  	} else {
  		// Process incoming RFM69 data
  		// Parse RFM69 Message and send to Serial
  		StaticJsonBuffer<JSON_BUFFER_SZ> jsonBuffer;
		JsonObject& json_out = jsonBuffer.createObject();


  	
  	}
}