#include <WiFiClient.h> 
#include <PubSubClient.h>

long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// String last_payload = "";

int switch_states[] = {0, 0};
int switch_pins[] = {5, 4};

int mqtt_connected(){
  return mqttClient.connected();
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  char c_payload[length];
  memcpy(c_payload, payload, length);
  c_payload[length] = '\0';
  Serial.print("RECEIVED CONTENT: ");
  Serial.println(c_payload);
  
  const char* topic_action  = "SWITCH_ACTION";
  const char* topic_status  = "SWITCH_STATUS";
  const char* cmd_query     = "QUERY";
  
  int cmp_status = strcmp(topic, topic_status);
  int cmp_action = strcmp(topic, topic_action);
  
  if(cmp_status == 0) {
    Serial.print("RECEIVED TOPIC SWITCH_STATUS!");    
    
    int cmp_query = strcmp(c_payload, cmd_query);
    if (cmp_query == 0){ // QUERY command is received
      Serial.print("RECEIVED REQUEST FOR STATUS, SENDING OUT STATUS...");   
      String out_payload =
       "0:" + String(switch_states[0]) +  ","
       "1:" + String(switch_states[1]);
      mqttClient.publish(topic_status, out_payload.c_str());
    }
  }
  
  if(cmp_action == 0) {
    Serial.print("RECEIVED TOPIC SWITCH_ACTION!");
    int relay_id = my_atoi(c_payload);
    Serial.print("RECEIVED REQUEST FOR SWITCHING RELAY ID: " + relay_id);   
    switch_relay(relay_id);
    String out_payload =
     "0:" + String(switch_states[0]) +  ","
     "1:" + String(switch_states[1]);
    mqttClient.publish(topic_status, out_payload.c_str());
  }
  
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);
  // }
  // Serial.println();
}

void setupMQTT() {
  if (wifi_status == WL_CONNECTED){ // wifi connected
    if (isSet(mqtt_server) && isSet(mqtt_port)){
      mqttClient.setServer(mqtt_server, my_atoi(mqtt_port));
      mqttClient.setCallback(mqtt_callback);
    }
  } else {
    
  }
}



void reconnectMQTT() {

  //  if (mqtt_server && !mqtt_server[0]){
  if (wifi_status == WL_CONNECTED && isSet(mqtt_server)){    
    if (!mqttClient.connected()) {
      long now = millis();
      if (now - lastMsg > 10000) {        
        Serial.print("Attempting MQTT connection...");
        Serial.print("MQTT Server: ");Serial.println(mqtt_server);
        Serial.print("MQTT Port: ");Serial.println(my_atoi(mqtt_port));
        Serial.print("MQTT Username: ");Serial.println(mqtt_username);
        Serial.print("MQTT Password: ");Serial.println(mqtt_password);
        mqttClient.setServer(mqtt_server, my_atoi(mqtt_port));
        
        // Attempt to connect
        int connected;
        if (mqtt_username && !mqtt_username[0]) {
          connected = mqttClient.connect(device_id);
        } else {
          connected = mqttClient.connect(device_id, mqtt_username, mqtt_password);
        }
        if (connected) {
          Serial.println("Connected to MQTT server.");
          // Once connected, publish an announcement...
          mqttClient.publish("HEARTBEAT", device_id);
          // ... and resubscribe
          mqttClient.subscribe("SWITCH_ACTION");
          mqttClient.subscribe("SWITCH_STATUS");
        } else {
          Serial.print("failed, rc=");
          Serial.print(mqttClient.state());
          Serial.println(" try again in later.");
        }
        lastMsg = now;
      }      
    } else {
      mqttClient.loop();
    }    
  } else {
    
  }
}


int mqtt_publish(){
  if (mqttClient.connected()) { 
    //    Serial.println("yes-connected");
//    String sax = String(1000*ax, 4);
//    sax.trim();
//    String say = String(1000*ay, 4);
//    say.trim();
//    String saz = String(1000*az, 4);
//    saz.trim();
//  
//  
//    String sgx = String(gx, 2);
//    sgx.trim();
//    String sgy = String(gy, 2);
//    sgy.trim();
//    String sgz = String(gz, 2);
//    sgz.trim();
//  
//    String smx = String((int) mx);
//    smx.trim();
//    String smy = String((int) my);
//    smy.trim();
//    String smz = String((int) mz);
//    smz.trim();
//  
//    String sTemp = String(Temperature, 2);
//    sTemp.trim();
//
//    String payload = "" + String(device_id) + ":" + sax + "," + say + "," + saz + "," +
//      sgx + "," + sgy + "," + sgz + "," +
//        smx + "," + smy + "," + smz + "," + 
//          // q[0] + "," + q[1] + "," + q[2] + "," + q[3] + "," +
//          sTemp;
//
//    if (mqttClient.publish("outTopic", payload.c_str())){
//         
//    } else {
//      //      Serial.println("publish failed.");
//    }
  }
  
  return 0;
}

//void publish_quat_data(){
//  if (mqttClient.connected()) { 
//
//    String payload = 
//      "" + String(device_id) + "," +
//        q.w + "," + q.x + "," + q.y + "," + q.z;
//
//    mqttClient.publish("QUAT", payload.c_str());
//
//    // if (last_payload == payload) {
//    //   //do not pub if last_payload is the same
//    // } else {
//    //   mqttClient.publish("QUAT", payload.c_str());
//    //   last_payload = payload;
//    // }
//    
//      
//  }
//
//}


//void publish_quat_and_euler_data(){
//  if (mqttClient.connected()) { 
//
//    String payload = 
//      "" + String(device_id) + "," +
//        q.w + "," + q.x + "," + q.y + "," + q.z + "," + euler[0] + "," + euler[1] + "," + euler[2] ;
//
//    mqttClient.publish("QUAT_EULER", payload.c_str());
//
//    // if (last_payload == payload) {
//    //   //do not pub if last_payload is the same
//    // } else {
//    //   mqttClient.publish("QUAT", payload.c_str());
//    //   last_payload = payload;
//    // }
//    
//      
//  }
//
//}


// Convert String to Int
int my_atoi(const char *s)
{
  int sign=1;
  if(*s == '-')
    sign = -1;
  // s++;
  int num=0;
  while(*s)
  {
    num=((*s)-'0')+num*10;
    s++;   
  }
  return num*sign;
}
