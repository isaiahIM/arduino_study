void InitFnd(int PinSize, int PowerSize, int FndPin[], int FndPower[])
{
    int i;
   for(i=0; i<PinSize; i++)
    {
        pinMode(FndPin[i], OUTPUT);
    }
    
    for(i=0; i<PowerSize; i++)
    {
        pinMode(FndPower[i], OUTPUT);
    }
}

void DisplayFnd(int Num, int *FndPower, int FndFont[][8], int FndPin[], int Digit)
{
    int Digit_10, i;
    
    if(Num/10)
    {
        Digit_10=Num/10;
        Num-= (Digit_10*10);
    }

    if(Digit==1)
    {   
        digitalWrite(FndPower[0], FND_POWER_OFF);
        digitalWrite(FndPower[1], FND_POWER_OFF);
        
        /*10 Digit Display*/
        for(i=0; i<8; i++)
        {
            digitalWrite(FndPin[i], FndFont[Digit_10][i]);
        }
        digitalWrite(FndPower[0], FND_POWER_OFF);
        digitalWrite(FndPower[1], FND_POWER_ON);
    }
   
        else if(Digit!=1)
    {
        digitalWrite(FndPower[0], FND_POWER_OFF);
        digitalWrite(FndPower[1], FND_POWER_OFF);
        
        /*1 Digit Display*/
        for(i=0; i<8; i++)
        {
            digitalWrite(FndPin[i], FndFont[Num][i]);
        }
        digitalWrite(FndPower[0], FND_POWER_ON);
        digitalWrite(FndPower[1], FND_POWER_OFF);
    }
}
