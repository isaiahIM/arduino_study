#define MSB_FIRST 1
#define LSB_FIRST 2

int latch=A7, Clock=52, data=51, enable=53, i;
uint8_t Data=1;

void setup() {
  
Init_Shift(data, Clock, latch);
Init_Enable(enable);
}

void loop() {
  
  ShiftEnable(enable);
  
  for(i=0; i<8; i++)
  {
    ShiftByte(data, Clock, latch, MSB_FIRST, Data);
    Data<<=1;
    delay(500);
  }
  Data=1;
  
  ShiftDisable(enable);
}
