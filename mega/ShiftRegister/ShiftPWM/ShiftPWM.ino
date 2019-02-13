#define MSB_FIRST 1
#define LSB_FIRST 2

#define STOP_PWM 0
#define RUN_PWM 1

int latch=A7, Clock=52, data=51, enable=53, i;
uint8_t Data[8]={1, 2, 4, 8, 16, 32, 64, 128};

static uint32_t Cycle=20000, Time ,Time_PWM[8]={0, }, Time_duty=0, flag[8]={0, };
double duty[8]={0, 1, 2, 3, 4, 5, 6, 7};

void setup() {
  Init_Shift(data, Clock, latch);
  Init_Enable(enable);
  
  ShiftEnable(enable);
  Serial.begin(9600);
  Serial.println("A");
}

void loop() {

  Time=micros();
  
  for(i=0; i<8; i++)
  {
      if(Chk_ShiftPWM(Cycle, duty[i], Time, &Time_PWM[i]) ==RUN_PWM) 
      { 
        ShiftByte(data, Clock, latch, MSB_FIRST, Data[i]);
//        flag[i]=1;
      }
      
      else if(Chk_ShiftPWM(Cycle, duty[i], Time, &Time_PWM[i])==STOP_PWM )
      {
        ShiftByte(data, Clock, latch, MSB_FIRST, 0x00);
        flag[i]=-1;
      }
  }
  
  if(Time-Time_duty>=20000)
  {
      flag[i]=0;
      Time_duty=Time;
      for(i=0; i<8; i++)
      {
          flag[i]=0;
          duty[i]+=1;
          if(duty[i]>=100)
          {
            duty[i]=0;
            Serial.println(i);
          }
      }
  }
  
}
