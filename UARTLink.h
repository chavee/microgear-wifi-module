#ifndef UART_LINK_H_
#define UART_LINK_H_

#define ESP_HEADER "ESP"
#define OUT_HEADER "OUT"


#define BAUD_RATE 250000
#define MAX_BUFFER_SIZE 256

#include <Arduino.h>
#include "HardwareSerial.h"
#include <ESP8266WiFi.h>
class UARTLink {

public:
      UARTLink(HardwareSerial*);

      void loop();
      void begin(int buad=BAUD_RATE);

      //WiFiClient* client;
      void out(char*);
      void out(char*,char*);
      void out(int);
      void out(size_t);
      void out(bool);
      void out(IPAddress);
      bool isInbox();
      char* getMethod();
      char* getCommand();
      char* getParam1();
      char* getParam2();
      char* getParam3();
      void clear_param();
      uint8_t getParamCount();
      uint8_t* getU8_Param1();
      uint8_t* getU8_Param2();
      uint8_t* getU8_Param3();
      size_t getU8_Size();
      void setMSG();
      void split_byte();
      void split_byte3();

private:


      bool msgIncome;
      bool isMSG;

      char * command;
      char * method;
      char * content;
      char * param1;
      char * param2;
      char * param3;

      uint8_t  u8_param1[50];
      uint8_t  u8_param2[50];
      uint8_t  u8_param3[50];
      size_t write_size;
      uint8_t count_para;
      uint8_t incomeBuffer[MAX_BUFFER_SIZE];
      uint8_t incomeBufferBackup[MAX_BUFFER_SIZE];
      uint8_t outBuffer[MAX_BUFFER_SIZE];
      uint16_t indexMSG;
      HardwareSerial* inSerial;
      uint8_t prev_char1;
      uint8_t prev_char2;
};

#endif /* SERIALINTERFACE_H_ */
