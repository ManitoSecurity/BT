/*
  Example Bluetooth Serial Passthrough Sketch
 by: Jim Lindblom
 SparkFun Electronics
 date: February 26, 2013
 license: Public domain

 This example sketch converts an RN-42 bluetooth module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and bluetooth module.
 */
 
 /*
 Heavily modified by Brian Gravelle and Chris Birmingham
 will commmunicate with phone
 sends "detection" when IR sensors reports movement
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D3
int IRPin = 8;
int IRout;
boolean connection;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  connection = true;
  
  delay(2000);
  pinMode(IRPin, INPUT);
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  Serial.println("Start of setup");
  delay(1500);
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps
                          // so be sure to fix you default SU,96
  delay(1500); 
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");
  delay(1500);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("D");  
  delay(1000);
  //bluetooth.println("---");  
  
  while(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    //Serial.println("Stuff was in the bluetooth que");
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());//(char)  
  }
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  
  bluetooth.println("---"); 
  
  Serial.println("did stuff");
}

void loop()
{
  IRout = digitalRead(IRPin);  
  /*
  delay(1500); 
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");
  delay(1500);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("GK");  
  int i = 0;
  while(bluetooth.available()>0 && i < 5)  // If the bluetooth sent any characters
  {
    if(i = 0){ 
      if((char)bluetooth.read() == '1')
        connection = true;
      else
        connection = false;
    } else{
      bluetooth.read(); 
    }
  }
  delay(1500); 
  bluetooth.println("---");
  delay(1500);
  */
  if(connection){
    if(IRout == LOW){
      bluetooth.println("ho ho ho");
      delay(10000); //wait 10 secs for kicks
    }
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
  
      Serial.print((char)bluetooth.read());//(char)  
    }
    if(Serial.available())  // If stuff was typed in the serial monitor
    {
      Serial.println("Stuff was in the serial moniter");
      // Send any characters the Serial monitor prints to the bluetooth
      bluetooth.print((char)Serial.read());//(char)
    }
  }
  // and loop forever and ever!
}

