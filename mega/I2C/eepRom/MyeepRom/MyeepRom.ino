#include <Wire.h>

enum EEPRom_WriteSequence
{
Seq_START,
Seq_DEVICE_ADDRESS,
Seq_ACK1,
Seq_WORD_ADDRESS,
Seq_ACK2,
Seq_DATA,
Seq_ACK3,
Seq_STOP  
};

//enum EEPRom_ReadSequence
//{
//Seq_START=0,
//Seq_DEVICE_ADDRESS,
//Seq_ACK1,
//Seq_WORD_ADDRESS,
//Seq_ACK2,
//Seq_ReStart,
//Seq_DEVICE_ADDRESS2,
//Seq_ACK3,
//Seq_DATA,
//Seq_STOP  
//};

#define SCL 21
#define SDA 20
#define READ_MODE 2
#define WRITE_MODE 3

#define SLAVE_RECEVE_DATA 1
#define SLAVE_NO_RECEVE_DATA 2


static uint32_t curTime, prevTime=0, timeCount=0;
static int eepRom_WriteTask[8]={0, }, eepRom_ReadTask[8]={0, }, byteCheck=0, maskcnt;

static uint8_t DevceAddr_w, WordAddr, Data=0, ackState=0;
static int i=0, writeFlag=0, readFlag=0, first=1;
static char readSerial, mode=0;

void setup()
{
  Serial.begin(9600);
  
  DevceAddr_w= 0xA0;
  WordAddr= 0x01;
  Init_eepRom(SCL, SDA);
  Delay_ms(5);
}
 

void loop()
{
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
  else
  {
    writeFlag=1;
    readFlag=1;
  }


  if(mode==WRITE_MODE && writeFlag==1)
  {
      writeFlag=1;

      curTime = micros();
 
      if(timeCount==4)
      {
        timeCount=0;
      } 
      
      if(curTime-prevTime>=50 || first==1)
      {
        first=0;
        prevTime=curTime;
        
        timeCount+=1;
        
      }
      
      /*send Start bit*/
      if(eepRom_WriteTask[0]==0)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask[0]=1;
        }
        
        EEPRom_Start(SCL, SDA, timeCount);
        
        if(eepRom_WriteTask[0]==1)
        {
//          Serial.println("start");
        }
      }

      /*send Device address*/
      else if(eepRom_WriteTask[0]==1 && eepRom_WriteTask[1]==0)
      { 
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, DevceAddr_w, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask[1]=1;
          byteCheck=0;
//          Serial.println("Device");
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask[1]==1 && eepRom_WriteTask[2]==0 )
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask[2]=1;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount, &ackState);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive DEVICE ADDRESS!!\nRetry again!");
          eepRom_WriteTask[1]=0;
          timeCount=0;
        }
        
        if(eepRom_WriteTask[2]==1)
        {
          ackState=0;
//          Serial.println("ACK");
        }
      }

      /*send Word address*/
      else if(eepRom_WriteTask[2]>=1 && eepRom_WriteTask[3]==0)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, WordAddr, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask[3]=1;
          byteCheck=0;
//          Serial.println("Word");
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask[3]==1 && eepRom_WriteTask[4]==0)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask[4]=1;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount, &ackState);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive WORD ADDRESS!!\nRetry again!");
          eepRom_WriteTask[3]=0;
          timeCount=0;
        }
        
        if(eepRom_WriteTask[4]==1)
        {
        ackState=0;
//        Serial.println("ACK");
        }
      } 

      /*send Data*/
      else if(eepRom_WriteTask[4]>=1 && eepRom_WriteTask[5]==0)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, Data, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask[5]=1;
          byteCheck=0;
//          Serial.println("Data");
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask[5]==1 && eepRom_WriteTask[6]==0)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask[6]=1;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount, &ackState);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive DATA!!\nRetry again!");
          eepRom_WriteTask[5]=0;
          timeCount=0;
        }
        
        if(eepRom_WriteTask[6]>=1)
        {
        ackState=0;
//        Serial.println("ACK");
        }
      }

      
      else if(eepRom_WriteTask[6]>=1 && eepRom_WriteTask[7]==0)
      {
        if(timeCount>=4)
        eepRom_WriteTask[7]=1;
        
        EEPRom_Stop(SCL, SDA, timeCount);
        
        if(eepRom_WriteTask[7]==1)
        {
          //readSerial=0;
//          Serial.println("stop");
          first=1;
        }
      }   
}
 


  else if(mode==READ_MODE)
  {
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
  
      writeFlag=1;
      readFlag=1;
      mode=0;
      Data=0;
      readSerial=0;
      timeCount=0;
      
      for(i=0; i<8; i++)
      {
        eepRom_WriteTask[i]=0;
        eepRom_ReadTask[i]=0;
      }
      
  }
    
}
