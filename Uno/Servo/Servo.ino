int servo=3, servo2=4, i;
int Led[3]={5, 6, 7};

unsigned long CurrentTime_us,  ProssesFinish_us, ProssesTime_us=0;// Prosses Timmer
unsigned long ServoMicro=0, ServoMicro2=0, ServoCycle=0;// Servo Control Time Value
unsigned long LedMicro=0, LedCycle[3]={0,};// LED Control Time Value
unsigned long P[12];
int LedFlag[3]={-2, -2, -1};
int Angle=20, Angle2=20, Bright[3]={0,}, ServoCount=0, LedCount[3]={0,}, Flag=-1, Flag2=-2;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 
 for(i=0; i<3; i++)
 {
    pinMode(Led[i], OUTPUT);
 }
 
 pinMode(servo, OUTPUT); 
 pinMode(servo2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    
    CurrentTime_us=micros();
    
/************************Cycle Function******************************/
    /*Servo Cycle(20ms)*/
    if( (CurrentTime_us+ProssesTime_us)-ServoCycle>=20000 )
    {
       // P[0]=micros();
        ServoCycle=CurrentTime_us;    
        //digitalWrite(servo, 1);
        //digitalWrite(servo2, 1);
        PORTD|= (1<<3) | (1<<4);
        
        if(Angle>=90 || Angle<=20)
        {
            Flag*=-1;
        }
        Angle+=Flag;

        if(Angle2>=90 || Angle2<=20)
        {
            Flag2*=-1;
        }
        Angle2+=Flag2;
        
        ServoCount=0;
        //P[1]=micros();
    }

    
    /*LED[0] Cycle(20ms)*/
    if( (CurrentTime_us+ProssesTime_us)-LedCycle[0]>=20000)
    {
        //P[2]=micros();
        LedCycle[0]=CurrentTime_us;

        if(Bright[0]>0)
        {
        PORTD|= (1<<5);
        }
        
        LedCount[0]=0;
        
        if(Bright[0]>=1000 || Bright[0]<=0)
        {
            LedFlag[0]*=-1;
        }
       // P[3]=micros();
       Bright[0]+=LedFlag[0];
    }

    /*LED[1] Cycle(10ms)*/
    if( (CurrentTime_us+ProssesTime_us)-LedCycle[1]>=10000)
    {
       // P[4]=micros();
        LedCycle[1]=CurrentTime_us;

        if(Bright[1]>0)
        {
        PORTD|= (1<<6);
        }
        
        LedCount[1]=0;
        
        if(Bright[1]>=500 || Bright[1]<=0)
        {
            LedFlag[1]*=-1;
        }
        //P[5]=micros();
         Bright[1]+=LedFlag[1];
    }   
   
    /*LED[2] Cycle(5ms)*/
    if( (CurrentTime_us+ProssesTime_us)-LedCycle[2]>=5000)
    {
        //P[6]=micros();
        LedCycle[2]=CurrentTime_us;
        
        if(Bright[2]>0)
        {
        PORTD|= (1<<7);
        }
        
        LedCount[2]=0;
        
        if(Bright[2]>=250 || Bright[2]<=0)
        {
            LedFlag[2]*=-1;
        }
       // P[7]=micros();
       Bright[2]+=LedFlag[2];
    }

    
;/************************PWM Control Function******************************/
    /*Servo PWM Control*/
    if( (CurrentTime_us)-ServoMicro >=10)
    {   
       // P[8]=micros();
        ServoMicro=CurrentTime_us;
       
        if( ServoCount== Angle)// 어디서 1ms delay가 발생하는지 찾기
        {
         //Serial.println(Angle);
            digitalWrite(servo, 0);
        }
        if( ServoCount == Angle2) 
        {
            digitalWrite(servo2, 0);
        }
        
        ServoCount++;
        //P[9]=micros();
    }

     
    /*Led PWM Control*/
    if( (CurrentTime_us+ProssesTime_us)-LedMicro>=20)
    {
        //P[10]=micros();
        
        LedMicro=CurrentTime_us;

        LedCount[0]++; 
        LedCount[1]++; 
        LedCount[2]++;
         
        if(LedCount[0]==Bright[0])
        {
            //PORTD&= !(1<<(5) );
            digitalWrite(Led[0], 0);
        }
 
        
        if(LedCount[1]==Bright[1])
        {
            //PORTD&= !(1<<(6) );
            digitalWrite(Led[1], 0);
        }
  
        
        if(LedCount[2]==Bright[2])
        {
            //PORTD&= !(1<<(7) );
            digitalWrite(Led[2], 0);
        }
        
       // P[11]=micros();
    }
/*
for(i=0;i<11; i+=2)
{
  Serial.println(P[i+1]-P[i]);
}
Serial.println();*/

ProssesFinish_us=micros();
ProssesTime_us=ProssesFinish_us-CurrentTime_us;
//Serial.println(ProssesTime_us);
}
