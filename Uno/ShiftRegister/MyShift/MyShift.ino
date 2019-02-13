#define MSB_FIRST 1
#define LSB_FIRST 2

int latch=10, Clock=13, data=7, i;
uint8_t Data=1;

void setup() {
  
Init_Shift(data, Clock, latch);
}

void loop() {
  for(i=0; i<8; i++)
  {
    ShiftByte(data, Clock, latch, MSB_FIRST, Data);
    Data<<=1;
    delay(100);
  }
  Data=1;
}
