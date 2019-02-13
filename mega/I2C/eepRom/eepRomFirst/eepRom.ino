#define SEQUENCE_FINISH 1
#define SEQUENCE_NOT_FINISH 0

#define SLAVE_RECEVE_DATA 1
#define SLAVE_NO_RECEVE_DATA 2

/* eepRom SCL pulse: 2ms
 * eepRom SDA pulse: 4ms
*/

void Init_eepRom(char Scl, char Sda)
{
  pinMode(Scl, OUTPUT);
  pinMode(Sda, OUTPUT);
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);
}
/*start Signal*/
int EEPRom_Start(char Scl, char Sda, unsigned long curTime, unsigned long *MillisDelay)
{
  if(curTime-(*MillisDelay)<=1)
  {
    gpioA_High(Scl);
  }
  
  //Delay_ms(1);
  else if( curTime-(*MillisDelay)>1 && curTime-(*MillisDelay)<=2)
  {
  gpioA_Low(Sda); 
  }
    //Delay_ms(1);
  else if(curTime-(*MillisDelay)>2 && curTime-(*MillisDelay)<=3)
  {
    gpioA_Low(Scl);
  }
  else
  {
    *MillisDelay=curTime;
//    Serial.println("start");
    return SEQUENCE_FINISH;
  }
  
  return SEQUENCE_NOT_FINISH;
  //Delay_ms(1);
}

/*stop signal*/
int EEPRom_Stop(char Scl, char Sda, unsigned long curTime, unsigned long *MillisDelay)
{
  if(curTime-(*MillisDelay)<=1)
  {
    gpioA_Low(Sda);
  }
  //Delay_ms(1);
  else if(curTime-(*MillisDelay)>1 && curTime-(*MillisDelay)<=2)
  {
    gpioA_High(Scl);
  }
  //Delay_ms(1);
  else if(curTime-(*MillisDelay)>2 && curTime-(*MillisDelay)<=3)
  {
    gpioA_High(Sda);
  }
  //Delay_ms(1);
  else
  {
      *MillisDelay=curTime;
      gpioA_High(Scl);
      return SEQUENCE_FINISH;
  }
  
  return SEQUENCE_NOT_FINISH;
}

/*ACK Signal*/
int EEPRom_ACK(char Scl, char Sda, unsigned long curTime, unsigned long *MillisDelay)
{
  int Ackbit=0;
  if(curTime-(*MillisDelay)<=1)
  {
    gpioA_Low(Sda);
  }
    //Delay_ms(1);
  else if(curTime-(*MillisDelay)>1 && curTime-(*MillisDelay)<=3)
  {
    pinMode(Sda, INPUT);
    gpioA_High(Scl);
    Ackbit+=digitalRead(Sda);
  }
    //Delay_ms(2)
  else 
  {
    pinMode(Sda, OUTPUT);
    gpioA_Low(Scl);
    *MillisDelay=curTime;
    
    if(Ackbit==0)
    {
        return SLAVE_RECEVE_DATA;
    }
    else
    {
      return SLAVE_NO_RECEVE_DATA;
    }
  }
  
  return SEQUENCE_NOT_FINISH;
}
  
/*Write 1bit*/
int EEPRom_WriteByte(char Scl, char Sda, uint8_t Byte, unsigned long curTime, unsigned long *MillisDelay, int MaskCount)
{
  unsigned char mask=0x80;
  
      /*if bit is 1*/
      if( Byte&(mask>>MaskCount) )
      {
          if(curTime-(*MillisDelay)<=1)
          {
            gpioA_High(Sda);
          }
            //Delay_ms(1);
          else if(curTime-(*MillisDelay)>1 && curTime-(*MillisDelay)<=3)
          {
          gpioA_High(Scl);
          }
            //Delay_ms(2);
          else if(curTime-(*MillisDelay)>3 && curTime-(*MillisDelay)<=4)
          {
            gpioA_Low(Scl);
            *MillisDelay=curTime;
            return SEQUENCE_FINISH;
          }
      }
      
      /*if bit is 0*/
      else
      {
          if(curTime-(*MillisDelay)<=1)
          {
            gpioA_Low(Sda);
          }
          //Delay_ms(1);
          else if(curTime-(*MillisDelay)>1 && curTime-(*MillisDelay)<=3)
          {
            gpioA_High(Scl);
          }
            //Delay_ms(2);
          else if(curTime-(*MillisDelay)>3 && curTime-(*MillisDelay)<=4)
          {
            gpioA_Low(Scl);
            *MillisDelay=curTime;
            return SEQUENCE_FINISH;
          }
      }
      //Delay_ms(1);
      
      return SEQUENCE_NOT_FINISH;
}
