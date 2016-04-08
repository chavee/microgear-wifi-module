
#include "UARTLink.h"


UARTLink::UARTLink(HardwareSerial* mySerial)
{

      indexMSG  = 0;
      msgIncome = false;
      inSerial  = mySerial;
      *incomeBuffer = 0;
      *outBuffer = 0;
      isMSG = false;
      prev_char1 = ' ';
      prev_char2 = ' ';
      param1 = NULL;
      param2 = NULL;
      param3 = NULL;
}

uint8_t * UARTLink::getU8_Param1()
{
  return u8_param1;
}
uint8_t * UARTLink::getU8_Param2()
{
  return u8_param2;
}
uint8_t * UARTLink::getU8_Param3()
{
  return u8_param3;
}
size_t  UARTLink::getU8_Size()
{
  return write_size;
}
void UARTLink::split_byte()
{
  //msg:ESP;client;write;␐,␀␄MQTT␄�␀␏test_espoheofmvh␀␌tsnqXoscRSoC;46;
  //Serial.print("msg:");
  uint8_t semicolon_check = 0 ;
  uint8_t index_u8_param1 = 0;
  uint8_t index_u8_param2 = 0;
  Serial.print("msg size:");
  Serial.print(indexMSG);
  Serial.print(" ");
  for (int i = 0; i < indexMSG; i++) {
    uint8_t byte = incomeBufferBackup[i];
    //Serial.print((char)byte);


    if(semicolon_check == 3) u8_param1[index_u8_param1++] = byte;
    if(semicolon_check == 4) u8_param2[index_u8_param2++] = byte;
    if(byte == ';') semicolon_check++;
  }
 //Serial.println((char)incomeBuffer[i]);
  u8_param1[index_u8_param1] = '\0';
  u8_param2[index_u8_param2] = '\0';
  write_size = atoi((char*) u8_param2);
  /*
  Serial.println(write_size);

  for (int i = 0; i < write_size; i++) {

    Serial.print((char)u8_param1[i]);
  }



  Serial.print("\n");
*/
}

void UARTLink::split_byte3()
{
  //msg:ESP;client;write;␐,␀␄MQTT␄�␀␏test_espoheofmvh␀␌tsnqXoscRSoC;46;
  //Serial.print("msg:");
  uint8_t semicolon_check = 0 ;
  uint8_t index_u8_param1 = 0;
  uint8_t index_u8_param2 = 0;
  uint8_t index_u8_param3 = 0;
  Serial.print("msg size:");
  Serial.print(indexMSG);
  Serial.print(" ");
  for (int i = 0; i < indexMSG; i++) {
    uint8_t byte = incomeBufferBackup[i];
    //Serial.print((char)byte);


    if(semicolon_check == 3) u8_param1[index_u8_param1++] = byte;
    if(semicolon_check == 4) u8_param2[index_u8_param2++] = byte;
    if(semicolon_check == 5) u8_param3[index_u8_param3++] = byte;
    if(byte == ';') semicolon_check++;
  }
 //Serial.println((char)incomeBuffer[i]);
  u8_param1[index_u8_param1-1] = '\0';
  u8_param2[index_u8_param2-1] = '\0';
  u8_param3[index_u8_param3-1] = '\0';
  //write_size = atoi((char*) u8_param2);

  //Serial.println(write_size);

  for (int i = 0; i < index_u8_param1; i++)
    Serial.print((char)u8_param1[i]);
  Serial.print("\n");
  for (int i = 0; i < index_u8_param2; i++)
    Serial.print((char)u8_param2[i]);
  Serial.print("\n");
  for (int i = 0; i < index_u8_param3; i++)
    Serial.print((char)u8_param3[i]);
  Serial.print("\n");

}

void UARTLink::begin(int baud )
{
    (*inSerial).begin(baud);
    (*inSerial).println("\nStart Interface");
}

void UARTLink::loop()
{

  while ((*inSerial).available())
  {
         //----- read the incoming char and combine to String MSG:
         uint8_t c = (*inSerial).read();
         //Serial.print((char)c);
         if(indexMSG < MAX_BUFFER_SIZE-1){
           incomeBufferBackup[indexMSG] = c;
           incomeBuffer[indexMSG++] = c;

         }
         //else msgIncome = true;
         if(indexMSG)  prev_char1 = incomeBuffer[indexMSG-2];

         //Serial.println((char)prev_char1,DEC);
         //incomeBuffer += c;
         if(prev_char1 == ';' && c == '\n') msgIncome = true;  // ----- Income MSG enable when char = '\n' -----

  }

  //if(msgIncome && !(*inSerial).available())
  if(msgIncome)
  {
          msgIncome = false;
          prev_char1 = ' ';
          prev_char2 = ' ';

          //Serial.println((char*)incomeBuffer);
          /*
          memset(param1, 0, sizeof(param1));
          memset(param2, 0, sizeof(param2));
          memset(param3, 0, sizeof(param3));
          */

          count_para = 0;
          if(!strcmp(strtok((char*)incomeBuffer,";"),ESP_HEADER)){

            command = strtok(NULL,";");
            method = strtok(NULL,";");
            if(!strcmp(method,"write"))
            {
              split_byte();
            }
            else if(!strcmp(method,"init"))
            {
              split_byte3();
            }
            else if(!strcmp(method,"chat"))
            {
              split_byte3();
            }
            /*
            else if(!strcmp(method,"microgear_connect"))
            {
              split_byte3();
            }*/
            else{


            param1 = strtok(NULL,";");
            param2 = strtok(NULL,";");
            param3 = strtok(NULL,";");



            }


            //content = strtok(NULL,"\n");
            //return true;
            //Serial.println((char*)command);
            //Serial.println((char*)method);
            //Serial.println(param1);
            //Serial.println(param2);
            //Serial.println(param3);
            isMSG = true;

          }
          indexMSG =0;

  }

}

void UARTLink::setMSG()
{
    isMSG = false;
}

//---------- out put MSG function ---------------
void UARTLink::out(char* outmsg)
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
//---------- out put MSG function ---------------
void UARTLink::out(char* event_method,char* outmsg)
{
  *outBuffer = 0;
  strcpy((char*)outBuffer,OUT_HEADER);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,command);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,event_method);
  strcat((char*)outBuffer,";");
  strcat((char*)outBuffer,outmsg);
  strcat((char*)outBuffer,";\n");
  //sprintf((char*)outBuffer,"%s;%s;%s;\n",OUT_HEADER,command,outmsg);
  (*inSerial).print((char*)outBuffer);
  *incomeBuffer = 0;
  *outBuffer = 0;
}
void UARTLink::out(IPAddress ip)
{
  char ipaddr[16];
  byte oct1 = ip[0];
  byte oct2 = ip[1];
  byte oct3 = ip[2];
  byte oct4 = ip[3];
  sprintf(ipaddr, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
  //Serial.print(ipaddr);
  out(ipaddr);
}

void UARTLink::out(size_t value)
{
      char buffer[10];
      out(itoa(value,buffer,10));
}

void UARTLink::out(int value)
{
    char buffer[10];
      out(itoa(value,buffer,10));
}

void UARTLink::out(bool value)
{
  if(value) out(1);
  else out(0);
}


bool UARTLink::isInbox()
{
  return isMSG;
}
uint8_t UARTLink::getParamCount()
{
  return count_para;
}

char* UARTLink::getParam1()
{

    return (param1);
}

char* UARTLink::getParam2()
{

    return (param2);
}


char* UARTLink::getParam3()
{

    return (param3);
}

char* UARTLink::getMethod()
{
  return method;
}

void UARTLink::clear_param()
{

  if(param1)
  param1[0] = '\0';
  if(param2)
  param2[0] = '\0';
  if(param3)
  param3[0] = '\0';

}
