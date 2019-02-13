#define RX 0x01
#define TX 0x02

#define STRING_SIZE 100
static uint8_t data=0x33;
uint8_t g_ReadData;
uint32_t bps=9600,Clock;

char string[STRING_SIZE];

void setup() {
  // put your se tup code here, to run once:// 비반전
DDRE=0XFF;
PORTE|=0X03;
Clock=1000000/bps;
}

void loop() {
  // put your main code here, to run repeatedly:

sprintf(string, "%c%c%c%c%c%c%c%c%c%c%c ", 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd');
UART_Print(TX, string, STRING_SIZE, Clock);

}


int UART_Write(int tx, char data, uint32_t Clock)
{
  int i;
  UART_WriteBit(tx, 0, Clock);
    
  for(i=0; i<8; i++)
  {
    UART_WriteBit(tx, data&(0x01<<i), Clock);
  }
  UART_WriteBit(tx, 1, Clock);
}

int UART_Print(int tx, char *string, uint8_t stringSize, uint32_t Clock)
{
  int j;
  
  for(j=0; j<stringSize; j++)
  {
    /*null character*/
    if(string[j]==0)
    {
      return 1;
    }
    
    UART_Write(tx, string[j], Clock);
  }
  return 1;
}


void UART_WriteBit(int pin, char Bit, uint32_t Clock)
{
  if(Bit!=0)
  {
    PORTE|=pin;
    delayMicroseconds(Clock);
  }
  else
  {
    PORTE&=(~pin);
    delayMicroseconds(Clock);
  }
}
