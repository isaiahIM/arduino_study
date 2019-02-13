/*Key matrix*/
void Init_KeyMatrix(int Key_col, int Key_row, int col[], int row[], int MatrixType)
{
  int i;
  
    if(MatrixType==KEY_MATRIX_COLUMN_BASE)
    {
        for(i=0; i<Key_col; i++)
        {
            pinMode(col[i], OUTPUT);
        }
        for(i=0; i<Key_row; i++)
        {
            pinMode(row[i], INPUT);
        }
         
    }
    
    else if(MatrixType==KEY_MATRIX_ROW_BASE)
    {
        for(i=0; i<Key_col; i++)
        {
            pinMode(col[i], INPUT);
        }
        for(i=0; i<Key_row; i++)
        {
            pinMode(row[i], OUTPUT);
        }
    }
}


int ReadKey(int Key_col, int Key_row, int col[], int row[], int MatrixType, int *GetCol, int *GetRow)
{
    int i, j, key_State=KEY_NON_CLICK;
    
    if(MatrixType==KEY_MATRIX_COLUMN_BASE)
    {
        for(i=0; i<Key_col; i++)
        {
            for(j=0; j<Key_col; j++)
            {
                if(i==j)
                {
                  digitalWrite(col[j], KEY_POWER_ON);
                }
                else
                {
                  digitalWrite(col[j], KEY_POWER_OFF);
                }
            }

            for(j=0; j<Key_row; j++)
            {
                key_State=digitalRead(row[j]);
                
                if(key_State==KEY_CLICK)
                {
                    *GetRow=j;
                    *GetCol=i;
//                  Serial.println(i);
//                  Serial.println(j);
                    return (j*Key_col)+(i+1);
                } 
            }
            
        }// end of loop i
        return -1;
    }

    else if(MatrixType==KEY_MATRIX_ROW_BASE)
    {
        for(i=0; i<Key_row; i++)
        {
            for(j=0; j<Key_row; j++)
            {
                if(i==j)
                {
                  digitalWrite(row[j], KEY_POWER_ON);
                }
                else
                {
                  digitalWrite(row[j], KEY_POWER_OFF);
                }
            }

            for(j=0; j<Key_col; j++)
            {
                key_State=digitalRead(col[j]);
                
                if(key_State==KEY_CLICK)
                {
                    *GetRow=i;
                    *GetCol=j;
                    
                    return (j*Key_row)+(i+1);
                } 
            }
            
        }// end of loop i
        return -1;
    }
}

String ConvKeyToNum(int KeyData)
{
   int Data;
   
   switch(KeyData)
   {
       case 1:
       return "C";
       break;
       
       case 2:
       return "()";
       break;

       case 3:
       return "%";
       break;
       
       case 4:
       return"/";
       break;

       case 8:
       return "*";
       break;

       case 12:
       return "-";
       break;

       case 16:
       return "+";
       break;

       case 17:
       return ".";
       break;

       case 19:
       return "+/-";
       break;

       case 20:
       return "=";
       break;
   }
   
   if(KeyData==18)
   {
       Data=0;
   }
   
   else if(KeyData>12 && KeyData<16)
   {
       Data=KeyData-12;
   }
   
   else if(KeyData>8 && KeyData<12)
   {
       Data=KeyData-5;
   }
   
   else if(KeyData>4 && KeyData<8)
   {
       Data= KeyData+2;
   }
   
   return (String)Data;
} 

int ConvStringToInt(String string)
{
    char num;
    int i=0, Num=0;
    
    while(string[i]!='\0')
    {
        num=string[i];
        i++;
        Num*=10;
        Num+=num-48;
    }
    return Num;
}


/*Dot matrix*/
void Init_Matrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    
    for(i=0; i<Matrix_Size; i++)
    {
        pinMode(Matrix_Row[i], OUTPUT);
        pinMode(Matrix_Col[i], OUTPUT);
        
        digitalWrite(Matrix_Row[i], DOTMATRIX_POWER_OFF);
        digitalWrite(Matrix_Col[i], DOTMATRIX_POWER_ON);
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
                digitalWrite(Matrix_Row[j], DOTMATRIX_POWER_ON);
            }
        } 
        digitalWrite(Matrix_Col[TimeCount], DOTMATRIX_POWER_OFF);
        
   //Time2=micros();
    //delay(100);
    //Serial.println(Time2-Time1);
}

void DisplayIndividual(int Matrix_Row[], int Matrix_Col[], int Display_Col, int Display_Row)
{
    ClearMatrix(Matrix_Row, Matrix_Col, 8);
    digitalWrite(Matrix_Row[Display_Row], DOTMATRIX_POWER_ON);
    digitalWrite(Matrix_Col[Display_Col], DOTMATRIX_POWER_OFF);  
}

void ClearMatrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    for(i=0; i<Matrix_Size; i++)
    {
        digitalWrite(Matrix_Row[i], DOTMATRIX_POWER_OFF);
        digitalWrite(Matrix_Col[i], DOTMATRIX_POWER_ON);
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

    else if(dir==SHIFT_DOWN)
    {
        unsigned char Buffer, Temp;
        
        if(FontLoopState==SHIFT_LOOP)
        {
            Temp=OriginalFont[FontSize-1];
        }
        
        for(i=FontSize-1; i>=0; i--)
        {        
          Buffer=OriginalFont[i-1];
          ShiftFont[i]= Buffer;
        }
        
        ShiftFont[0]=Temp;
    }
    
    else if(dir==SHIFT_UP)
    {
        unsigned char Buffer, Temp;

        if(FontLoopState==SHIFT_LOOP)
        {
            Temp=OriginalFont[0];
        }
               
       for(i=0; i<FontSize; i++)
       {
           Buffer=OriginalFont[i+1];
           ShiftFont[i]= Buffer;
       }
       ShiftFont[7]=Temp;
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

