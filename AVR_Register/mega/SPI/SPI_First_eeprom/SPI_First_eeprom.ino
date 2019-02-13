#include "myUART.h"

#define SPI_ENABLE 0X06
#define SPI_WRITE_MODE 0X02
#define SPI_READ_MODE 0X03
#define CS 1
#define SS 1
#define CLK 2
#define MISO 8
#define MOSI 4

uint8_t read_data=0x31, wordAddress=0x00;
volatile uint8_t data=0x45;
void setup() {
  // put your setup code here, to run once:
  
noInterrupts();
  Serial.begin(9600);
  SPCR=0X00;
  SPDR=0X00;
  SPSR=0X00;
  
/*SPI register*/
  DDRL|=CS;
  DDRB|=SS;
  DDRB|=CLK;
  DDRB|= MOSI;
  DDRB&= ~MISO;  

  SPCR&=~(0X01<<7);
  SPCR&= ~(0X01<<5);//MSB first send
  SPCR|= 0X01<<4;// master mode
  SPCR&= ~(0X01<<3);
  SPCR&= ~(0X01<<2);
  SPCR|= 0X03;//128분주
  SPCR|= 0X01<<6;// spi enable
  SPSR= 0X00;
  
  PORTL|=CS;
  PORTB|=SS;
interrupts();



/*write*/
  PORTL&= ~(CS);
  SPI_Write(SPI_ENABLE);
  PORTL|= CS;

  PORTL&= ~(CS);
  SPI_Write(SPI_WRITE_MODE);
  SPI_Write(wordAddress);
  SPI_Write(data);
  PORTL|= CS;
  delay(10);
  
/*read*/
  PORTL&= ~(CS);
  SPI_Write(SPI_READ_MODE);
  SPI_Write(wordAddress);
  SPI_Write(0x00);
  read_data=SPI_Read();
  PORTL|= CS;

  Serial.println((char)read_data);
}

void loop() {
  // put your main code here, to run repeatedly:

/*write*/
  PORTL&= ~(CS);
  SPI_Write(SPI_ENABLE);
  PORTL|= CS;
  
  PORTL&= ~(CS);
  SPI_Write(SPI_WRITE_MODE);
  SPI_Write(wordAddress);
  SPI_Write(data);
  PORTL|= CS;
  
/*read*/
  PORTL&= ~(CS);
  SPI_Write(SPI_READ_MODE);
  SPI_Write(wordAddress);
  SPI_Write(0x00);
  read_data=SPI_Read();
  PORTL|= CS;
  Serial.println((int)read_data);
}
