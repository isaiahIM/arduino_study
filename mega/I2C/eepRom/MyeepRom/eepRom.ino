
#define SLAVE_RECEVE_DATA 1
#define SLAVE_NO_RECEVE_DATA 2

void Init_eepRom(char Scl, char Sda)
{
  pinMode(Scl, OUTPUT);
  pinMode(Sda, OUTPUT);
  gpioA_High(Sda);
  gpioA_High(Scl);
}

/*start Signal*/
void EEPRom_Start(char Scl, char Sda, uint32_t timeCnt)
{
  if(timeCnt==0)
  {
    gpioA_High(Sda);
    gpioA_High(Scl);
  }
  
  //Delay_ms(1);
  else if(timeCnt==1)
  {
  gpioA_Low(Sda); 
  }
    //Delay_ms(1);
  else if(timeCnt==2)
  {
    gpioA_Low(Scl);
  }
}

/*stop signal*/
void EEPRom_Stop(char Scl, char Sda, uint32_t timeCnt)
{
  if(timeCnt==0)
  {
    gpioA_Low(Scl);
    gpioA_Low(Sda);
  }
  //Delay_ms(1);
  else if(timeCnt==1)
  {
    gpioA_High(Scl);
  }
  //Delay_ms(1);
  else if(timeCnt==2)
  {
    gpioA_High(Sda);
  }
  //Delay_ms(1);
}


/*ACK Signal*/
int EEPRom_ACK(uint8_t Scl, uint8_t Sda, uint32_t timeCnt, uint8_t *ackState)
{
  if(timeCnt==0)
  {
    gpioA_Low(Scl);
    gpioA_Low(Sda);
//    gpioA_High(Scl);
  }
    //Delay_ms(1);
  else if(timeCount==1)
  {
    pinMode(Sda, INPUT);
    gpioA_High(Scl);
    *ackState=digitalRead(Sda);
  }
    //Delay_ms(2)
  else 
  {
    pinMode(Sda, OUTPUT);
    gpioA_Low(Scl);
    gpioA_Low(Sda);
    
    if(*ackState==0)
    {
        return SLAVE_RECEVE_DATA;
    }
  }
  
}

  
/*Write 1bit*/
void EEPRom_WriteByte(uint8_t Scl, uint8_t Sda, uint8_t Byte, uint32_t timeCnt, int MaskCount)
{
  unsigned char mask=0x80;
  
      /*if bit is 1*/
      if( Byte&(mask>>MaskCount) )
      {
          if(timeCnt==0)
          {
            gpioA_Low(Scl);
          }
          else if(timeCount==1)
          {
            gpioA_High(Sda);
          }
            //Delay_ms(1);
          else if(timeCnt==2)
          {
          gpioA_High(Scl);
          }
            //Delay_ms(2);
      }
      
      /*if bit is 0*/
      else
      {
          if(timeCnt==0)
          {
            gpioA_Low(Scl);
          }

          else if(timeCount==1)
          {
            gpioA_Low(Sda);
          }
          //Delay_ms(1);
          else if(timeCnt==2)
          {
            gpioA_High(Scl);
          }
            //Delay_ms(2);
      }
      //Delay_ms(1);
}

