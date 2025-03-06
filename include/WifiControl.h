#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include "MemoryControl.h"

bool connected = false;
WebServer server(80);
String ssidEP32 = "SmartBoiler KILL";
String passwordESP32 = "12345678";

void handleRoot();
void connectToWifi();
void handleData();
void initAP();

void handleRoot() {
    server.send(200, "text/plain", ssidSaved + " " + passwordSaved + " "+ String(connected)) ;
}

void connectToWifi() {
    WiFi.begin(ssidSaved.c_str(), passwordSaved.c_str());  
    Serial.print("Conectando");

    delay(5000);

    connected = WiFi.status() == WL_CONNECTED;
    
}


void handleData() {
    if(server.arg("ssid") && server.arg("password")) {
        ssidSaved = server.arg("ssid");
        passwordSaved = server.arg("password");
        writeMemory();
        connectToWifi();
        server.send(200, "text/plain", "Datos recibidos correctamente");
    } else {
        server.send(400, "text/plain", "No se recibieron datos.");
    }
}

void initAP(){
    WiFi.softAP(ssidEP32, passwordESP32);
    MDNS.begin("esp32");
    server.on("/", handleRoot);
    server.on("/data", HTTP_POST, handleData);
    server.begin();
}

void initWifi(){
    WiFi.mode(WIFI_AP_STA); 
    if(readMemory()) connectToWifi(); 
    initAP();
}

#endif
