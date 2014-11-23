#include <SoftwareSerial.h>

int lightB = 7;
int lightG = 6;
int lightR = 5;
int masterPin = 4;
int isMaster = 0;
int msgToken = 0;

int rxpin = 3;
int txpin = 5;

int incomingByte = 0;
char gBtMsg[256];
char gBTAdr[13];
char gBtCmd[256];

int gBtKnownMACTotal = 2;
char* gBtKnownMAC[2]; //This is set to hold only two MAC adresses

SoftwareSerial btSerial(rxpin, txpin);


void BtReceive(void){
  SdLog("recieving");  
  
  bool keepReading = true;
  int index = 0;
  
  gBtMsg[0] = '\0';
  
  while(keepReading){
    keepReading = false;
    if (btSerial.available() > 0) {
      SdLog("btserial available");
      // read the incoming byte:
      incomingByte = btSerial.read();
      SdLog("got some shit");
      if(incomingByte != 13){
        gBtMsg[index++] = (char)incomingByte;
        keepReading = true;
      }
    }
  }
  
  gBtMsg[index] = '\0';
}

void BtSend(char* i_pBtCmd, bool i_ln = true){

  SdLog("command:");
  SdLog(i_pBtCmd);
  
  if(i_ln){
    btSerial.println(i_pBtCmd);
  } else{
    btSerial.print(i_pBtCmd);
  }
  
  delay(100);
  BtReceive();
  
  //Debug start
//  SdLog("\\");
//  SdLog(i_pBtCmd);
//  SdLog(gBtMsg);
//  SdLog("/");
  //Debug end
}

void SdLog(char* shit){
  Serial.println(shit);
  
}

void BtInit(void){ 
  bool btConnect = false;

  gBtKnownMAC[0] = "000666412345"; // Change this to one of your MAC addresses
  gBtKnownMAC[1] = "000666454321"; // and this one too
  
  isMaster = digitalRead(masterPin);
  
  Serial.begin(115200);
  btSerial.begin(115200); 
  BtSend("$$$", false);
  
  if(isMaster == LOW){
    SdLog(">>>> Set To Master <<<<");

    BtSend("SM,1");

    BtSend("I,10");
    BtSend("---"); 
   
    delay(5000);
    
    while(!btConnect){
      
      SdLog(">>>> connecting <<<<");
      delay(100);
      BtReceive();
      int msgLen = strlen(gBtMsg);
      if(msgLen > 0){
        
        SdLog(">>>> IDK <<<<");
        
        if(msgLen >= 12){
          
          SdLog(">>>> found somthing <<<<");
          char* doneMsg = &gBtMsg[msgLen - 12];
          
          gBtMsg[12] = '\0';
          bool foundKnownMAC = false;
          
          for(int i = 0; i < gBtKnownMACTotal && !foundKnownMAC; i++){
            if(!strcmp(gBtMsg, gBtKnownMAC[i])){
              foundKnownMAC = true;
            }
          }
          
          if(!strcmp(doneMsg, "Inquiry Done")){
            digitalWrite(lightB,HIGH);

            SdLog(doneMsg);
            
            if(foundKnownMAC){              
              gBtCmd[0] = 'C';
              gBtCmd[1] = ',';
              
              for(int i = 0; i < 12; i++){
                gBtCmd[i + 2] = gBtMsg[i];
              }
              
              gBtCmd[15] = '\0';
                  
              BtSend("$$$", false);
              BtSend(gBtCmd);
              BtSend("---");      

              delay(2000);
              digitalWrite(lightG,HIGH);
              
              while(!btConnect){
                delay(1000);
                BtSend("$$$", false);
                BtSend("GK");
                
                int numVal = 0;
      
                if(strlen(gBtMsg) > 0) {
                  numVal= atoi(gBtMsg);
                }
                
                if(numVal == 1){
                  btConnect = true;
                  SdLog("Is connected !!!!!!");
                }
                BtSend("---");
              }
            }
          }
        }
      }
    }
    
    digitalWrite(lightG,LOW);
    digitalWrite(lightB,LOW);
    msgToken = 1;
  } else{
    SdLog(">>>> Set To Slave <<<<");
    BtSend("SM,0");
    BtSend("---"); 
    
    while(!btConnect){
      delay(1000);

      BtSend("$$$", false);
      BtSend("GK");
      
      int numVal = 0;
      
      if(strlen(gBtMsg) > 0) {
        numVal= atoi(gBtMsg);
      }
      
      if(numVal == 1){                  
        btConnect = true;
        SdLog("Is connected !!!!!!");
      }

      BtSend("---");              
    }
  }
  
  SdLog("#### end ####");
  digitalWrite(lightR,LOW);
}

void setup() {
  pinMode(lightR, OUTPUT);
  pinMode(lightG, OUTPUT);
  pinMode(lightB, OUTPUT);
  pinMode(isMaster, INPUT);
  pinMode(txpin, OUTPUT);
  pinMode(rxpin, INPUT);
    
  
  BtInit();
}

void receiveMsg(){  
  for(int i = 0; i < 10; i++){
    if (btSerial.available() > 0) {
      // read the incoming byte:
      incomingByte = btSerial.read();
      if(incomingByte == 'R'){
        digitalWrite(lightB, LOW);
        delay(50);
        digitalWrite(lightB,HIGH);
      }
    }
    
    delay(100);
  }
}

void sendMsg(){
  digitalWrite(lightG,LOW);  
  delay(50);  
  btSerial.print("R");
  digitalWrite(lightG,HIGH);
  delay(50);  
  btSerial.print("R");
  digitalWrite(lightG,LOW);  
  delay(50);  
  digitalWrite(lightG,HIGH);
  btSerial.print("R");
}

void loop() 
{  
  if(msgToken == 1){
    digitalWrite(lightG,HIGH);
    sendMsg();
    delay(1000);
  }else{
    digitalWrite(lightB,HIGH);
    receiveMsg();
  }
}

