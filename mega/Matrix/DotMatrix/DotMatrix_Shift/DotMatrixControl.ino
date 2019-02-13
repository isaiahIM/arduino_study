void InitMatrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    
    for(i=0; i<Matrix_Size; i++)
    {
        pinMode(Matrix_Row[i], OUTPUT);
        pinMode(Matrix_Col[i], OUTPUT);
        
        digitalWrite(Matrix_Row[i], POWER_OFF);
        digitalWrite(Matrix_Col[i], POWER_ON);
    }
}

void DisplayFont(int Matrix_Row[], int Matrix_Col[], int Matrix_Size, unsigned char Font[], unsigned long TimeCount) 
{
    int j;
    //unsigned long Time1, Time2;

    //Time1=micros();
        ClearMatrix(Matrix_Row, Matrix_Col, Matrix_Size);
        
        for(j=0; j<Matrix_Size; j++)
        {
            if(Font[j]&(0x80>>TimeCount) )
            {   
                digitalWrite(Matrix_Row[j], POWER_ON);
            }
        } 
        digitalWrite(Matrix_Col[TimeCount], POWER_OFF);
        
   //Time2=micros();
    //delay(100);
    //Serial.println(Time2-Time1);
}

void DisplayIndividual(int Matrix_Row[], int Matrix_Col[], int Display_Col, int Display_Row)
{
    ClearMatrix(Matrix_Row, Matrix_Col, 8);
    digitalWrite(Matrix_Row[Display_Row], POWER_ON);
    digitalWrite(Matrix_Col[Display_Col], POWER_OFF);  
}

void ClearMatrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    for(i=0; i<Matrix_Size; i++)
    {
        digitalWrite(Matrix_Row[i], POWER_OFF);
        digitalWrite(Matrix_Col[i], POWER_ON);
    }
}

void ShiftFont(int FontSize, const unsigned char OriginalFont[], unsigned char ShiftFont[], int dir, int FontLoopState)
{
    int i;
    unsigned char ChainBit=0x00;
    
    if(dir==SHIFT_RIGHT)
    {
       for(i=0; i<FontSize; i++)
       {
          if(FontLoopState==SHIFT_LOOP)
          {
              ChainBit=OriginalFont[i]&0x01;
              ChainBit<<=7;
              ShiftFont[i]= OriginalFont[i]>>1;
              
              ShiftFont[i]|=ChainBit;
          }
          else
          {
            ShiftFont[i]= OriginalFont[i]>>1;
          }
       }
    }
    
    else if(dir==SHIFT_LEFT)
    {
       for(i=0; i<FontSize; i++)
       {
          if(FontLoopState==SHIFT_LOOP)
          {
              ChainBit=OriginalFont[i]&0x80;
              ChainBit>>=7;
              ShiftFont[i]= OriginalFont[i]<<1;
              
              ShiftFont[i]|=ChainBit;
          }
          else
          {
            ShiftFont[i]= OriginalFont[i]<<1;
          }
       }
    }
}

void CloneFont(int FontSize, const unsigned char OriginalFont[], unsigned char ShiftFont[])
{
  int i;
  for(i=0; i<FontSize; i++)
  {
      ShiftFont[i]=OriginalFont[i];
  }
}
