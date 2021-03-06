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
        digitalWrite(FndPower[1], FND_POWER_ON);
        
        /*10 Digit Display*/
        for(i=0; i<8; i++)
        {
            digitalWrite(FndPin[i], FndFont[Digit_10][i]);
        }
        
    }
    
    else if(Digit!=1)
    {
        digitalWrite(FndPower[0], FND_POWER_ON);
        digitalWrite(FndPower[1], FND_POWER_OFF);
        
        /*1 Digit Display*/
        for(i=0; i<8; i++)
        {
            digitalWrite(FndPin[i], FndFont[Num][i]);
        }
    
    }
}
