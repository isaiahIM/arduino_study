#define TRUE 1
#define FALSE 0

#define CUSTOM_CODE 1
#define DATA_CODE 2
#define REMOTE_PULSE_COUNT 38

#define PIN_CHANGE 0x01

#define ULTRA_SONIC_INT 0
#define SONIC_TRIGGER 3
#define SONIC_ECHO 4

#define IR_REMOTE 2

uint32_t curTime_us, curTime_ms;

int g_SonicTrigState=0;// arduino pin: trig:18, echo:2
volatile uint32_t g_echoHighTime=0, g_echoLowTime=0, g_sonic_dist, g_SonicReadTime=0, g_SonicTrigTime, g_EchoHighTime; 

int g_remotePulseCnt=0, g_remoteMapData;

uint16_t g_remoteDataCode=0;
uint32_t g_remoteData=0;

volatile uint32_t g_remotePulseStart_us=0, g_remotePulseEnd_us=0;// Global value
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  DDRE|=1<<SONIC_TRIGGER;// pinMode
  PORTE &= ~(1<<SONIC_TRIGGER);// low
  
//  DDRD &= ~(1<<SONIC_ECHO);

EICRB&= ~(2<<(ULTRA_SONIC_INT*2));
EICRB|= (1<<( ULTRA_SONIC_INT*2));// inturrupt option setting
//EICRA|=(PIN_CHANGE<<(IR_REMOTE*2));// inturrupt option setting

EIMSK |= 1<<4;
}

void loop() {
  // put your main code here, to run repeatedly:
    
  curTime_us=micros();
  
  if(curTime_us-g_SonicReadTime>=100)
  {
      g_SonicReadTime=curTime_ms;
      UltraSonic_Pulse(SONIC_TRIGGER, curTime_us, &g_SonicTrigTime, &g_SonicTrigState);
//      Serial.println(g_sonic_dist);
  }
  
}

/*UltraSonic ISR*/
ISR(INT4_vect)
{
uint8_t echoState;
    Serial.println("A");
    echoState&=(PIND& (1<<SONIC_ECHO));
    if(echoState!=0)
    {
        g_echoHighTime=micros();
    }

    else
    {
        g_echoLowTime=micros();
        
        g_sonic_dist= CalculateDist(g_echoHighTime, g_echoLowTime);
        Serial.println("A");
    }
}

/*IR remote ISR*/
//ISR(INT2_vect)
//{
//  cli();
//  ISR_Read_RemoteData();
//  EIFR|=(0x01<<IR_REMOTE);
//  sei();
//}

void ISR_Read_UltraSonic(void)
{
    uint8_t echoState;
    
    echoState= PINE;
    echoState&=(0x01<<SONIC_ECHO);
    
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


void ISR_Read_RemoteData(void)
{
    if(g_remotePulseCnt==0)
    {
        g_remotePulseStart_us=micros();
        g_remotePulseCnt++;
    }
    else
    {
        g_remotePulseEnd_us=micros();
        
        /*LeadCode*/
        if(g_remotePulseEnd_us - g_remotePulseStart_us>=12000 && g_remotePulseEnd_us - g_remotePulseStart_us<=15000)
        {
            g_remoteData= 0;
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
      
        /*bit is 0 */
        else if(g_remotePulseEnd_us - g_remotePulseStart_us>=800 && g_remotePulseEnd_us - g_remotePulseStart_us<=1400)
        {
            g_remoteData=g_remoteData<<1;
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
      
        /*bit is 1*/
        else if(g_remotePulseEnd_us - g_remotePulseStart_us>=1900 && g_remotePulseEnd_us - g_remotePulseStart_us<=2600)
        {
            g_remoteData=g_remoteData<<1;
            g_remoteData|=0x00000001;  
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
        g_remotePulseCnt++;
    }
}
