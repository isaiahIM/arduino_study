int UART_WriteData(char data)
{
  if(UCSR0A & UART_IS_EMPTY_DATA)
  {
    UDR0=data;
    return 1;
  }
  return 0;
}
char UART_ReadData(void)
{ 
  if(UCSR0A & UART_CHECK_RX_COMPLETE)
  {
    return UDR0;
  }
  return -1;
}
