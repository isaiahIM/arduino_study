/*
2ms cycle, 0~100 Range PWM
Count Cycle:20us
*/

#define LED_PIN 7
int bright=0, lightState=0, lightFlag=1;

void setup() {
  // put your setup code here, to run once:
  
  DDRB|=1<<LED_PIN;
  PORTB=(1<<LED_PIN);
  
  noInterrupts();
  TIMSK2=0X00;// overflow
  TCCR2B=0X00;// 64
  TCCR2A=0X00;// 64
  TCNT2=0;

  TIMSK2=0X01;// overflow
  TCCR2B=0X04;// 64
  TCNT2=251;
  

  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bright>100)
  {
    bright=0;
    PORTB|= 1<<LED_PIN;
    lightState+=1;
    lightFlag=1;

    if(lightState>100)
    {
      lightState=0;
    }
  }

  if(lightFlag==1 && lightState<bright)
  {
    lightFlag=0;
    PORTB&= ~(1<<LED_PIN);
  }
}

ISR(TIMER2_OVF_vect)
{
  TCNT2=251;
  bright++;
}
