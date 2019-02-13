#define SEQUENCE_FINISH 1
#define SEQUENCE_NON_FINISH 0

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
  if(Bit!=0)
  {
    if(clockCount==0)
    {
      digitalWrite(mosi, HIGH);
    }
    
    else if(clockCount==1)
    {
      digitalWrite(clk, HIGH);
    }
  }
  else
  {
    if(clockCount==0)
    {
      digitalWrite(mosi, LOW);
    }
    
    else if(clockCount==1)
    {
      digitalWrite(clk, HIGH);
    }
  }
  
  if(clockCount==2)
  {
    digitalWrite(clk, LOW);
  }
  
  else if(clockCount==3)
  {
    digitalWrite(mosi, LOW);
  }
}

/*send 1byte*/ 
volatile int SPI_WriteByte(int clk, int mosi, uint8_t data, uint32_t clockCount, int *bitCount)
{ 
  if(clockCount>=4)
  {
//    Serial.println(data&(0x80>>(*bitCount)));
//    Serial.println((*bitCount));
//    Serial.println();
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
int SPI_ReadBit(int clk, int miso, uint32_t clockCount)
{
  if(clockCount==0)
  {
      digitalWrite(clk, HIGH);
  }
  
  else if(clockCount==3)
  {
      digitalWrite(clk, LOW);  
      
      if(digitalRead(miso)!=0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
  }
  return -1;
}
