#define RISING_EDGE 1
#define FALLING_EDGE 0

/*UCSRnA*/
#define UART_CHECK_TX_COMPLETE 0X40
#define UART_CHECK_RX_COMPLETE 0X80
#define UART_IS_EMPTY_DATA 0x30
#define UART_CHECK_FRAME_ERROR 0X10
#define UART_DOUBLE_BPS_USE 0x02

/*UCSRnB*/
#define UART_RX_INT_ENABLE 0x80
#define UART_TX_INT_ENABLE 0x40
#define UART_EMPTY_INT_ENABLE 0X20
#define UART_RX_ENABLE 0X10
#define UART_TX_ENABLE 0X08

/*UCSRnC */
#define UART_8BIT_DATA_SIZE 0x06
#define UART_DATACHANGE FALLING_EDGE
#define UART_NO_PARITY 0X00

/*UBRRL*/
#define UART_9600_BPS 103
#define UART_19200_BPS 51
#define UART_57600_BPS 16
#define UART_115200_BPS 8