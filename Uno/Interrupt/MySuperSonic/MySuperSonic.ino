int trig=3, echo=2, distacne;
unsigned long Echo_HIGH, Echo_LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(trig, OUTPUT);
attachInterrupt(0, SuperSonic_ISR, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trig, 1);
delayMicroseconds(10);
digitalWrite(trig, 0);
delay(100);
}

void SuperSonic_ISR(void)
{
    int EchoState;
    EchoState= digitalRead(echo);
//Serial.println("as");
    if(EchoState==1)
    {
        Echo_HIGH=micros();
    }

    else if(EchoState==0)
    {
        Echo_LOW=micros();
        
        distacne= CalculateDist(Echo_HIGH, Echo_LOW);
        
        Serial.println(distacne);
    }
}

int CalculateDist(unsigned long HighPulseTime, unsigned long LowPulseTime)
{
    return 17*(LowPulseTime-HighPulseTime)/1000;
}
