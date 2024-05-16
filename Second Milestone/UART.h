#include "tm4c123gh6pm.h"
#include "io.h"
typedef enum {
    UART0, //UART0=0
    UART2, //UART0=1
} UARTNO;
//INITIALIZING UART
void UART_INIT(UARTNO UART_NO){
switch(UART_NO) {
       case UART0:
SYSCTL_RCGCUART_R |=(0x0001);
SYSCTL_RCGCGPIO_R |=(0x0001);
while ((SYSCTL_PRGPIO_R & (0x01)) ==0); // waiting for GPIOA clock
GPIO_PORTA_LOCK_R=GPIO_LOCK_KEY; //lock
GPIO_PORTA_CR_R|=0x03;
GPIO_PORTA_AFSEL_R|= 0x03; //enable alternate function
GPIO_PORTA_PCTL_R=(GPIO_PORTA_PCTL_R & 0XFFFFFF00)+0X00000011; //enable UARTpins
GPIO_PORTA_DEN_R|=0x03;
GPIO_PORTA_AMSEL_R &= ~(0x03);
UART0_CTL_R &= ~0x0001; //Disable UART
UART0_IBRD_R=104;  //int (16Mhz/16*9600)
UART0_FBRD_R= 11;  // int ((16Mhz/16*9600 -104 )* 64+0.5)
UART0_LCRH_R=0x0070; // enable FIFO
UART0_CTL_R= 0x0301; // enable UART
            break;
        case UART2:
SYSCTL_RCGCUART_R |=(0x0004);
SYSCTL_RCGCGPIO_R |=(0x0008);
while ((SYSCTL_PRGPIO_R & (0x08)) ==0); // waiting for GPIOD clock
GPIO_PORTD_LOCK_R=GPIO_LOCK_KEY; //lock
GPIO_PORTD_CR_R|=0xC0;
GPIO_PORTD_AFSEL_R|= 0xC0; //enable alternate function
GPIO_PORTD_PCTL_R=(GPIO_PORTD_PCTL_R & 0X00FFFFFF)+0X11000000; //enable UARTpins
GPIO_PORTD_DEN_R|=0xC0;
GPIO_PORTD_AMSEL_R &= ~0xC0;
UART2_CTL_R &= ~0x0001;  //Disable UART
UART2_IBRD_R=104;  //int (16Mhz/16*9600)
UART2_FBRD_R= 11; // int ((16Mhz/16*9600 -104 )* 64+0.5)
UART2_LCRH_R=0x0070; // enable FIFO
UART2_CTL_R= 0x0301; // enable UART
            break;
    }
}

void UART_send(UARTNO uart_no, char data) {

    // Select the appropriate UART data register based on uart_no
    switch(uart_no) {
        case UART0:
            while((UART0_FR_R& 0x0020)!=0); //cheking if fifo is full
               UART0_DR_R=data;

            break;
        case UART2:
           while((UART2_FR_R& 0x0020)!=0); //cheking if fifo is full
               UART2_DR_R=data;
            break;
    }
}

char UART_receive(UARTNO uart_no) {
    // Select the appropriate UART data register based on uart_no
    switch(uart_no) {
        case UART0:
while ((UART0_FR_R & 0x10)!=0) return 0;  //if PC fifo is empty return
            return (char)(UART0_DR_R & 0xFF);
            break;
        case UART2:
            while ((UART2_FR_R & 0x10)!=0) ;  //if GPS fifo is empty wait for data
            return (char)(UART2_DR_R & 0xFF);
            break;
        default:
            // Invalid UART number
            return '\0';
    }
}
void UART_OutPutStr(UARTNO uart_no,char ptr){
while(ptr){
UART_send(uart_no,*ptr); // send string charachter by charachter
ptr++; }
}
