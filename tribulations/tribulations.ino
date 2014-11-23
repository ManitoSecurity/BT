
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  //pinMode(bluetoothTx, OUTPUT);
  //pinMode(bluetoothRx, INPUT);
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  delay(1000);
  bluetooth.begin(9600); 
  //bluetooth.listen();
  delay(1000);
  Serial.println("did stuff");
}

void loop()
{
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    Serial.println("Stuff was in the bluetooth que");
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());  
  }
  delay(1000);
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    Serial.println("Stuff was in the serial moniter");
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
    //Serial.read();
    //bluetooth.print("hi");
  }
  delay(1000);
  // and loop forever and ever!
}

