
//#include <SerialInterface.h>
#include <ESP8266WiFi.h>

#include "UARTLink.h"
#include "UARTManager.h"
#include <Microgear.h>

//const char* ssid     = "ROOM_G";
//const char* password = "74107410";

//#define ESP_HEADER "ESP"
WiFiClient client;
UARTLink uart(&Serial);

MicroGear microgear(client);
/*
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {

  msg[msglen] = '\0';
  Serial.print("Incoming message -->");
  Serial.println((char*)msg);
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setName("mbed");
}
*/
UARTManager esp_manage(&uart);
void setup() {
    //Serial.begin(115200);
    //sInterface.begin();
    uart.begin();

    //microgear.on(MESSAGE,onMsghandler);
    //microgear.on(PRESENT,onFoundgear);
    //microgear.on(ABSENT,onLostgear);
    //microgear.on(CONNECTED,onConnected);
    //microgear.init(KEY,SECRET,"");

    esp_manage.initMicroGear(&microgear);
    //microgear.setName("mbed");
    //microgear.connect("remote");
  // put your setup code here, to run once:
    //Serial.begin(115200);
    //Serial.println("Starting...");

    /*
    if (WiFi.begin(ssid, password)) {

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
    }
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    */
}


void loop() {
  // put your main code here, to run repeatedly:
  //uart.loop();
  while(1) esp_manage.manage();
  //sInterface.(*inSerial).println("loop");
/*
   while (Serial.available())
        {
          //----- read the incoming char and combine to String MSG:
           byte c = Serial.read();
           input_buffer[indexMSG++] = c;

           if(c == '\n') msgIncome = true;  // ----- Income MSG enable when char = '\n' -----

        }

  if(msgIncome && !Serial.available())
  {

    msgIncome = false;
    indexMSG =0;

    char * header = strtok((char*)input_buffer,";");


    if (!strcmp(header,ESP_HEADER)) // exposure time setting.
    {
        char * command = strtok(NULL,";");
        //Serial.println(command);
        if (!strcmp(command,"wifi"))
        {
        char * method = strtok(NULL,";");
            Serial.println(method);
            if (!strcmp(method,"begin"))
            {
              char *new_ssid     = strtok(NULL,";");
              char *new_password = strtok(NULL,";");
              //Serial.println(new_ssid);
              //Serial.println(new_password);
              if (WiFi.begin(new_ssid, new_password)) {
                Serial.println("WIFI Connecting..");
                while (WiFi.status() != WL_CONNECTED) {
                    delay(500);
                    Serial.print(".");
               }
               Serial.println("WiFi connected");
              Serial.println("IP address: ");
              Serial.println(WiFi.localIP());
              }
            }
        }
        else if(!strcmp(command,"client")){
           char * method = strtok(NULL,";");
            //Serial.println(method);
            if (!strcmp(method,"connect"))
            {
              char *ip     = strtok(NULL,";");
              uint16_t port   = atoi(strtok(NULL,";"));

              Serial.println(client.connect(ip,port));

            }

            if (!strcmp(method,"available"))
            {


              Serial.println(client.available());

            }

            if (!strcmp(method,"write"))
            {

              char *http     = strtok(NULL,";");
              uint8_t len = strlen(http) ;

                //client.write((char*)http,len);

               char timestr[200];
               //strcpy(timestr,http);
               //strcpy(timestr,"GET /api/time HTTP/1.1\r\n\r\n");
                char *ip     = "ga.netpie.io";
              uint16_t port   = 8080;

              Serial.println(http);
              //Serial.println(len);
              //Serial.println(timestr);
              //Serial.println(strlen(timestr));
              client.connect(ip,port);
              client.write((const uint8_t *)http,len);




               delay(10);
               while(1){
                Serial.print((char)client.read());

                if(!client.available()) break;
                }
            }

            if (!strcmp(method,"read"))
            {

                while(client.available()){
                Serial.print(client.read());
                }
            }

        }





    }




    *input_buffer =0;
  }
  */

}
