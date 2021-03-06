/*
 * Cycle: 20ms(TCINT1=398)
 * Duty:1.5ms(OCR1A=773)
 * Servo:PF0 ~ PF7 (A0~A7 pin to arduino)
*/

#define TIMSK_OVF 0X01
#define TIMSK_COMP_A 0X02
#define PRE_SCALE_64 0X03

#define PIN_SERVO 5
#define SERVO_SPEED 10
#define SERVO_NUM 8
#define RUN 1
#define STOP 0

static int servoState=RUN, timerCount=0, OVF_flag=0;
static uint16_t servoPos[SERVO_NUM]={575, 575, 575, 575, 575, 575, 575, 575};
static int servoSpeed[SERVO_NUM]={SERVO_SPEED*3, SERVO_SPEED, SERVO_SPEED/2, SERVO_SPEED/3, SERVO_SPEED*2, SERVO_SPEED, SERVO_SPEED, SERVO_SPEED};
int i;

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
OCR1A=580;
TCNT1=398;
interrupts();

  for(i=0; i<SERVO_NUM; i++)
  {
    DDRK|= 1<<i;
    PORTK|= 1<<i;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
}


ISR(TIMER1_COMPA_vect)
{
  PORTK&= ~(1<<(timerCount));
}

ISR(TIMER1_OVF_vect)
{
  TCNT1=398;
  
  timerCount++;
  OVF_flag=1;
  
  if(timerCount>=8)
  {
    timerCount=0;
  }
  
  if(servoState!=STOP)
  {
    servoPos[(timerCount)]+=servoSpeed[(timerCount)];
  }
  
  OCR1A=servoPos[(timerCount+1)%8];
  PORTK|=1<<((timerCount));

  if(servoPos[timerCount]>=1023 || servoPos[timerCount]<=573)
  {
    servoSpeed[timerCount] = -servoSpeed[timerCount];
  }
}
