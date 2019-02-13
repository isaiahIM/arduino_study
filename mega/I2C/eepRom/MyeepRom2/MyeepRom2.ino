
enum EEPRom_WriteTask
{
W_START,
W_DEVICE_ADDRESS,
W_ACK1,
W_WORD_ADDRESS,
W_ACK2,
W_DATA,
W_ACK3,
W_STOP  
};

enum EEPRom_ReadTask
{
R_START,
R_DEVICE_ADDRESS,
R_ACK1,
R_WORD_ADDRESS,
R_ACK2,
R_ReStart,
R_DEVICE_ADDRESS2,
R_ACK3,
R_READ_DATA,
R_SEND_ACK,
R_STOP  
};

#define SCL 21
#define SDA 20
#define READ_MODE 2
#define WRITE_MODE 3

#define SLAVE_RECEVE_DATA 1
#define SLAVE_NO_RECEVE_DATA 2


static uint32_t curTime, prevTime=0, timeCount=0;
static int eepRom_WriteTask=0, eepRom_ReadTask=0, byteCheck=0;

static uint8_t DeviceAddr_w, DeviceAddr_r, WordAddr, Data=0, ackState=0;
static int writeFlag=0, readFlag=0, first=1, readData=0;
static char readSerial, mode=0;

void setup()
{
  Serial.begin(9600);
  DeviceAddr_w= 0xA0;
  DeviceAddr_r= 0xA1;
  
  WordAddr= 0x01;
  Init_eepRom(SCL, SDA);
  Delay_ms(5);
}
 

void loop()
{
      curTime = micros();

      if(timeCount>=4)
      {
        timeCount=0;
      } 
      
      if(curTime-prevTime>=500 || first==1)
      {
          prevTime=curTime;
          if(first==1)
          {
            first=0;
            timeCount=0;
          }
          else
          {
            timeCount+=1;
          }
      }
  
  if (Serial.available() > 0) 
  {
    first=1;
    readSerial=Serial.read();

    if(readSerial=='s')
    {
      Serial.println("Write");
      mode=WRITE_MODE;
      writeFlag=1;
      readFlag=0;
    }
    
    else if(readSerial=='r')
    {
      Serial.println("Read");
      mode=READ_MODE;
      readFlag=1;
      writeFlag=0;
    }
    
    if(mode==WRITE_MODE && readSerial!='s')
    {
      Data*=10;
      Data+=ConvAskiiToNum(readSerial);
      Serial.println(Data);
    }
  }


  if(mode==WRITE_MODE && writeFlag==1)
  {     
      /*send Start bit*/
      if(eepRom_WriteTask==W_START)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask+=1;
        }
        
        EEPRom_Start(SCL, SDA, timeCount);
      }

      /*send Device address*/
      else if(eepRom_WriteTask==W_DEVICE_ADDRESS)
      { 
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, DeviceAddr_w, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask+=1;
          byteCheck=0;
//          Serial.println("Device");
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask==W_ACK1)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive DEVICE ADDRESS!!\nRetry again!");
          eepRom_WriteTask-=1;
          timeCount=0;
        }
      }

      /*send Word address*/
      else if(eepRom_WriteTask==W_WORD_ADDRESS)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, WordAddr, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask+=1;
          byteCheck=0;
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask==W_ACK2)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive WORD ADDRESS!!\nRetry again!");
          eepRom_WriteTask-=1;
          timeCount=0;
        }
      } 

      /*send Data*/
      else if(eepRom_WriteTask==W_DATA)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, Data, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_WriteTask+=1;
          byteCheck=0;
//          Serial.println("Data");
        }
      }

      /*receive ACK*/
      else if(eepRom_WriteTask==W_ACK3)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive DATA!!\nRetry again!");
          eepRom_WriteTask-=1;
          timeCount=0;
        }
      }

      
      else if(eepRom_WriteTask==W_STOP)
      {
        if(timeCount>=4)
        {
          eepRom_WriteTask+=1;
          timeCount=0;
          writeFlag=0;
        }
        
        EEPRom_Stop(SCL, SDA, timeCount);
      }   
}
 


  else if(mode==READ_MODE && readFlag==1)
  {
      /*send Start bit*/
      if(eepRom_ReadTask==R_START)
      {
        if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
        }
        
        EEPRom_Start(SCL, SDA, timeCount);
      }

      /*send Device address*/
      else if(eepRom_ReadTask==R_DEVICE_ADDRESS)
      { 
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, DeviceAddr_w, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_ReadTask+=1;
          byteCheck=0;
//          Serial.println("Device");
        }
      }

      /*receive ACK*/
      else if(eepRom_ReadTask==R_ACK1)
      {
        if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive DEVICE ADDRESS!!\nRetry again!");
          eepRom_ReadTask-=1;
          timeCount=0;
        }
      }

      /*send Word address*/
      else if(eepRom_ReadTask==R_WORD_ADDRESS)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, WordAddr, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_ReadTask+=1;
          byteCheck=0;
//          Serial.println("Word");
        }
      }

      /*receive ACK*/
      else if(eepRom_ReadTask==R_ACK2)
      {
        if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive WORD ADDRESS!!\nRetry again!");
          eepRom_ReadTask-=1;
          timeCount=0;
        } 
      } 

      /*send ReStart bit*/
      else if(eepRom_ReadTask==R_ReStart)
      {
        if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
        }
        
        EEPRom_Start(SCL, SDA, timeCount);
      }

      /*send Device address*/
      else if(eepRom_ReadTask==R_DEVICE_ADDRESS2)
      { 
        if(timeCount>=4)
        {
          byteCheck+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, DeviceAddr_r, timeCount, byteCheck);
        
        if(byteCheck>=8)// 8bit loop
        {
          eepRom_ReadTask+=1;
          byteCheck=0;
//          Serial.println("Device");
        }
      }

      /*receive ACK*/
      else if(eepRom_ReadTask==R_ACK3)
      { 
        if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
          ackState=0;
        }
        
        ackState=EEPRom_ACK(SCL, SDA, timeCount);

        if(ackState==SLAVE_NO_RECEVE_DATA)
        {
          Serial.println("Slave module not receive WORD ADDRESS!!\nRetry again!");
          eepRom_ReadTask-=1;
          timeCount=0;
        } 
      }

      else if(eepRom_ReadTask==R_READ_DATA)
      {
        if(timeCount>=4)
        {
          byteCheck+=1;
          readData<<=1;
        }

        if(EEPRom_ReadBit(SCL, SDA, timeCount) !=-1)
        {
          readData|= EEPRom_ReadBit(SCL, SDA, timeCount);
        }
        
        if(byteCheck>=8)
        {
          eepRom_ReadTask+=1;
          byteCheck=0;
          readData>>=1;
        }
      }
      
      else if(eepRom_ReadTask==R_SEND_ACK)
      {
         if(timeCount>=4)
        {
          eepRom_ReadTask+=1;
        }
        
        EEPRom_WriteByte(SCL, SDA, 0x80, timeCount, 0);
      }
      
      else if(eepRom_ReadTask==R_STOP)
      {
        if(timeCount>=4)
        { 
          Serial.print("ReadValue:");
          Serial.println(readData);

          Init_eepRom(SCL, SDA);
          readFlag=0;
          mode=0;
          Data=0;
          readData=0;
          eepRom_ReadTask=R_START;
          eepRom_WriteTask=W_START;
        }
        
        EEPRom_Stop(SCL, SDA, timeCount);
      }   
  }   
} 
