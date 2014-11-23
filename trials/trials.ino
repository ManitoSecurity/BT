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
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
int IRPin = 8;
int IRout;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  delay(2000);
  pinMode(IRPin, INPUT);
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  Serial.println("Start of setup");
  delay(1500);
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  delay(1500); 
  bluetooth.write("$$$");  // Print three times individually
  //bluetooth.write("$");
  //bluetooth.write("$");  // Enter command mode
  delay(1500);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  Serial.println("did stuff");
}

void loop()
{
  IRout = digitalRead(IRPin);
  //Serial.println(IRout);
  if(IRout == LOW){
    bluetooth.println("I spy with my little eye...");
    delay(2000); //wait a sec for kicks
  }
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    //Serial.println("Stuff was in the bluetooth que");
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());//(char)  
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    Serial.println("Stuff was in the serial moniter");
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());//(char)
  }
  // and loop forever and ever!
}

