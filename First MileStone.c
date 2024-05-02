#include "io.h"
#include "tm4c123gh6pm.h"

// RECEIVE FUNCTION
char UART0_Read (void){
while (UART0_FR_R& 0x010 !=0);  //check if fifo is empty
return (char)(UART0_DR_R &0xFF); 
}

void UART0_transmit(char data) {			//Function to transfer data between the GPS module and the MC
	while ((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
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

 while (1)
 {
unsigned char SW1;
 SW1 = SW1_read();
if(SW1 != 0x10 ){

 RGB_Output(GREEN);
}
}
}
