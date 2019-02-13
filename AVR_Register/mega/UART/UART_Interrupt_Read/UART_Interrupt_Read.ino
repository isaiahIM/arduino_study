#include"myUART.h"

void setup() {
  // put your setup code here, to run once:
UCSR0A=0x00;

UCSR0B|=UART_RX_INT_ENABLE;
UCSR0B|=UART_RX_ENABLE;
UCSR0B|=UART_TX_ENABLE;

UCSR0C|=UART_8BIT_DATA_SIZE;
UCSR0C|=UART_DATACHANGE;

UBRR0L=UART_9600_BPS;
UBRR0H=0x00;
}

void loop() {
  // put your main code here, to run repeatedly:
// printf 제작, 1번 uart에서 2번 uart로 송신하고 그걸 print
}

ISR(USART0_RX_vect)
{
  char data;
  data=UART_ReadData();

  UART_WriteData(data);
}
