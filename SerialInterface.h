#ifndef SERIALINTERFACE_H_
#define SERIALINTERFACE_H_

#define ESP_HEADER "ESP"
#define OUT_HEADER "OUT"

#define WIFI_COMMAND "wifi"
#define CLIENT_COMMAND "client"

#define WIFI_BEGIN  "begin"
#define WIFI_IP  "localIP"
#define WIFI_STATUS  "status"

#define CLIENT_CONNECT  "connect"
#define CLIENT_WRITE  "write"
#define CLIENT_READ  "read"
#define CLIENT_AVAILABLE  "available"
#define CLIENT_CONNECTED  "connected"
#define CLIENT_STOP  "stop"

#define WIFI_BEGIN_METHOD  1
#define WIFI_IP_METHOD  2
#define WIFI_STATUS_METHOD  3

#define CLIENT_CONNECT_METHOD   11
#define CLIENT_WRITE_METHOD     12
#define CLIENT_READ_METHOD      13
#define CLIENT_AVAILABLE_METHOD 14
#define CLIENT_CONNECTED_METHOD 15
#define CLIENT_STOP_METHOD      16


#define BAUD_RATE 250000
//#define BAUD_RATE 115200
#define MAX_BUFFER_SIZE 256

#include <Arduino.h>
#include "HardwareSerial.h"
#include <ESP8266WiFi.h>
class SerialInterface {

public:
      SerialInterface(HardwareSerial*,WiFiClient*);
      void begin(int buad=BAUD_RATE);
      void loopMSG();
      bool isMSG();
      void out(char*);
      void out(int);
      void out(size_t);
      void out(bool);
      void serialWifi();
      void serialClient();
      HardwareSerial* inSerial;
      WiFiClient* client;
private:
      bool isWifi();
      bool isClient();
      uint8_t isMethod();
      void clearBuffer();
      // Wifi Method
      void wifiBegin();
      void wifiIP();


      void wifiStatus();


      // client
      void clientConnect();
      void clientWrite();
      void clientRead();
      void clientAvaliable();
      void clientConnected();
      void clientStop();
      bool msgIncome;
      char * command;
      char * method;
      uint8_t incomeBuffer[MAX_BUFFER_SIZE];
      uint8_t outBuffer[MAX_BUFFER_SIZE];
      //String incomeBuffer;
      uint16_t indexMSG;
};

#endif /* SERIALINTERFACE_H_ */
