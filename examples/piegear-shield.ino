
#include <ESP8266WiFi.h>

#include "UARTLink.h"
#include "UARTManager.h"
#include <Microgear.h>

WiFiClient client;
UARTLink uart(&Serial);

MicroGear microgear(client);

UARTManager esp_manage(&uart);
void setup() {
    uart.begin();



    esp_manage.initMicroGear(&microgear);
}


void loop() {
  while(1) esp_manage.manage();
}
