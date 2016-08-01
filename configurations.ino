#define MAX_DATA_SIZE     500
#define ACTION_WIFI       "WIFI"
#define ACTION_MQTT       "MQTT"
#define ACTION_DEVICE     "DEVICE"
#define ACTION_CALIBRATE  "CALIBRATE"

const char* wifi_ssid = "";
const char* wifi_password = "";
const char* device_id = "";
const char* mqtt_server = "";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_port = "";

/** Should I connect to WLAN asap? */
boolean reconnect = true;

void saveConfigurations(){  
  serial.println("saveConfigurations()");
  serial.indent();
  
  StaticJsonBuffer<MAX_DATA_SIZE> jsonBuffer;
  JsonObject& json_object = jsonBuffer.createObject();
 
  json_object["wifi_ssid"]      = wifi_ssid;
  json_object["wifi_password"]  = wifi_password;
  json_object["device_id"]      = device_id;
  json_object["mqtt_server"]    = mqtt_server;
  json_object["mqtt_port"]      = mqtt_port;
  json_object["mqtt_username"]  = mqtt_username;
  json_object["mqtt_password"]  = mqtt_password;
  
//  JsonArray& mag_bias = json_object.createNestedArray("magBias");
//  mag_bias.add(double_with_n_digits(magBias[0], 2));
//  mag_bias.add(double_with_n_digits(magBias[1], 2));
//  mag_bias.add(double_with_n_digits(magBias[2], 2));
//  
//  JsonArray& mag_scale = json_object.createNestedArray("magScale");
//  mag_scale.add(double_with_n_digits(magScale[0], 2));
//  mag_scale.add(double_with_n_digits(magScale[1], 2));
//  mag_scale.add(double_with_n_digits(magScale[2], 2));

  json_object.printTo(serial); //for debugging
  serial.println();

  char buffer[MAX_DATA_SIZE];
  json_object.printTo(buffer, sizeof(buffer));
  eeprom_save_data(buffer);  
}

void loadConfigurations(){  
  serial.println("loadConfigurations()");
  serial.indent();
  
  char* config_json;
  eeprom_load_data(&config_json);
  serial.print("config_json: "); serial.println(config_json);
 
  StaticJsonBuffer<MAX_DATA_SIZE> jsonBuffer;
  JsonObject& json_object = jsonBuffer.parseObject(config_json);

  // Test if parsing succeeds.
  if (!json_object.success()) {
    serial.println("parseObject() failed");
    serial.unindent();
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  wifi_ssid     = json_object["wifi_ssid"];
  wifi_password = json_object["wifi_password"];
  device_id     = json_object["device_id"];
  mqtt_server   = json_object["mqtt_server"];
  mqtt_port     = json_object["mqtt_port"];
  mqtt_username = json_object["mqtt_username"];
  mqtt_password = json_object["mqtt_password"];
//  magBias[0]    = json_object["magBias"][0];
//  magBias[1]    = json_object["magBias"][1];
//  magBias[2]    = json_object["magBias"][2];
//  magScale[0]   = json_object["magScale"][0];
//  magScale[1]   = json_object["magScale"][1];
//  magScale[2]   = json_object["magScale"][2];

  serial.println("Configuration Settings: ");
  serial.indent();
  serial.println(wifi_ssid);
  serial.println(wifi_password);
  serial.println(device_id);
  serial.println(mqtt_server);
  serial.println(mqtt_port);
  serial.println(mqtt_username);
  serial.println(mqtt_password);
  serial.unindent();
  
  serial.unindent();
}

/* 
Configuration Commands:

WIFI|KevinVo2.4Ghz-NEW|troioilatroi
MQTT|10.1.2.119|1883||
DEVICE|s01
*/
void checkSerialInputAndSaveConfigurations(){
  // serial.println("checkserialInput()");
  serial.indent();
  
  String input;
  if (Serial.available() > 0){
    input = Serial.readString();
    serial.print("input: ");serial.println(input);
    
    // Extract the 1st token of the string as ACTION
    int pipeIndex = input.indexOf('|');
    String action = input.substring(0, pipeIndex);    
    serial.print("ACTION: "); serial.println(action);
    
    char* substr;
    
    if (action == ACTION_WIFI) {            
      serial.indent();
      int secondPipeIndex = input.indexOf('|', pipeIndex+1);
      
      // EXTRACT wifi_ssid
      int substr_size = secondPipeIndex - pipeIndex - 1;
      if (substr_size <= 0) {
        wifi_ssid = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[pipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        wifi_ssid = substr;
      }
      
      // EXTRACT wifi_password
      substr_size = input.length() - secondPipeIndex - 1;
      if (substr_size <= 0) {
        wifi_password = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[secondPipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        wifi_password = substr;
      }
      
      serial.print("INPUT wifi_ssid: ");serial.println(wifi_ssid);
      serial.print("INPUT wifi_password: ");serial.println(wifi_password);
      saveConfigurations();      
      
      serial.unindent();
    } else if (action == ACTION_MQTT) {            
      serial.indent();      
      
      int secondPipeIndex = input.indexOf('|', pipeIndex+1);     
      int thirdPipeIndex = input.indexOf('|', secondPipeIndex+1);
      int fourthPipeIndex = input.indexOf('|', thirdPipeIndex+1);
      
      // EXTRACT mqtt_server
      int substr_size = secondPipeIndex - pipeIndex - 1;
      if (substr_size <= 0) {
        mqtt_server = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[pipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        mqtt_server = substr;
      }
      
      // EXTRACT mqtt_port
      substr_size = thirdPipeIndex - secondPipeIndex - 1;
      if (substr_size <= 0) {
        mqtt_port = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[secondPipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        mqtt_port = substr;
      }
      
      // EXTRACT mqtt_username
      substr_size = fourthPipeIndex - thirdPipeIndex - 1;
      if (substr_size <= 0) {
        mqtt_username = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[thirdPipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        mqtt_username = substr;
      }
      
      // EXTRACT mqtt_password
      substr_size = input.length() - fourthPipeIndex - 1;
      if (substr_size <= 0) {
        mqtt_password = "";
      } else {
        substr = (char*) malloc(substr_size);
        memcpy(substr, &input[fourthPipeIndex+1], substr_size);
        substr[substr_size] = '\0';
        mqtt_password = substr;
        // serial.println(substr);
      }
      
      serial.print("INPUT mqtt_server: "); serial.println(mqtt_server);
      serial.print("INPUT mqtt_port: "); serial.println(mqtt_port);
      serial.print("INPUT mqtt_username: "); serial.println(mqtt_username);
      serial.print("INPUT mqtt_password: "); serial.println(mqtt_password);
      
      saveConfigurations();
            
      serial.unindent();
    } else if (action == ACTION_DEVICE) {
     serial.indent();

     // EXTRACT device_id
     int substr_size = input.length() - pipeIndex - 1;
     if (substr_size <= 0) {
       device_id = "";
     } else {
       substr = (char*) malloc(substr_size);
       memcpy(substr, &input[pipeIndex+1], substr_size);
       substr[substr_size] = '\0';
       device_id = substr;
     }

     serial.print("INPUT device_id: "); serial.println(device_id);
     saveConfigurations();
     serial.unindent();
   } 
   // else if (action == ACTION_CALIBRATE) {
   //   magcalMPU9250(magBias, magScale);
   //   saveConfigurations();
   // }
    else {
      serial.indent();
      serial.println("Unrecognized command, please retry!");
      serial.unindent();      
    }
  }
  // delay(5000);
  
  serial.unindent();
}
