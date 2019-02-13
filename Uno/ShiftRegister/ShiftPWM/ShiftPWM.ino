#define MSB_FIRST 1
#define LSB_FIRST 2

#define STOP_PWM 0
#define RUN_PWM 1

int latch=10, Clock=13, data=7, i;
uint8_t Data=1;

static uint32_t Cycle=20000, Time ,Time_PWM=0, Time_duty=0;
double duty=0;

void setup() {
  // put your setup code here, to run once:
  Init_Shift(data, Clock, latch);
}

void loop() {

  Time=micros();

  
  if(Chk_ShiftPWM(Cycle, duty, Time, &Time_PWM) ==RUN_PWM)
  {
    ShiftByte(data, Clock, latch, MSB_FIRST, Data);
  }
  else
  {
    ShiftByte(data, Clock, latch, MSB_FIRST, 0);
  }

  if(Time-Time_duty>=10000)
  {
    Time_duty=Time;
    duty++;
    if(duty==100)
    {
      duty=0;
      Data<<=1;
       if(Data==0x00)
      {
        Data=1;
      }
    }
  }
}
