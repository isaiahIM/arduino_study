int UART_WrtieData(char data)
{
  if(UCSR0A & UART_IS_EMPTY_DATA)
  {
    UDR0=data;
    return 1;
  }
  return 0;
}
