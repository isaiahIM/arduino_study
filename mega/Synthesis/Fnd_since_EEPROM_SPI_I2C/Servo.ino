void Init_Servo(int Pin)
{
    pinMode(Pin, OUTPUT);
    digitalWrite(Pin, 1);
}

void ServoMove(int Pin, int Angle_Degree,  float ServoCnt)
{
    
    if(ServoCnt>= 70+Angle_Degree)
    {
        digitalWrite(Pin, 0);
    }
   
}

void ServoReset(int Pin, float *ServoCnt)
{
    digitalWrite(Pin, 1);
    *ServoCnt=0;
}
