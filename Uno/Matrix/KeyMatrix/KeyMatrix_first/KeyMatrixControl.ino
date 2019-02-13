void InitKeyMatrix(int MatrixSize, int col[], int row[], int MatrixType)
{
  int i;
  
    if(MatrixType==KEY_MATRIX_COLUMN_BASE)
    {
        for(i=0; i<MatrixSize; i++)
        {
            pinMode(col[i], OUTPUT);
            pinMode(row[i], INPUT);
        }
    }
    
    else if(MatrixType==KEY_MATRIX_ROW_BASE)
    {
        for(i=0; i<MatrixSize; i++)
        {
            pinMode(row[i], OUTPUT);
            pinMode(col[i], INPUT);
        }
    }
}


int ReadKey(int MatrixSize, int col[], int row[], int MatrixType)
{
    int i, j, key_State=KEY_NON_CLICK;
    
    if(MatrixType==KEY_MATRIX_COLUMN_BASE)
    {
        for(i=0; i<MatrixSize; i++)
        {
            for(j=0; j<MatrixSize; j++)
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

            for(j=0; j<MatrixSize; j++)
            {
                key_State=digitalRead(row[j]);
                
                if(key_State==KEY_CLICK)
                {
                  
//                    Serial.println(i);
//                    Serial.println(j);
                    return (j*MatrixSize)+(i+1);
                }
                
            }
        }
        return -1;
    }

    else if(MatrixType==KEY_MATRIX_ROW_BASE)
    {
        for(i=0; i<MatrixSize; i++)
        {
            for(j=0; j<MatrixSize; j++)
            {
                if(i==j)
                {
                  digitalWrite(row[j], KEY_POWER_ON);
                }
                else
                {
                  digitalWrite(row[j], KEY_POWER_OFF);
                }
                key_State=digitalRead(col[j]);
                
                if(key_State==KEY_CLICK)
                {
                    return (j*MatrixSize)+(i+1);
                }
            }
        }
    }
    
}

char ConvKeyToNum(int KeyData)
{
   int Data;
   
   switch(KeyData)
   {
       case 4:
       return'A';
       break;

       case 8:
       return 'B';
       break;

       case 12:
       return 'C';
       break;

       case 13:
       return '*';
       break;

       case 15:
       return '#';
       break;

       case 16:
       return 'D';
       break;
   }
   if(KeyData>12)
   {
       Data='0';
   }
   else if(KeyData>8 && KeyData<12)
   {
       Data=ConvIntToChar(KeyData-2);
   }
   else if(KeyData>4 && KeyData<8)
   {
       Data=ConvIntToChar(KeyData-1);
   }
   else
   {
       Data= ConvIntToChar(KeyData);
   }
   return Data;
} 

char ConvIntToChar(int Data)
{
    return Data+48;
}
