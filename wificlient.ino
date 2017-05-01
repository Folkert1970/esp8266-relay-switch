// WIFI|KevinVo2.4Ghz-NEW|<password>



#define LONG_WAIT 30000 //30 seconds
#define SHORT_WAIT 10000 //10 seconds


/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;

//  Just for reference
//typedef enum {
//    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//    WL_IDLE_STATUS      = 0,
//    WL_NO_SSID_AVAIL    = 1,
//    WL_SCAN_COMPLETED   = 2,
//    WL_CONNECTED        = 3,
//    WL_CONNECT_FAILED   = 4,
//    WL_CONNECTION_LOST  = 5,
//    WL_DISCONNECTED     = 6
//} wl_status_t;


void setupWifi() {    
    connectWifi();
    reconnect = WiFi.status() != WL_CONNECTED;
}

void loopWifi() {
  
  // Check reconnect variable for wifi reconnection
  if (reconnect && isSet(wifi_ssid)) {
    serial.println ( "Connect requested" );
    connectWifi();
    reconnect = false;
    lastConnectTry = millis();
  }
  
  // Check WIFI status to set reconnect variable to TRUE for next loop
  int s = WiFi.status();
  if (s == WL_IDLE_STATUS && millis() > (lastConnectTry + SHORT_WAIT) ) {
      /* If WLAN disconnected and idle try to connect */
      reconnect = true;
  }
  
  // WIFI STATUS changes from IDLE to CONNECTED or vice versa
  if (wifi_status != s) { 
    serial.print( "loop_wifi(): WIFI Status = " );serial.println ( s );    
    wifi_status = s; //update wifi status
    
    if (s == WL_CONNECTED) {
      
      /* Just connected to WLAN */
      serial.print ( "Just successfully connected to " ); serial.println ( wifi_ssid );
      serial.print ( "Obtained IP address: " ); serial.println ( WiFi.localIP() );

    } else if (s == WL_NO_SSID_AVAIL) {
      WiFi.disconnect();
    }
  }
  
}

void connectWifi() {
  serial.println("connectWifi()");
  serial.indent();
  
  serial.println("Connecting as WIFI client...");
  serial.print("SSID: "); serial.println(wifi_ssid);
  serial.print("Password: "); serial.println(wifi_password);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin ( wifi_ssid, wifi_password ); //try to make connection
  wifi_status = WiFi.waitForConnectResult();
  if (wifi_status != WL_CONNECTED){
    serial.print ( "connectWifi(): Wifi failed, status: " );serial.println ( wifi_status );
  } else {
    serial.print ( "connectWifi(): Wifi succeeded, status: " );serial.println ( wifi_status );
    serial.print ("IP Address: "); serial.println(WiFi.localIP());
  }
  serial.unindent();
}

boolean isSet(const char* c){
  return !(c && !c[0]);
}
