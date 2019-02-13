#define PRE_SCALE_256 0X06
#define COM2A 6
#define LED_PIN 7

int timeCount=0, ledFlag=1;
void setup() {
  // put your setup  here, to run once:
Serial.begin(9600);
  DDRB|=1<<LED_PIN;
PORTB&= ~(1<<LED_PIN);
noInterrupts();
/*CTC mode*/
TCCR2A=0X02;// CTC mode
TCCR2A|=(0x02<<COM2A);//clear compare

TCCR2B&=~(1<<3);// CTC mode
TCCR2B|=PRE_SCALE_256;// PreScale: 256

OCR2A=100;
TIMSK2=0X02;
interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(timeCount>=15)
  {
    timeCount=0;
    ledFlag^=1;
  }

  if(ledFlag)
  {
    PORTB|=1<<LED_PIN;// on
  }
  else
  {
    PORTB&= ~(1<<LED_PIN);// off
  }
  
}

ISR(TIMER2_COMPA_vect)
{
  timeCount++;
}
