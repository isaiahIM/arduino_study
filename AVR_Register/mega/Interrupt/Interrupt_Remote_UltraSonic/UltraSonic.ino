void UltraSonic_Pulse(int trig, unsigned long Time_us, unsigned long *SonicTime_us, int *TrigState)
{
  if( !(*TrigState) )
  {
    PORTE|=(0X01<<trig);// HIGH
    
    *SonicTime_us=micros();
    *TrigState=1;
  }
  
  if(*SonicTime_us-Time_us>=10)
  {
    PORTE&=~(0X01<<trig);// LOW
    *TrigState=0;
  }
}

int CalculateDist(unsigned long HighPulseTime, unsigned long LowPulseTime)
{
    return 17*(LowPulseTime-HighPulseTime)/1000;
}
