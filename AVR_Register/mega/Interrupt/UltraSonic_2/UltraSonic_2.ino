#define TRUE 1
#define FALSE 0

#define CUSTOM_CODE 1
#define DATA_CODE 2
#define REMOTE_PULSE_COUNT 38

#define PIN_CHANGE 0x01

#define ULTRA_SONIC_INT 4
#define SONIC_TRIGGER 3
#define SONIC_ECHO 4

#define IR_REMOTE 2

uint32_t curTime_us, curTime_ms;

int g_SonicTrigState=0;// arduino pin: trig:18, echo:2
volatile uint32_t g_echoHighTime=0, g_echoLowTime=0, g_sonic_dist, g_SonicReadTime=0, g_SonicTrigTime=0, g_EchoHighTime; 

int g_remotePulseCnt=0, g_remoteMapData;

uint16_t g_remoteDataCode=0;
uint32_t g_remoteData=0;

volatile uint32_t g_remotePulseStart_us=0, g_remotePulseEnd_us=0;// Global value

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

DDRD|=0X01<<SONIC_TRIGGER;
PORTD&=~(1<<SONIC_TRIGGER);

DDRE&=~(1<<SONIC_ECHO);

EICRB|= (PIN_CHANGE);
EIMSK|=1<<ULTRA_SONIC_INT;

}

void loop() {
  // put your main code here, to run repeatedly:
 curTime_us=micros();
  
  if(curTime_us-g_SonicReadTime>=100)
  {
      g_SonicReadTime=curTime_ms;
      UltraSonic_Pulse(SONIC_TRIGGER, curTime_us, &g_SonicTrigTime, &g_SonicTrigState);
      Serial.println(g_sonic_dist);
  }
}

ISR(INT4_vact)
{
uint8_t echoState;
    
    echoState=(PINE);
    
    if(echoState!=0)
    {
        g_echoHighTime=micros();
    }
    else
    {
        g_echoLowTime=micros();
        g_sonic_dist= CalculateDist(g_echoHighTime, g_echoLowTime);
    }
}
