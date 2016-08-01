
/* 
   ========================================
    UTILITY METHODS
   ========================================
*/

int HL = 0;

void switchLED (){
  if (HL == 0) {
    HL = 1;
  } else {
    HL = 0;
  }
  digitalWrite(BUILTIN_LED, HL);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because 
  // it is acive low on the ESP-01)
}



