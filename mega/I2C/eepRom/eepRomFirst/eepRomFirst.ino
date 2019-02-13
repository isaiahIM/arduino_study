#include <Wire.h>

#define SCL 21
#define SDA 20
#define READ_MODE 2
#define WRITE_MODE 3

static unsigned long currentMillis, delayTime=0, previousMillis0=0;
static int eepRom_sequence[8]={0, }, byteCheck=0;

static uint8_t DevceAddr_w, WordAddr, Data=0;
static int i=0, first=1;
static char readSerial, mode=0, a=1;

void setup()
{
  Serial.begin(9600);
  
  DevceAddr_w= 0xA0;
  WordAddr= 0x01;
  
  Init_eepRom(SCL, SDA);
  gpioA_High(SDA);
  gpioA_High(SCL);
  Delay_ms(5);
}
 

void loop()
{
  currentMillis = CheckMillis();


  if (Serial.available() > 0) 
  {
    readSerial=Serial.read();

    if(readSerial=='s')
    {
      Serial.println("Write");
      mode=WRITE_MODE;
    }
    

    else if(readSerial=='r')
    {
      mode=READ_MODE;
    }
    
    if(mode==WRITE_MODE && readSerial!='s')
    {
      Data*=10;
      Data+=ConvAskiiToNum(readSerial);
      Serial.println(Data);
    }
  }


  if(mode==WRITE_MODE)
  {
    
  if(first==1)
  {
    delayTime=currentMillis;
    Init_eepRom(SCL, SDA);
    first=0;
  }
  
  if(eepRom_sequence[0]==0)
  {
    eepRom_sequence[0]= EEPRom_Start(SCL, SDA, currentMillis, &delayTime);
//    Serial.println("start");
  }
    
  if(eepRom_sequence[0]==1 && eepRom_sequence[1]==0)
  {
    byteCheck+=EEPRom_WriteByte(SCL, SDA, DevceAddr_w, currentMillis, &delayTime, byteCheck);
    
    if(byteCheck>=8)// 8bit loop
    {
      eepRom_sequence[1]=1;
      byteCheck=0;
//      Serial.println("Device");
    }
  }
  
  if(eepRom_sequence[1]==1 && eepRom_sequence[2]==0 )
  {
    eepRom_sequence[2]=EEPRom_ACK(SCL, SDA, currentMillis, &delayTime);
  }

  if(eepRom_sequence[2]==1 && eepRom_sequence[3]==0)
  {
    byteCheck+=EEPRom_WriteByte(SCL, SDA, WordAddr, currentMillis, &delayTime, byteCheck);
    
    if(byteCheck>=8)// 8bit loop
    {
      eepRom_sequence[3]=1;
      byteCheck=0;
//      Serial.println("Word");
//      Serial.println(byteCheck);
    }
  }
  
  if(eepRom_sequence[3]==1 && eepRom_sequence[4]==0)
  {
    eepRom_sequence[4]=EEPRom_ACK(SCL, SDA, currentMillis, &delayTime);
  } 
  
  if(eepRom_sequence[4]==1 && eepRom_sequence[5]==0)
  {
    byteCheck+=EEPRom_WriteByte(SCL, SDA, Data, currentMillis, &delayTime, byteCheck);
    
    if(byteCheck>=8)// 8bit loop
    {
//      Serial.println("Data");
      eepRom_sequence[5]=1;
      byteCheck=0;
    }
  }
  
  if(eepRom_sequence[5]==1 && eepRom_sequence[6]==0)
  {
    eepRom_sequence[6]=EEPRom_ACK(SCL, SDA, currentMillis, &delayTime);
  }
  
  if(eepRom_sequence[6]==1 && eepRom_sequence[7]==0)
  {
    eepRom_sequence[7]=EEPRom_Stop(SCL, SDA, currentMillis, &delayTime);
    
    if(eepRom_sequence[7]==1)
    {
      readSerial=0;
      mode=0;
//        Serial.println("Write Finish!");
    }
  }
  }

  else if(mode==READ_MODE)
  {
      first=1;
      mode=0;
      Data=0;
      readSerial=0;
      
      for(i=0; i<8; i++)
      {
        eepRom_sequence[i]=0;
      }
      
      Wire.begin();
      
      Wire.beginTransmission(0x50); // transmit to device address 0xa0 -> right shift 1 -> 0x50
      Wire.write(0x01);        // send data address
      Wire.endTransmission();    // stop transmitting
      
      Wire.requestFrom(0x50, 1);    // request 1 bytes from slave device address

      while (Wire.available())   // slave may send less than requested
      {
        char c = Wire.read(); // receive a byte as character
        Serial.print("i2c read data : ");
        Serial.print((int)c);         // print the character
        Serial.print("\n");
      }
      Serial.print("i2c read end\n");
      
      Wire.end();
      
      Init_eepRom(SCL, SDA);
//      gpioA_High(SDA);
//      gpioA_High(SCL);
  

      
  }
    
}
