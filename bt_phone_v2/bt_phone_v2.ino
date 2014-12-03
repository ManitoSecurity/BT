/*
Project Manito- Brian Gravelle and Chris Birmingham
This file is a prototype of the software that will be used for communication
  over bluetooth 
*/

#include <SoftwareSerial.h>  

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D3
int IRPin = 8;

int IRout;
boolean connection;
boolean armed;
char btReply[256];

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void sendCmd(char* cmd){
  delay(1000); 
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");
  delay(1000);  // Short delay, wait for the Mate to send back CMD
  getReply(); 
  delay(100);
  bluetooth.println(cmd);
  getReply();  
  bluetooth.println("---"); 
  purgeReply(); 
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
  delay(100);
}

void purgeReply(){
  int i = 0;
  delay(100);
  while(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    bluetooth.read();  
  }
  delay(100);
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
    
  Serial.println("setup complete");
  
}//start

void loop()
{
  IRout = digitalRead(IRPin);  
  btReply[0] = 'N';  
  btReply[1] = 'O';  
  btReply[2] = '\0';
  
  sendCmd("GK");
  
  if(btReply[0] == '1') connection = true;
  else connection = false;

  if(connection && armed){
    if(IRout == LOW){
      bluetooth.println("detection");
      delay(1000); //wait a sec for kicks
    }
  }
  
  delay(500);
  getReply();
  if(btReply[0] == 'A') armed = true;
  if(btReply[0] == 'D') armed = false;
  Serial.println(btReply);

  if(Serial.available()){
      bluetooth.print((char)Serial.read());
  }
  
}//loop

