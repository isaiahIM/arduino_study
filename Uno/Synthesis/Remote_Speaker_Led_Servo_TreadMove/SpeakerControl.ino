void ToneInit(int Pin)
{
    pinMode(Pin, OUTPUT);
}

void Tone(int Pin, int Frequency, int Delay, unsigned long Time_us, unsigned long *SoundTime_us)
{
    unsigned long Cycle;
    
    Cycle=ConvFrequencyToCycle(Frequency);
    
    if(Time_us- (*SoundTime_us)<=(Delay)*1000 )
    {
        *SoundTime_us=micros();
        PWM(Pin, Cycle, 50, Time_us);
    }
    else if(Time_us- (*SoundTime_us)>=(Delay)*1000 )
    {
        PWM(Pin, Cycle, 0, Time_us);

    }
}

/*
void PWM(int Pin, int Frequency, int Duty, unsigned long Time)
{
    unsigned long Cycle;
    ConvFrequencyToCycle(Frequency);
    
    if(Time%Frequency<=Duty)
    {
        digitalWrite(Pin, 1);
    }
    else
    {
        digitalWrite(Pin, 0);
    }
}
*/
