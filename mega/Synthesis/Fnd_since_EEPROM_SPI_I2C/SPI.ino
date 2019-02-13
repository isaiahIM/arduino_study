/*init SPI data*/
void Init_SPI(int clk, int mosi, int miso, int cs)
{
  pinMode(clk, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(miso, INPUT);
  pinMode(cs, OUTPUT);
  
  digitalWrite(clk,0);
  digitalWrite(mosi,0);
  digitalWrite(cs, 1);
}

/*SPI Chip Select control*/
void SPI_Enable(int cs)
{
  digitalWrite(cs, 0);
}

/*SPI Chip Select control*/
void SPI_Disable(int cs)
{
  digitalWrite(cs, 1);
}

/*send 1bit for clock(4 clock is 1bit)*/
volatile void SPI_WriteBit(int clk, int mosi, uint8_t Bit, uint32_t clockCount)
{
  if(clockCount==0)
   {
     digitalWrite(clk, LOW);
   }
    
  if(Bit!=0)
  { 
    if(clockCount==1)
    {
      digitalWrite(mosi, HIGH);
    }
  }
  else
  {
    if(clockCount==1)
    {
      digitalWrite(mosi, LOW);
    }
  }
  
  if(clockCount==2)
  {
    digitalWrite(clk, HIGH);
  }
  
  else if(clockCount==3)
  {
    digitalWrite(clk, LOW);
  }
  else if(clockCount>3)
  {
    digitalWrite(mosi, LOW);
  }
}

/*send 1byte*/ 
int SPI_WriteByte(int clk, int mosi, uint8_t data, uint32_t clockCount, int *bitCount)
{ 
  if(clockCount>=4)
  {
    (*bitCount)++;
  }
  
  SPI_WriteBit(clk, mosi, data&(0x80>>(*bitCount)), clockCount);
  
  if( (*bitCount)>=8)
  {
    *bitCount=0;
    return SEQUENCE_FINISH;// return 1
  }
  
  return SEQUENCE_NON_FINISH;// return 0
}

/*read 1bit for clock(4clock is 1bit)*/
int SPI_ReadBit(int clk, int mosi, int miso, uint32_t clockCount)
{
  if(clockCount==2)
  {
      digitalWrite(clk, HIGH);
  }
  
  else if(clockCount==3)
  {
      digitalWrite(clk, LOW);  
      //digitalWrite(mosi, HIGH);
      
      if(digitalRead(miso)!=0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
  }
  else if(clockCount>3)
  {
    //digitalWrite(mosi, LOW);
  }
  return -1;
}

int SPI_ReadByte(int clk, int mosi, int miso, uint32_t clockCount, int *bitCount, volatile uint8_t *readData)
{
  if(clockCount>=4)
  {
    (*bitCount)++;

    if( (*bitCount)<=7)
    {
      (*readData)<<=1;
    }
  }
  
  if(SPI_ReadBit(clk, mosi, miso, clockCount)!=-1)
  {
  (*readData)|=SPI_ReadBit(clk, mosi, miso, clockCount);
//  Serial.println(*readData);
  }

  if( (*bitCount)>=8)
  {
    *bitCount=0;
//    Serial.println();
    
    (*readData)>>=1;
//    Serial.println(*readData);
    return SEQUENCE_FINISH;// return 1
  }
  
  return SEQUENCE_NON_FINISH;// return 0
}
