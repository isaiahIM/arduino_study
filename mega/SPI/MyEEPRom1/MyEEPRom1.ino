#include<SPI.h>

/*SPI Port Information*/
#define CLK 52
#define MISO 50
#define MOSI 51
#define CS 49

/*SPI eeprom Register*/
#define SPI_WREN 0x06// write enable
#define SPI_WRDI 0X04// reset write enable
#define SPI_RDSR 0X05// read status Register
#define SPI_WRSR 0X01// write status Register
#define SPI_READ 0X03// read mode
#define SPI_WRITE 0X02// write mode


#define READ_MODE 2
#define WRITE_MODE 3

enum SPI_WriteSequence
{
  SPI_ENABLE_W,
//  SPI_STATUS_W,
//  SPI_ADDRESS_MODE_W,
  SPI_MODE_W,
  SPI_WORD_ADDRESS_W,
  SPI_WRITE_DATA_W
};// write sequence
enum SPI_ReadSequence
{
  SPI_ENABLE_R,
  SPI_READ_R,
  SPI_WORD_ADDRESS_R,
  SPI_READ_DATA_R
};// read sequence

static uint32_t curTime, prevTime=0, timeCount=0, writeSpeed;
static uint8_t wordAddr, write_data, protect_mode=0x06;
volatile static uint8_t readVal=0;
static int firstLoop=1, spi_wiriteTask=0, spi_readTask=0, bitCount=0, writeState=0, readState=0, sequenceNextState=0;
static int mode=0;

char serialData;
void setup() {
  // put your setup code here, to run once:
Init_SPI(CLK, MOSI, MISO, CS);
SPI_Disable(CS);

Serial.begin(9600);

wordAddr=0x00;
write_data=0x00;
writeSpeed=50;// 50us clock
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if(Serial.available())
  {
    serialData=Serial.read();

    if(serialData=='w')
    {
      mode=WRITE_MODE;
    }
    
    else if(serialData=='r')
    {
      mode=READ_MODE;
    }
    
    if(serialData>='0' && serialData<='9')
    {
      write_data*=10;
      write_data+=ConvAskiiToNum(serialData);
      Serial.println(write_data);
    }
  }
  else
  {
    if(mode==WRITE_MODE)
    {
      writeState=1;
      readState=0;
    }
    
    else if(mode==READ_MODE)
    {
      readState=1;
      writeState=0;
    }
    mode=0;
  }
  
  
  
  
  curTime=micros();
  
  if(timeCount>=4)
  {
    timeCount=0;
  }
  
  if(curTime-prevTime>=writeSpeed ||firstLoop==1)
  {
    prevTime=curTime;
    timeCount++;
    
    if(firstLoop==1)
    {
      timeCount=0;
      firstLoop=0;
    }
  }

    /*Write Sequence*/
  if(writeState==1)
  {
    SPI_Enable(CS);
    
    if(spi_wiriteTask==SPI_ENABLE_W)
    {
      sequenceNextState=SPI_WriteByte(CLK, MOSI, SPI_WREN, timeCount, &bitCount);
      
      if(sequenceNextState==1)
      {
        spi_wiriteTask+=1;
        sequenceNextState=0;
        SPI_Disable(CS);
//        Serial.println("Write Enable");
      }
    }

//    else if(spi_wiriteTask==SPI_STATUS_W)
//    { 
//      sequenceNextState=SPI_WriteByte(CLK, MOSI, SPI_WRSR, timeCount, &bitCount);
//      
//      if(sequenceNextState==1)
//      {
////        Serial.println("Write Status Register");
//        spi_wiriteTask+=1;
//        sequenceNextState=0;
//      }
//    }
//
//     else if(spi_wiriteTask==SPI_ADDRESS_MODE_W)
//     {
//       sequenceNextState=SPI_WriteByte(CLK, MOSI, protect_mode, timeCount, &bitCount);
//
//      if(sequenceNextState==1)
//      {
//        SPI_Disable(CS);
////        Serial.println("Write Status Register2");
//        spi_wiriteTask+=1;
//        sequenceNextState=0;
//      }
//     }
    
    else if(spi_wiriteTask==SPI_MODE_W)
    {
      sequenceNextState=SPI_WriteByte(CLK, MOSI, SPI_WRITE, timeCount, &bitCount);
      
      if(sequenceNextState==1)
      {
        spi_wiriteTask+=1;
        sequenceNextState=0;
//        Serial.println("Write Mode");
      }
    }
    
    else if(spi_wiriteTask==SPI_WORD_ADDRESS_W)
    {
      sequenceNextState=SPI_WriteByte(CLK, MOSI, wordAddr, timeCount, &bitCount);
      
      if(sequenceNextState==1)
      {
        spi_wiriteTask+=1;
        sequenceNextState=0;
//        Serial.println("Word Address");
      }
    }
    
    else if(spi_wiriteTask==SPI_WRITE_DATA_W)
    {
      sequenceNextState=SPI_WriteByte(CLK, MOSI, write_data, timeCount, &bitCount);
      
      if(sequenceNextState==1)
      {
        spi_wiriteTask=SPI_ENABLE_W;
        sequenceNextState=0;
        SPI_Disable(CS);
        Serial.println("Write Data");
        firstLoop=1;
        writeState=0;
        write_data=0;
      }
    }
  }

  else if(readState==1)
  {
    SPI.begin();
    
    SPI_Enable(CS);
    
    SPI.transfer(SPI_READ);
    SPI.transfer(wordAddr);
    readVal = SPI.transfer(0x00);
    
    SPI_Disable(CS);
    
    Serial.print("Reading value : ");
    Serial.println(readVal);
    SPI.end();
    readState=0;
  }
  
}
