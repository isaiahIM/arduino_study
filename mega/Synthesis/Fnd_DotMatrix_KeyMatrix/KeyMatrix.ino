void InitKeyMatrix(int Key_col, int Key_row, int col[], int row[], int MatrixType)
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
