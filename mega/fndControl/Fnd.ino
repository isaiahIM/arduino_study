void DisplayFnd(int Num, int *FndPower, int FndFont[][8], int FndPin[], unsigned long Time, unsigned long *Digit_10Time, unsigned long *Digit_1Time)
{
    int Digit_10, i;
    
    if(Num/10)
    {
        Digit_10=Num/10;
        Num-= (Digit_10*10);
    }

    if(Time - (*Digit_10Time)>=5)
    {
        *Digit_10Time=Time;
        
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
    
    else if( (*Digit_10Time) - (*Digit_1Time) >=10)
    {
        *Digit_1Time=*Digit_10Time;
        
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
