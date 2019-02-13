unsigned long ConvFrequencyToCycle(int Frequency)
{
    return 1000000/Frequency;// us Cycle
}

int ConvCycleToFrequency(unsigned long Cycle)
{
    return 1000000/Cycle;
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
