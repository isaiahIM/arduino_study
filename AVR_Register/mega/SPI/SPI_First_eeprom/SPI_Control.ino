#define WRITE_FINISH 0X80

int SPI_Write(uint8_t data)
{
  SPDR=data;
  while(!(SPSR & WRITE_FINISH));  
  
  return 1;
}

uint8_t SPI_Read(void)
{
 while(!(SPSR & WRITE_FINISH));  
 
 return SPDR;
}
