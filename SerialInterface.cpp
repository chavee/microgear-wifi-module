

#include "SerialInterface.h"




SerialInterface::SerialInterface(HardwareSerial* mySerial,WiFiClient* wifi_client)
{

      indexMSG  = 0;
      msgIncome = false;
      inSerial  = mySerial;
      client = wifi_client;
      *incomeBuffer = 0;
      *outBuffer = 0;
      //Serial.println("Start Interface....");
}

void SerialInterface::begin(int buad)
{
    (*inSerial).begin(buad);
    (*inSerial).println("Start Interface");
}

void SerialInterface::loopMSG()
{
    if(isMSG()){
      // Check wifi
      serialWifi();
      serialClient();

    }


    //(*inSerial).println("loop");
}
void SerialInterface::clearBuffer()
{
  *incomeBuffer = 0;
  *outBuffer    = 0;
}
bool SerialInterface::isMSG()
{


  while ((*inSerial).available())
  {
         //----- read the incoming char and combine to String MSG:
         uint8_t c = (*inSerial).read();
         incomeBuffer[indexMSG++] = c;
         //incomeBuffer += c;
         if(c == '\n') msgIncome = true;  // ----- Income MSG enable when char = '\n' -----
  }

  if(msgIncome && !(*inSerial).available())
  {
          msgIncome = false;
          indexMSG =0;
          //Serial.println((char*)incomeBuffer);
          if(!strcmp(strtok((char*)incomeBuffer,";"),ESP_HEADER)){
            command = strtok(NULL,";");
            return true;
          }
          else return false;
  }



}
// --------- Define Method
uint8_t SerialInterface::isMethod()
{
  method = strtok(NULL,";");
  if (!strcmp(method,WIFI_BEGIN)) return WIFI_BEGIN_METHOD;
  if(!strcmp(method,WIFI_IP)) return WIFI_IP_METHOD;
  if(!strcmp(method,WIFI_STATUS)) return WIFI_STATUS_METHOD;

  if (!strcmp(method,CLIENT_CONNECT)) return CLIENT_CONNECT_METHOD;
  if (!strcmp(method,CLIENT_WRITE)) return CLIENT_WRITE_METHOD;
  if (!strcmp(method,CLIENT_READ)) return CLIENT_READ_METHOD;
  if (!strcmp(method,CLIENT_AVAILABLE)) return CLIENT_AVAILABLE_METHOD;
  if (!strcmp(method,CLIENT_CONNECTED)) return CLIENT_CONNECTED_METHOD;
  if (!strcmp(method,CLIENT_STOP)) return CLIENT_STOP_METHOD;
}

//---------- out put MSG function ---------------
void SerialInterface::out(char* outmsg)
{
  *outBuffer = 0;
  strcpy((char*)outBuffer,OUT_HEADER);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,command);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,method);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,outmsg);
  strcat((char*)outBuffer,";\n");
  //sprintf((char*)outBuffer,"%s;%s;%s;\n",OUT_HEADER,command,outmsg);
  (*inSerial).print((char*)outBuffer);
  *incomeBuffer = 0;
  *outBuffer = 0;
}

void SerialInterface::out(size_t value)
{
      char buffer[10];
      out(itoa(value,buffer,10));
}

void SerialInterface::out(int value)
{
    char buffer[10];
      out(itoa(value,buffer,10));
}

void SerialInterface::out(bool value)
{
  if(value) out("1");
  else out("0");
}


void SerialInterface::wifiIP()
{
  sprintf((char*)outBuffer,"%s;%s;%s;",OUT_HEADER,command,method);

  (*inSerial).print((char*)outBuffer);
  (*inSerial).print(WiFi.localIP());
  (*inSerial).print(";\n");
}


//---------- out put MSG function ---------------
void SerialInterface::wifiBegin()
{
  char *new_ssid     = strtok(NULL,";");
  char *new_password = strtok(NULL,";");
  /*
  if (WiFi.begin(new_ssid, new_password)) {
    //(*inSerial).println("WIFI Connecting..");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        //(*inSerial).print(".");
    }
  }*/
  out(WiFi.begin(new_ssid, new_password));
  //if(WiFi.status()== WL_CONNECTED) out(WiFi.status());
  //else out("error");
}

//---------- out put MSG function ---------------
void SerialInterface::wifiStatus()
{

  out(WiFi.status());
  }

// --------------- WIFI method ------------------//
bool SerialInterface::isWifi()
{

  return (!strcmp(command,WIFI_COMMAND));
}

bool SerialInterface::isClient()
{

  return (!strcmp(command,CLIENT_COMMAND));
}





// --------------- WIFI manage ------------------//
void SerialInterface::serialWifi()
{

  if(isWifi())
  {
      switch (isMethod())
      {
        case WIFI_BEGIN_METHOD:
        wifiBegin();
        break;
        case WIFI_IP_METHOD:
        wifiIP();
        break;
        case WIFI_STATUS_METHOD:
        wifiStatus();
        break;
      }


  }


}

// --------------- client manage ------------------//
void SerialInterface::clientConnect()
{
  char *host     = strtok(NULL,";");
  uint16_t port   = atoi(strtok(NULL,";"));
  out(client->connect(host,port));
}
void SerialInterface::clientAvaliable()
{
  out(client->available());
}
void SerialInterface::clientConnected()
{
    out(client->connected());
}
void SerialInterface::clientWrite()
{
  char *http     = strtok(NULL,";");


  String recheck = String(http);
  char key_return[3];
  char key_newline[3];
  key_return[0]  = 0x5c;
  key_return[1]  = 0x72;
  key_return[2]  = '\0';
  key_newline[0] =  0x5c;
  key_newline[1] =  0x6e;
  key_newline[2]  = '\0';
  //Serial.println(key_return);
  //Serial.println(key_newline);
  recheck.replace(key_return, "\r");
  recheck.replace(key_newline, "\n");
  //Serial.println(recheck);


  char http_write[200];
  recheck.toCharArray( http_write, 200);
  uint8_t len = strlen(http_write) ;

  #ifdef TEST
      char *ip     = "ga.netpie.io";
      uint16_t port   = 8080;
      client->connect(ip,port);
  #endif

out(client->write((const uint8_t *)http_write,len));
  #ifdef TEST
delay(10);
while(1){
 Serial.print((char)client->read());

 if(!client->available()) break;
 }
 #endif

}

void SerialInterface::clientRead()
{
  out((char)client->read());
}

void SerialInterface::clientStop()
{
  client->stop();
  //out(1);
}

void SerialInterface::serialClient()
{

  if(isClient())
  {
      switch (isMethod())
      {
        case CLIENT_CONNECT_METHOD:
        clientConnect();
        break;
        case CLIENT_AVAILABLE_METHOD:
        clientAvaliable();
        break;
        case CLIENT_CONNECTED_METHOD:
        clientConnected();
        break;
        case CLIENT_WRITE_METHOD:
        clientWrite();
        break;
        case CLIENT_READ_METHOD:
        clientRead();
        break;
        case CLIENT_STOP_METHOD:
        clientStop();
        break;

      }


  }


}
