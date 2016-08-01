#include <EEPROM.h>
#define DATA_SIZE 500

void eeprom_save_data(char* input){
  serial.println("eeprom_save_data()");
  serial.indent();
  
  EEPROM.begin(512);

  for (int i=0; i<strlen(input); i++){
    EEPROM.write(i, *(input+i));
  }
  EEPROM.commit();

  EEPROM.end();
  serial.unindent();
}

void eeprom_load_data(char** output){
  serial.println("eeprom_load_data()");
  serial.indent();
  
  EEPROM.begin(512);
  
  char buffer[DATA_SIZE];
  int startAddress = 0;
  EEPROM.get(startAddress, buffer);

  *output = (char *) malloc(strlen(buffer));
  strcpy(*output, buffer);

  EEPROM.end();
  serial.unindent();
}