#ifndef MEMORY_CONTROL_H
#define MEMORY_CONTROL_H
#include <EEPROM.h>

const int EEPROM_SIZE  = 200;  // Define el tamano de la EEPROM 
const int SSID_ADDR  = 0;      // Direccion en la EEPROM donde guardaremos el SSID
const int PASS_ADDR = 100;
String ssidSaved = "";
String passwordSaved = "";

bool readMemory(){
    EEPROM.begin(EEPROM_SIZE);

    ssidSaved = EEPROM.readString(SSID_ADDR);
    passwordSaved = EEPROM.readString(PASS_ADDR);
   
    /*
    //LIMPIAR EEPROM
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
    */
   
    return (ssidSaved.length() != 0) && (passwordSaved.length() != 0);
}

bool writeMemory(){
    EEPROM.writeString(SSID_ADDR, ssidSaved);
    EEPROM.writeString(PASS_ADDR, passwordSaved);
    EEPROM.commit();
    return (ssidSaved.length() != 0) && (passwordSaved.length() != 0);
}

#endif
