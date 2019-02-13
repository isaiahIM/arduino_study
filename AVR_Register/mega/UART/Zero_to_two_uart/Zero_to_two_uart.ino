#include "myUART.h"

static char Data='A';

void setup() {
  // put your setup code here, to run once:
  /*uart 2*/
  UCSR2A=0x00;
  
  UCSR2B|=UART_RX_INT_ENABLE;
  UCSR2B|=UART_RX_ENABLE;
  UCSR2B|=UART_TX_ENABLE; 
  
  UCSR2C|=UART_8BIT_DATA_SIZE;
  UCSR2C|=UART_DATACHANGE_FALLING;

  UBRR2L=UART_9600_BPS;

  /*UART 1*/
  UCSR1A=0x00;
  
  UCSR1B|=UART_RX_INT_ENABLE;
  UCSR1B|=UART_RX_ENABLE;
  UCSR1B|=UART_TX_ENABLE;
  
  UCSR1C|=UART_8BIT_DATA_SIZE;
  UCSR1C|=UART_DATACHANGE_FALLING;

  UBRR1L=UART_9600_BPS;

  /*UART 0*/
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
  UART_WriteData(Data, 0);
  delay(1000);
  Data++;
  
}

ISR(USART2_RX_vect)
{
  char data;
  data=UART_ReadData(2);
  UART_WriteData(data, 0);
}

ISR(USART1_RX_vect)
{
  char data;
  data=UART_ReadData(1);
  UART_WriteData(data, 1);
}

ISR(USART0_RX_vect)
{
  char data;
  data=UART_ReadData(0);
  UART_WriteData(data, 0);
}
