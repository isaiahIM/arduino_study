#define RISING_EDGE 1
#define FALLING_EDGE 0

/*UCSRnA*/
#define UART_IS_EMPTY_DATA 0x30
#define UART_GET_TX_COMPLETE 0X40
#define UART_GET_RX_COMPLETE 0X80
#define UART_DOUBLE_BPS_USE 0x02

/*UCSRnB*/
#define UART_RX_TX_INT_ENABLE 0xc0
#define UART_RX_TX_ENABLE 0X18
#define UART_EMPTY_INT_ENABLE 0x20

/*UCSRnC*/
#define UART_8BIT_DATA_SIZE 0x06
#define UART_DATACHANGE FALLING_EDGE
#define UART_NO_PARITY 0X00

/*UBRRL*/
#define UART_9600_BPS 103

char readData=0;

void setup() {
  // put your setup code here, to run once:
UCSR0A &= ~UART_DOUBLE_BPS_USE;

UCSR0B|=UART_RX_TX_ENABLE;

UCSR0C|=UART_8BIT_DATA_SIZE;
UCSR0C|=UART_DATACHANGE;
UCSR0C|=UART_NO_PARITY;

UBRR0L=UART_9600_BPS;
UBRR0H=0X00;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  readData=UART_ReadData();
  
  if(readData!=-1)
  {
    UART_WriteData(readData);
  }
  
}
