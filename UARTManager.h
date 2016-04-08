#ifndef UART_MANAGER_H_
#define UART_MANAGER__H_


#define WIFI_COMMAND "wifi"
#define CLIENT_COMMAND "client"
#define SEND_OK "ok"

#define WIFI_BEGIN  "begin"
#define WIFI_IP  "localIP"
#define WIFI_STATUS  "status"

#define WIFI_COMMAND_ID 1
#define CLIENT_COMMAND_ID 2
#define MICROGEAR_COMMAND_ID 3


#define WIFI_BEGIN_METHOD  1
#define WIFI_IP_METHOD  2
#define WIFI_STATUS_METHOD  3

#define CLIENT_CONNECT  "connect"
#define CLIENT_WRITE  "write"
#define CLIENT_WRITE_BYTE  "write_byte"
#define CLIENT_READ  "read"
#define CLIENT_READ_BYTE  "read_byte"
#define CLIENT_AVAILABLE  "available"
#define CLIENT_ISCONNECTED  "connected"
#define CLIENT_STOP  "stop"

#define CLIENT_CONNECT_METHOD         11
#define CLIENT_WRITE_METHOD           12
#define CLIENT_READ_METHOD            13
#define CLIENT_READ_BYTE_METHOD       14
#define CLIENT_AVAILABLE_METHOD       15
#define CLIENT_CONNECTED_METHOD       16
#define CLIENT_STOP_METHOD            17
#define CLIENT_WRITE_BYTE_METHOD      18



#define MICROGEAR_CONNECT  "microgear_connect"
#define MICROGEAR_ISCONNECTED  "microgear_connected"
#define MICROGEAR_SETNAME  "setName"
#define MICROGEAR_CHAT "chat"
#define MICROGEAR_LOOP "loop"
#define MICROGEAR_SETALIAS "setAlias"
#define MICROGEAR_PUBLISH "publish"
#define MICROGEAR_SUBSCRIBE "subscribe"
#define MICROGEAR_UNSUBSCRIBE "unsubscribe"
#define MICROGEAR_STATE "state"
#define MICROGEAR_ON "on"
#define MICROGEAR_INIT "init"
#define MICROGEAR_RESET_TOKEN "resetToken"

#define MICROGEAR_CONNECT_METHOD  30
#define MICROGEAR_ISCONNECTED_METHOD   31
#define MICROGEAR_SETNAME_METHOD   32
#define MICROGEAR_CHAT_METHOD  33
#define MICROGEAR_LOOP_METHOD  34
#define MICROGEAR_SETALIAS_METHOD  35
#define MICROGEAR_PUBLISH_METHOD  36
#define MICROGEAR_SUBSCRIBE_METHOD  37
#define MICROGEAR_UNSUBSCRIBE_METHOD  38
#define MICROGEAR_STATE_METHOD  39
#define MICROGEAR_ON_METHOD  40
#define MICROGEAR_INIT_METHOD  41
#define MICROGEAR_RESET_TOKEN_METHOD  42

#include "UARTLink.h"
#include <ESP8266WiFi.h>
#include "MicroGear.h"

#define LED_STATUS 2


class UARTManager : ESP8266WiFiClass , WiFiClient{

public:
  UARTManager(UARTLink* uart_link)
  {
    link = uart_link;
    pinMode(LED_STATUS, OUTPUT);
    digitalWrite(LED_STATUS,HIGH);
  };

  void manage();
  uint8_t isMethod();
  uint8_t isCommand();
  void initMicroGear(MicroGear* gear);
  void event_out(unsigned char,char*,uint8_t*,unsigned int);
  void isNetworkWork();
  void statusWiFiConnected();
  void statusMicrogearConnected();

private:
  static void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen);
  static void onFoundgear(char *topic, uint8_t* msg, unsigned int msglen);
  static void onLostgear(char *topic, uint8_t* msg, unsigned int msglen);
  static void onConnected(char *topic, uint8_t* msg, unsigned int msglen);
  char m_appid[15];
  char* m_key;
  char* m_secret;
  char* m_name;
  UARTLink* link;
  MicroGear* microgear;
};

#endif /* UART_INTERFACE_H_ */
