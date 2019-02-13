

void InitMatrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    
    for(i=0; i<Matrix_Size; i++)
    {
        pinMode(Matrix_Row[i], OUTPUT);
        pinMode(Matrix_Col[i], OUTPUT);

        digitalWrite(Matrix_Row[i], POWER_ON);
        digitalWrite(Matrix_Col[i], POWER_OFF);
    }
}

void DisplayFont(int Matrix_Row[], int Matrix_Col[], int Matrix_Size, unsigned char Font[])
{
    int i, j;
//unsigned long Time1, Time2;

   // Time1=micros();
    for(i=0; i<Matrix_Size; i++)
    {
        for(j=0; j<Matrix_Size; j++)
        {
            if(Font[i]&(0x80>>j))
            {
                ClearMatrix(Matrix_Row, Matrix_Col, Matrix_Size);
                
                digitalWrite(Matrix_Row[j], POWER_OFF);
                digitalWrite(Matrix_Col[i], POWER_ON);
            }
        }
    }
    //Time2=micros();
    //Serial.println(Time2-Time1);
}

void DisplayIndividual(int Matrix_Row[], int Matrix_Col[], int Display_Col, int Display_Row)
{
    digitalWrite(Matrix_Row[Display_Row], POWER_OFF);
    digitalWrite(Matrix_Col[Display_Col], POWER_ON );
}

void ClearMatrix(int Matrix_Row[], int Matrix_Col[], int Matrix_Size)
{
    int i;
    for(i=0; i<Matrix_Size; i++)
    {
        digitalWrite(Matrix_Row[i], POWER_ON);
        digitalWrite(Matrix_Col[i], POWER_OFF);
    }
}

