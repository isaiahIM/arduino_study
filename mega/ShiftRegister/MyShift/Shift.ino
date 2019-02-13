void Init_Shift(int dataPin, int clockPin, int latchPin)
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}
void Init_Enable(int enablePin)
{
  pinMode(enablePin, OUTPUT);
}

void ShiftEnable(int enablePin)
{
  digitalWrite(enablePin, 0);
}

void ShiftDisable(int enablePin)
{
  digitalWrite(enablePin, 1);
}

void ShiftByte(int dataPin, int clockPin, int latchPin, int type, uint8_t Data)
{
    uint8_t mask, i;
    
    digitalWrite(latchPin, 0);
    
    if(type==LSB_FIRST)
    {
        mask=0x01;
        for(i=0; i<8; i++)
        {
          digitalWrite(dataPin, Data&mask);
          digitalWrite(clockPin, 1);
          digitalWrite(clockPin, 0);
          mask<<=1;
        }
    }
    
    else if(type==MSB_FIRST)
    {
        mask=0x80;
        for(i=0; i<8; i++)
        {
          digitalWrite(dataPin, Data&mask);
          digitalWrite(clockPin, 1);
          digitalWrite(clockPin, 0);
          mask>>=1;
        }
    }
    digitalWrite(latchPin, 1);
}
