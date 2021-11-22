#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "SD_MMC.h"
#include <spartan-edge-esp32-boot.h>

extern "C" {
  //external header for the pulling up of SD pins at the initialization
  #include "driver/sdmmc_host.h"
}

spartan_edge_esp32_boot esp32Cla;

const char* ssid = "VM6851248";        //Write your own Wi-Fi name here
const char* password = "yfCwzw6J5ghc";    //Write your own password here


AsyncWebServer server(80); //object created on default port 80

void initmicroSDCard(){
  //PULL UP of SD card pins <--- preventing mounting failure due to floating state
  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4
  
  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed,please reboot the board");
  while(1);
    return;
  }
  
//  if(!esp32Cla.begin()){
//  Serial.println("init failed");
//    return;}
}
//  if(!SD.begin()){
//    Serial.println("Initialization Failed");
//    return;
//  }

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi network");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(2000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  delay(1000);
  initmicroSDCard();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.serveStatic("/", SD_MMC, "/");

  server.begin();
}

void loop() {
  
}
