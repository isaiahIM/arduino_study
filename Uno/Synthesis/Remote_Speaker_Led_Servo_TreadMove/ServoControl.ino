void ServoInit(int Pin)
{
    pinMode(Pin, OUTPUT);
    digitalWrite(Pin, 1);
}

double ConvAngleToDuty(double Angle)
{
    double Duty;
    Duty=((5*(Angle/180) )+5);
    return Duty; 
}

void ServoMove(int Pin, int Angle_Degree, unsigned long Time_us, unsigned long *ServoTime_us)
{
    double Duty;
    Duty=ConvAngleToDuty(Angle_Degree);
    //Serial.println(Duty);
    if(Time_us- *ServoTime_us>= (Duty/100)*20000 )
    {
        *ServoTime_us=Time_us;
        digitalWrite(Pin, 0);
    }
    else
    {
        digitalWrite(Pin, 1);
    }
    
}

