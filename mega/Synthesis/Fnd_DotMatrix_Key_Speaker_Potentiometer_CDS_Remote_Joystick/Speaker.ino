void ToneInit(int Pin)
{
    pinMode(Pin, OUTPUT);
}

void Tone(int Pin, int Frequency, unsigned long time_us, unsigned long *soundTime_us, int *SpeakFlag)
{
    unsigned long Cycle;
    Cycle=ConvFrequencyToCycle(Frequency);
        
    if(time_us-(*soundTime_us)>= Cycle/2)// Frequency to cycle
    {
        *soundTime_us=micros();
        
        (*SpeakFlag)^=1;// toggle(50% Duty)
        
        if(*SpeakFlag)
        {
        digitalWrite(Pin, 1);
        }
        else
        {
        digitalWrite(Pin, 0);
        }
    }
}

void MuteTone(int Pin)
{
  digitalWrite(Pin, 0);
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
