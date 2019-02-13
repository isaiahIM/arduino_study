#define LED_PIN 7// arduino 13pin from mega
//2ms 주기 led, 서보
//2번 타이머 사용
// isr(타이머2 ovf 사용)

int led_State=1, timeCnt=0;

void setup() {
  // put your setup code here, to run once:
DDRB|=1<<LED_PIN;
PORTB&= ~(1<<LED_PIN);

noInterrupts();
TIMSK2=0X01;// timer counter use register(if count overflow, goto ISR)
TCCR2B= 0x06;// free scale: 256
TCNT2=6;
interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(timeCnt>=125)
  {
    led_State^=1;
    timeCnt=0;
  }
  
  if(led_State)
  {
    PORTB|=1<<LED_PIN;// on
  }
  else
  {
    PORTB&= ~(1<<LED_PIN);// off
  }
}

ISR(TIMER2_OVF_vect)
{
  TCNT2=6;
  timeCnt++;
}
