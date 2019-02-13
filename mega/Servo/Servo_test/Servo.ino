void ServoInit(int Pin)
{
    pinMode(Pin, OUTPUT);
    digitalWrite(Pin, 1);
}

void ServoMove(int Pin, int Angle_Degree, unsigned int ServoCnt)
{
    
    if(ServoCnt== 60+Angle_Degree)
    {
        digitalWrite(Pin, 0);
    }
   
}

void ServoReset(int Pin, int *ServoCnt)
{
    digitalWrite(Pin, 1);
    *ServoCnt=0;
}
