/*
Project Manito- Brian Gravelle, Chris Birmingham, Collin Walker
bt_phone_v2
This file is a prototype of the software that will be used for communication
  over bluetooth 
*/

#include <SoftwareSerial.h>  

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D3
int IRPin = 8;
//int LED_PIN1 = 3;
//int LED_PIN2 = 9;

int IRout;
boolean connection;
boolean armed;
char btReply[256];
int loopCount;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void sendCmd(char* cmd){
  delay(1000); 
  checkArmed(); //grab message sent in that second (must b/c these delay are most of the time)
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");
  purgeReply(); 
  delay(1000);  // Short delay, wait for the Mate to send back CMD */
  
  bluetooth.println(cmd);
  getReply();  
  bluetooth.println("---"); 
  purgeReply(); 
}

void sendMsg(char* msg){
  delay(50);
  //bluetooth.print("P,");
  bluetooth.println(msg);
  delay(50);
}

void getReply(){
  int i = 0;
  delay(100);
  while(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    btReply[i] = (char)bluetooth.read();  
    i++;
  }
  btReply[i] = '\0';
  Serial.println(btReply);
  delay(100);
}

void purgeReply(){
  int i = 0;
  delay(100);
  while(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    bluetooth.read();  
  }
}

void checkArmed(){
  delay(500);
  getReply();
  if(btReply[0] == 'A'){
    armed = true;
    sendMsg("A");
  }
  if(btReply[0] == 'D'){
    armed = false;
    sendMsg("D");
  }
}

void checkConnection(){
  delay(100);
  sendCmd("GK");  
  if(btReply[0] == '1') connection = true;
  else connection = false;
}

void checkStatus(){
  checkArmed();
  
  //avoid delays that make arming and disarming difficult
  if(loopCount == 20){
    checkConnection();
    loopCount = 0;
  }
  loopCount++;
}

void setup()
{
  
  Serial.println("Start of setup");
  
  pinMode(IRPin, INPUT);

  armed = true;
  connection = false;
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  bluetooth.begin(9600);  
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  // The Bluetooth Mate defaults to 115200bps
  // so be sure to fix you default SU,96

  delay(500);
      
  getReply(); 
  sendCmd("D");
      
  Serial.println("setup complete");
  
}//setup

void loop()
{
  IRout = digitalRead(IRPin);
  checkStatus();
  sendMsg("W");

  if(connection){
    if(armed){
      if(IRout == LOW){
        sendMsg("X");
      }
    }
  }

  //for debugging
  if(Serial.available()){
      bluetooth.print((char)Serial.read());
  }
  
}//loop

