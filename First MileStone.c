#include "io.h"
#include "tm4c123gh6pm.h"
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
