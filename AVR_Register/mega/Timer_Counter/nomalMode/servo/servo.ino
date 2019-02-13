/*
 * Servo control;
 * 0.5us timer count
*/

#define PRE_SCALE_8 0x02
#define TIMER_OVF 0x
void setup() {
  // put your setup code here, to run once:
  noInterrupts();
  TCCR1B|=PRE_SCALE_8;
  TIMSK1=TIMER_OVF;
  TCNT1=
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

}
