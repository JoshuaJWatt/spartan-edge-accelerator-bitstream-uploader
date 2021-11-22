/*
  Based on WiFiAccessPoint.ino:
        WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.
      
        Steps:
        1. Connect to the access point "yourAp"
        2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
           OR
           Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port
      
        Created for arduino-esp32 on 04 July, 2018
        by Elochukwu Ifediora (fedy0)
  
  Created for seeed spartan edge accelerator esp32 by JoshuaJWatt
*/

// Not all of these are necessary, I'll go through and clean up once everything is working
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SD_MMC.h"

#define FORMAT_SPIFFS_IF_FAILED true

extern "C" {
  //external header for the pulling up of SD pins at the initialization
  #include "driver/sdmmc_host.h"
}

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

// Set these to your desired credentials. Eventually pull these from a config file.
const char *ssid = "yourAP";
const char *password = "yourPassword";

AsyncWebServer server(80);

String getContentType(String filename);
bool handleFileRead(String path);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  esp32Cla.begin();
  
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  Serial.println("Server started");

  //PULL UP of SD card pins <--- preventing mounting failure due to floating state
  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4
  
  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed,please reboot the board");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.serveStatic("/", SD_MMC, "/");
  server.begin();
   
  Serial.println("page up");
}

void loop() {
  
}
