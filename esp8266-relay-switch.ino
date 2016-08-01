#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

using namespace ArduinoJson::Internals;

#define SerialDebug false // set to true to get Serial output for debugging

IndentedPrint serial(Serial);

/** Current WLAN status */
int wifi_status = WL_IDLE_STATUS;

void setup() {
  
  Serial.begin(19200);
  // Serial.set_tx(5);
  // Serial.set_rx(6);
  while(!Serial);
  IndentedPrint serial(Serial);
  serial.setTabSize(3);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  delay(5000);
  
  loadConfigurations();
  setupWifi();
  setupMQTT();
  reconnectMQTT();
  
  switchLED();
  switchLED();
}


void loop() {
  // switchLED();
  checkSerialInputAndSaveConfigurations();
  loopWifi();
  reconnectMQTT();
  
}


// =================================== 
// UTILITY
// =================================== 

