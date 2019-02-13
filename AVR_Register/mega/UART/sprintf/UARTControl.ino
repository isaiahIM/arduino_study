int UART_WriteData(char data, int uart_num)
{
  if(uart_num==0)
  {
    if(UCSR0A & UART_IS_EMPTY_DATA)
    {
      UDR0=data;
      return 1;
    }
  }

  else if(uart_num==1)
  {
    if(UCSR1A & UART_IS_EMPTY_DATA)
    {
      UDR1=data;
      return 1;
    }
  }

  else if(uart_num==2)
  {
    if(UCSR2A & UART_IS_EMPTY_DATA)
    {
      UDR2=data;
      return 1;
    }
  }
  
  return 0;
}
char UART_ReadData(int uart_num)
{ 
  if(uart_num==0)
  {
    if(UCSR0A & UART_CHECK_RX_COMPLETE)
    {
      return UDR0;
    }
  }

  else if(uart_num==1)
  {
    if(UCSR1A & UART_CHECK_RX_COMPLETE)
    {
      return UDR1;
    }
  }

  else if(uart_num==2)
  {
    if(UCSR2A & UART_CHECK_RX_COMPLETE)
    {
      return UDR2;
    }
  }
  
  return -1;
}

void MyPrintf(char arr[], char format[], char data)
{
  sprintf(arr, format, data);
}
