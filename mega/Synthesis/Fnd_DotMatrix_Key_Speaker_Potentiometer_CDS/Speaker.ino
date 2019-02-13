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
        *SoundTime_us=Time_us;
        PWM(Pin, Cycle, 50, Time_us);
    }
    else if(Time_us- (*SoundTime_us)>=(Delay)*1000 )
    {
        PWM(Pin, Cycle, 0, Time_us);

    }
}

unsigned long ConvFrequencyToCycle(int Frequency)
{
    return 1000000/Frequency;// us Cycle
}

void PWM(int Pin, unsigned long Cycle, double Duty, unsigned long Time)
{
    double PW;
    PW= Cycle*(Duty/100);
    
    if( (Time%(Cycle) ) <=PW && Duty!=0)
    {
        digitalWrite(Pin, 1);
    }
    else
    {
        digitalWrite(Pin, 0); 
    }

}
