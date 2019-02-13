#include "myUART.h"
char dataArr[100]={0, }, writeState=0;
volatile char i=0;
void setup() {
  // put your setup code here, to run once:
  UCSR0A=0x00;
  
  UCSR0B|=UART_RX_INT_ENABLE;
  UCSR0B|=UART_RX_ENABLE;
  UCSR0B|=UART_TX_ENABLE;
  
  UCSR0C|=UART_8BIT_DATA_SIZE;
  UCSR0C|=UART_DATACHANGE_FALLING;

  UBRR0L=UART_9600_BPS;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(writeState && dataArr[i]!='\0')
  {
    if( UART_WriteData(dataArr[i], 0) )
    {
    i++;
    }
  }
  else
  {
    i=0;
    writeState=0;
  }
}

ISR(USART0_RX_vect)
{
  writeState=1;
  MyPrintf(dataArr, "%c", UDR0);
}
