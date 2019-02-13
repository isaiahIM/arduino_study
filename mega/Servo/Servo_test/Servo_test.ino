static int g_servo1_pin=4, g_servo2_pin=3, g_ServoChage=0;;
int Angle1=0, Angle2=0, ServoCount=0;

unsigned long Time, g_ServoTime=0, g_CountTime=0, g_AngleTime=0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
ServoInit(g_servo1_pin);
ServoInit(g_servo2_pin);
}

void loop() {
  // put your main code here, to run repeatedly:
  Time=micros();

  if(Time-g_CountTime>=10)
  {
      g_CountTime=Time;
      ServoCount++;
      if(ServoCount==2000)
      {
        ServoReset(g_servo1_pin, &ServoCount);
        ServoReset(g_servo2_pin, &ServoCount);
      }
      
      ServoMove(g_servo1_pin, Angle1, ServoCount);
      ServoMove(g_servo2_pin, Angle2, ServoCount);
  }
  
  if(Time-g_AngleTime>=50000)
  {
    g_AngleTime=Time;
    Angle1++;
    Angle2+=2;
    
    Angle1%=120;
    Angle2%=120;
  }
}
