#include "io.h"
#include "tm4c123gh6pm.h"

void UART0_transmit(char data) {			//Function to transfer data between the GPS module and the MC
	while ((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
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
