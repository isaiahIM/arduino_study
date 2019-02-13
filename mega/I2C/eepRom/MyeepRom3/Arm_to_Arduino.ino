void gpioA_High(int pin)
{
  digitalWrite(pin, 1);
}

void gpioA_Low(int pin)
{
  digitalWrite(pin, 0);
}
 
uint32_t CheckMillis(void)
{
  return millis();
}

void Delay_ms(uint32_t DelayTime)
{
  delay(DelayTime);
}
