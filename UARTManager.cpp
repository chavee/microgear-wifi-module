#include "UARTManager.h"


uint8_t UARTManager::isCommand()
{
  char* command = link->getMethod();
  if (!strcmp(command,WIFI_BEGIN))  return WIFI_COMMAND_ID;
  if (!strcmp(command,WIFI_IP))     return CLIENT_COMMAND_ID;

}
UARTManager* event_handler;
void UARTManager::onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {

  msg[msglen] = '\0';
  //Serial.print("Incoming message -->");
  //Serial.println((char*)msg);
  event_handler->event_out(MESSAGE,topic,msg,msglen);
}

void UARTManager::onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  //printf("Found new member --> ");
  //for (int i=0; i<msglen; i++)
  //Serial.print("Found new member -->");
  //Serial.println((char*)msg);
  event_handler->event_out(PRESENT,attribute,msg,msglen);

}

void UARTManager::onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  //printf("Lost member --> ");
 //Serial.print("Lost member -->");
 //Serial.println((char*)msg);
 event_handler->event_out(ABSENT,attribute,msg,msglen);

}

void UARTManager::onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  //Serial.println("Connected to NETPIE...");
 event_handler->event_out(CONNECTED,attribute,msg,msglen);
}

void UARTManager::event_out(unsigned char event,char* topic,uint8_t* msg,unsigned int msglen)
{
  char buffer_out[200];
  char event_char[4];
  char msglen_char[10];
  itoa(event,event_char,4);
  itoa(msglen,msglen_char,10);

  strcpy(buffer_out,event_char);
  strcat(buffer_out,";");
  if(topic){
    strcat(buffer_out,topic);
    strcat(buffer_out,";");
  }
  if(msg){
    strcat(buffer_out,(char*)msg);
    strcat(buffer_out,";");
  }
  if(msglen){
    strcat(buffer_out,msglen_char);
  }
  //strcat(buffer_out,msglen_char);
  //Serial.println(buffer_out);
  link->out("callback",buffer_out);
}

uint8_t UARTManager::isMethod()
{
  //method = strtok(NULL,";");
  char* method = link->getMethod();

  if (!strcmp(method,WIFI_BEGIN)) return WIFI_BEGIN_METHOD;
  if(!strcmp(method,WIFI_IP)) return WIFI_IP_METHOD;
  if(!strcmp(method,WIFI_STATUS)) return WIFI_STATUS_METHOD;

  if (!strcmp(method,CLIENT_CONNECT)) return CLIENT_CONNECT_METHOD;
  if (!strcmp(method,CLIENT_WRITE)) return CLIENT_WRITE_METHOD;
  if (!strcmp(method,CLIENT_WRITE_BYTE)) return CLIENT_WRITE_BYTE_METHOD;
  if (!strcmp(method,CLIENT_READ)) return CLIENT_READ_METHOD;
  if (!strcmp(method,CLIENT_READ_BYTE)) return CLIENT_READ_BYTE_METHOD;
  if (!strcmp(method,CLIENT_AVAILABLE)) return CLIENT_AVAILABLE_METHOD;
  if (!strcmp(method,CLIENT_ISCONNECTED)) return CLIENT_CONNECTED_METHOD;
  if (!strcmp(method,CLIENT_STOP)) return CLIENT_STOP_METHOD;


  if (!strcmp(method,MICROGEAR_CONNECT)) return MICROGEAR_CONNECT_METHOD;
  if (!strcmp(method,MICROGEAR_ISCONNECTED)) return MICROGEAR_ISCONNECTED_METHOD;
  if (!strcmp(method,MICROGEAR_SETNAME)) return MICROGEAR_SETNAME_METHOD;
  if (!strcmp(method,MICROGEAR_CHAT)) return MICROGEAR_CHAT_METHOD;
  if (!strcmp(method,MICROGEAR_LOOP)) return MICROGEAR_LOOP_METHOD;
  if (!strcmp(method,MICROGEAR_SETALIAS)) return MICROGEAR_SETALIAS_METHOD;
  if (!strcmp(method,MICROGEAR_PUBLISH)) return MICROGEAR_PUBLISH_METHOD;
  if (!strcmp(method,MICROGEAR_SUBSCRIBE)) return MICROGEAR_SUBSCRIBE_METHOD;
  if (!strcmp(method,MICROGEAR_UNSUBSCRIBE)) return MICROGEAR_UNSUBSCRIBE_METHOD;
  if (!strcmp(method,MICROGEAR_STATE)) return MICROGEAR_STATE_METHOD;
  if (!strcmp(method,MICROGEAR_ON)) return MICROGEAR_ON_METHOD;
  if (!strcmp(method,MICROGEAR_INIT)) return MICROGEAR_INIT_METHOD;
}

void UARTManager::initMicroGear(MicroGear* gear)
{
    microgear = gear;
    event_handler=this ;
}

void UARTManager::statusWiFiConnected()
{
  digitalWrite(LED_STATUS,HIGH);
  delay(300);
  digitalWrite(LED_STATUS,LOW);
  delay(300);
}

void UARTManager::statusMicrogearConnected()
{
  digitalWrite(LED_STATUS,LOW);
}

void UARTManager::isNetworkWork()
{
  if(ESP8266WiFiClass::status() == WL_CONNECTED)
  {
    if(microgear->connected())
      statusMicrogearConnected();
    //  statusWiFiConnected();
    //else
      //statusMicrogearConnected();
  }
}

void UARTManager::manage()
{

 link->loop();
 int result = 0;

 //char* ssid = "";
 //char* pass = "";
isNetworkWork();
if(link->isInbox())
{

    char* param1 = "";
    char* param2 = "";
    char* param3 = "";

    param1 = link->getParam1();
    param2 = link->getParam2();
    param3 = link->getParam3();


    switch (isMethod())
    {


      case WIFI_BEGIN_METHOD:
      link->out(begin(param1,param2));
      break;
      case WIFI_IP_METHOD:
      link->out(ESP8266WiFiClass::localIP());
      break;
      case WIFI_STATUS_METHOD:
      link->out(ESP8266WiFiClass::status());
      break;

      case CLIENT_CONNECT_METHOD:
      result = connect(param1,atoi(param2));
      link->out(result);
      break;
      case CLIENT_AVAILABLE_METHOD:
      link->out(available());
      break;
      case CLIENT_CONNECTED_METHOD:
      link->out(connected());
      break;
      case CLIENT_WRITE_METHOD:
      link->out(write(link->getU8_Param1(),link->getU8_Size()));
      break;
      case CLIENT_WRITE_BYTE_METHOD:
      link->out(write((const uint8_t* )param1,atoi(param2)));
      break;
      case CLIENT_READ_BYTE_METHOD:
      link->out(read());
      break;
      case CLIENT_READ_METHOD:
      link->out(read((uint8_t* )param1,atoi(param2)));
      break;
      case CLIENT_STOP_METHOD:
      this->stop();
      link->out(SEND_OK);
      break;


      case MICROGEAR_CONNECT_METHOD:

      //param1[strlen(param1)] = '\0';
      //Serial.print(strlen(param1));
      if(param1){

      //Serial.println(strlen(param1));
      for(int i =0;i < strlen(param1) ;i++ )
        m_appid[i] = param1[i];
      //strcpy(this->m_appid,param1);
      //Serial.println(m_appid);
      link->out(microgear->connect(m_appid));
      }
      //strcpy(appid,param1);
      //microgear->connect(param1);
      break;
      case MICROGEAR_ISCONNECTED_METHOD:

      link->out(microgear->connected());

      break;
      case MICROGEAR_SETNAME_METHOD:
      microgear->setName(param1);
      link->out(SEND_OK);
      break;
      case MICROGEAR_CHAT_METHOD:
      //Serial.println(this->m_appid);
      microgear->chat((char*)link->getU8_Param1(),(char*)link->getU8_Param2());
      link->out(SEND_OK);

      break;
      case MICROGEAR_LOOP_METHOD:

      microgear->loop();
      link->out(SEND_OK);
      break;
      case MICROGEAR_SETALIAS_METHOD:

      microgear->setAlias(param1);
      link->out(SEND_OK);
      break;
      case MICROGEAR_PUBLISH_METHOD:

      microgear->publish(param1,param2);
      link->out(SEND_OK);
      break;
      case MICROGEAR_SUBSCRIBE_METHOD:

      microgear->subscribe(param1);
      link->out(SEND_OK);
      break;
      case MICROGEAR_UNSUBSCRIBE_METHOD:

      microgear->unsubscribe(param1);
      link->out(SEND_OK);
      break;
      case MICROGEAR_STATE_METHOD:
      link->out(microgear->state());
      break;
      case MICROGEAR_ON_METHOD:

      if(atoi(param1) == MESSAGE)     microgear->on(atoi(param1),onMsghandler);
      if(atoi(param1) == PRESENT)     microgear->on(atoi(param1),onFoundgear);
      if(atoi(param1) == ABSENT)      microgear->on(atoi(param1),onLostgear);
      if(atoi(param1) == CONNECTED)   microgear->on(atoi(param1),onConnected);
      link->out(SEND_OK);

      break;
      case MICROGEAR_INIT_METHOD:

      microgear->init((char*)link->getU8_Param1(),(char*)link->getU8_Param2(),(char*)link->getU8_Param3());
      //microgear->init(KEY,SECRET,ALIAS);
      link->out(SEND_OK);
      break;

      case MICROGEAR_RESET_TOKEN_METHOD:
      microgear->resetToken();
      link->out(SEND_OK);
      break;

    }
    link->setMSG();
    link->clear_param();
}

}
