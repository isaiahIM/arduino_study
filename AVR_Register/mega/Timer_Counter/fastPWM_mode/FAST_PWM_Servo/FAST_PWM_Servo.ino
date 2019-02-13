/*
 * Cycle: 20ms(TCINT1=398)
 * Duty:1.5ms(OCR1A=773)
 * Servo:PG5
*/

#define TIMSK_OVF 0X01
#define TIMSK_COMP_A 0X02
#define PRE_SCALE_64 0X03

#define PIN_SERVO 5
#define SERVO_SPEED 40
#define SERVO_NUM 2

static int servoPowerState=1, timerCount=0, AngleCnt=0, AngleFlag=10;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
noInterrupts();
TIMSK1|=TIMSK_COMP_A;
TIMSK1|=TIMSK_OVF;

/*FAST PWM mode*/
TCCR1A|=0X03;
TCCR1B&=~(0X03<<3);

TCCR1B|=PRE_SCALE_64;// pre scale setting

TCCR1A|=0x02<<6;// OCR enable
OCR1A=573;
TCNT1=398;
interrupts();

DDRG|=1<<PIN_SERVO;
PORTG|=1<<PIN_SERVO;

}

void loop() {
  // put your main code here, to run repeatedly:

  if(timerCount>=8)
  {
    servoPowerState=1;
    
    PORTG|=1<<PIN_SERVO;
    timerCount=0;
    
    if(OCR1A>=1023)
    {
      AngleFlag=-SERVO_SPEED;
    }
    
    else if(OCR1A<=573)
    {
      AngleFlag=SERVO_SPEED;
    }
  }
  
}

ISR(TIMER1_COMPA_vect)
{
  PORTG&=~(1<<PIN_SERVO);
}

ISR(TIMER1_OVF_vect)
{
  timerCount++;
  TCNT1=398;
  
  if(servoPowerState)
  {
    servoPowerState=0;
    OCR1A+=AngleFlag;
  }
}
