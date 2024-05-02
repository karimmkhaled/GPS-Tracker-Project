#include "io.h"
#include "tm4c123gh6pm.h"


// initilization of UART
void UART_Init(void){
SYSCTL_RCGCGPIO_R |=0x0001;
while ((SYSCTL_PRGPIO_R & 0X0001)==0);//activate UART0
SYSCTL_RCGCUART_R |=0X0001;//activate portA
UART0_CTL_R &=~0x0001;//disable UART
UART0_IBRD_R =520;
UART0_FBRD_R=53;
UART0_LCRH_R|=0x0070;
UART0_CTL_R |=0x301;//enable RXE,TXE,UART
GPIO_PORTA_AFSEL_R |=0x03; //ENABLE ALT FUNC PA0,PA1
GPIO_PORTA_PCTL_R |=0x00000011;
GPIO_PORTA_DEN_R |=0x03; //DIGITAL ENABLE
GPIO_PORTA_AMSEL_R &=~0x03; //DISABLE ANALOG
}

// RECEIVE FUNCTION
char UART0_Read (void){
while (UART0_FR_R& 0x010 !=0);  //check if fifo is empty
return (char)(UART0_DR_R &0xFF);
}
//transmit function
void UART0_transmit(char data) {			//Function to transfer data between the GPS module and the MC
	while ((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
}
//GPIO Initialization
void GPIO_Init(void){
SYSCTL_RCGCGPIO_R |= PF_mask; // Clock init for port F
while((SYSCTL_PRGPIO_R & PF_mask) == 0); // delay
GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // Lock disable
GPIO_PORTF_CR_R |= 0X18;
GPIO_PORTF_AFSEL_R &= ~0X18;
GPIO_PORTF_AMSEL_R &= ~0X18;
GPIO_PORTF_DEN_R |= 0X18;
GPIO_PORTF_PCTL_R &= ~0x0000FF000;
GPIO_PORTF_DIR_R |= 0X18;
GPIO_PORTF_DATA_R &= ~0X18;
}
//Switch Initialization
void SW_Init(){

GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
GPIO_PORTF_CR_R |= 0X10;
GPIO_PORTF_AFSEL_R &= ~0X10;
GPIO_PORTF_AMSEL_R &= ~0X10;
GPIO_PORTF_DEN_R |= 0X10;
GPIO_PORTF_PCTL_R &= ~0x000F0000;
GPIO_PORTF_DIR_R &= ~0X10;
GPIO_PORTF_PUR_R |= 0X10;
}

//LED  FUNCTION
void RGB_Output(unsigned char data) {
 GPIO_PORTF_DATA_R &= ~PF123_mask;
 GPIO_PORTF_DATA_R |= data;
}


int main(void){
 GPIO_Init();
 SW_Init();
 UART_Init();

 while (1)
 {
unsigned char SW1;
 SW1 = SW1_read();
if(SW1 != 0x10 ){

 RGB_Output(GREEN);
}
}
}
