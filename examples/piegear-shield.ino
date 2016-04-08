#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <MicroGear.h>
#include <UARTLink.h>
#include <UARTManager.h>

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
