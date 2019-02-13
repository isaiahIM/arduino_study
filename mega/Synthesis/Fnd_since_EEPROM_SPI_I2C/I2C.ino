void Init_eepRom(char Scl, char Sda)
{
  pinMode(Scl, OUTPUT);
  pinMode(Sda, OUTPUT);
  gpioA_High(Sda);
  gpioA_High(Scl);
}

/*start Signal*/
void I2C_Start(char Scl, char Sda, uint32_t timeCnt)
{
  if(timeCnt==0)
  {
    gpioA_Low(Scl);
//    gpioA_High(Sda); 
  }
  
  //Delay_ms(1);
  else if(timeCnt==1)
  {
    gpioA_High(Sda); 
  }
    //Delay_ms(1);
  else if(timeCnt==2)
  {
    gpioA_High(Scl);
  }
  else 
  {
    gpioA_Low(Sda);
  }
}

/*stop signal*/
void I2C_Stop(char Scl, char Sda, uint32_t timeCnt)
{
  if(timeCnt==0)
  {
    gpioA_Low(Scl);
  }
  //Delay_ms(1);
  else if(timeCnt==1)
  {
    gpioA_Low(Sda);
  }
  //Delay_ms(1);
  else if(timeCnt==2)
  {
    gpioA_High(Scl);
  }
  
  else if(timeCnt==3)
  {
  gpioA_High(Sda);
  }
  //Delay_ms(1);
}


/*ACK Signal*/
int I2C_ACK(uint8_t Scl, uint8_t Sda, uint32_t timeCnt)
{
  int ackState;
  
  if(timeCnt==0)
  {
    gpioA_Low(Scl);
  }
    //Delay_ms(1);
  else if(timeCnt==1)
  {
    gpioA_Low(Sda);
    pinMode(Sda, INPUT);
  }
    //Delay_ms(2)
  else if(timeCnt==2)
  {
    gpioA_High(Scl);
    ackState=digitalRead(Sda);
//    pinMode(Sda, OUTPUT);
  
  
    if(ackState==0)
    {
        return SLAVE_RECEVE_DATA;
    }
    else
    {
        Serial.println(ackState);
        Serial.println("no ack");
        return SLAVE_NO_RECEVE_DATA;
    }
  }
  else if(timeCnt==4)
  {
    gpioA_Low(Scl);
    pinMode(Sda, OUTPUT);
  }
  return -1;
  
}

  
/*Write 1bit*/
void I2C_WriteBit(uint8_t Scl, uint8_t Sda, uint8_t Byte, uint32_t timeCnt, int MaskCount)
{ 
      if(timeCnt==0)
      {
        gpioA_Low(Scl);
      }

      else if(timeCnt==2)
      {
        gpioA_High(Scl);
      }
      
      /*if bit is 1*/
      if( Byte&(0x80>>MaskCount) )
      {
          if(timeCnt==1)
          {
            gpioA_High(Sda);
          }
            //Delay_ms(1);
      }
      
      /*if bit is 0*/
      else
      {
          if(timeCnt==1)
          {
            gpioA_Low(Sda);
          }
          //Delay_ms(1);
      }
      //Delay_ms(1);
}

int I2C_WriteByte(uint8_t scl, uint8_t sda, uint8_t data, uint32_t clockeCnt, int *bitCount)
{
  if(clockeCnt>=4)
  {
    (*bitCount)++;
  }

  I2C_WriteBit(scl, sda, data, clockeCnt, *bitCount );

  if( (*bitCount)>=8)
  {
    *bitCount=0;
    return SEQUENCE_FINISH;// return 1
  }
  return SEQUENCE_NON_FINISH;// return 0
}



int I2C_ReadBit(uint8_t Scl, uint8_t Sda, uint32_t timeCnt)
{
  if(timeCnt>=0 && timeCnt<2)
  {
    gpioA_Low(Scl);
    gpioA_Low(Sda);
   
    pinMode(Sda, INPUT);
  }
  
  else if(timeCnt==2)
  {
    gpioA_High(Scl);
//    Serial.println(digitalRead(Sda));
    return digitalRead(Sda);
  }
  else
  {
      gpioA_Low(Scl);
      pinMode(Sda, OUTPUT);
      gpioA_Low(Sda);
  }
  return -1;
}

int I2C_ReadByte(uint8_t scl, uint8_t sda, uint32_t clockCnt, int *bitCount, volatile uint8_t *readData)
{
  if(clockCnt>=4)
  {
    (*bitCount)++;

    if( (*bitCount)<=7)
    {
      (*readData)<<=1;
    }
  }

  if(I2C_ReadBit(scl, sda, clockCnt)!=-1)
  {
    (*readData)|=I2C_ReadBit(scl, sda, clockCnt);
  }

  if( (*bitCount)>=8)
  {
    *bitCount=0;
    return SEQUENCE_FINISH;// return 1
  }
  
  return SEQUENCE_NON_FINISH;// return 0
}


