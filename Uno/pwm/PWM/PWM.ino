int Led=11, Led2=6, Led3=7, Delay=20;
double cnt=1;
unsigned long Time, CntTime;
int Flag=-1, Delay2=50, Delay3=10;
int cnt2=1, cnt3=1, Flag2=-1, Flag3=-1;

void setup() {
  // put your setup code here, to run once:
pinMode(Led, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(Led3, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    Time=micros();
    CntTime=millis();
  
    if(cnt>=100)
    {
        Flag=-1;
    }
    
    else if(cnt==0)
    {
        Flag=1;
    }
    
    if(cnt2>=100)
    {
        Flag2=-1;
    }
    else if(cnt2==0)
    {
        Flag2=1;
    }
    
    if(cnt3>=100)
    {
        Flag3=-1;
    }
    
    else if(cnt3==0)
    {
        Flag3=1;
    }
    
    PWM(Led, 2, cnt, Time);
    PWM(Led2, 2, cnt2, Time);
    PWM(Led3, 2, cnt3, Time);
    
    if(CntTime%Delay==0)
    {
        cnt+=Flag;
    }
    
    if(CntTime%Delay2==0)
    {
        cnt2+=Flag2;
    }
    
    if(CntTime%Delay3==0)
    {
        cnt3+=Flag3;
    }
}

void PWM(int Port, int Cycle, double Duty, unsigned long Time)
{
    double PW;
    
    PW= Cycle*(Duty/100);
    
    if( (Time%(Cycle*1000) ) <=PW*1000 && Duty!=0)
    {
        digitalWrite(Port, 1);
    }
        
    else if( Time%(Cycle*1000) >PW*1000 && Duty!=100)
    {
        digitalWrite(Port, 0); 
    }
}
