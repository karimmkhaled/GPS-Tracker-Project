#include "tm4c123gh6pm.h"
#include "stdint.h"
void systic_unit (unsigned long tdelay){
NVIC_ST_CTRL_R=0; //disable timer
NVIC_ST_RELOAD_R=tdelay-1; //number of cycles for delay
NVIC_ST_CURRENT_R=0;
NVIC_ST_CTRL_R=0X05; // enable timer
while((NVIC_ST_CTRL_R&0X00010000)==0);
}
void delay(uint32_t time_delay){
uint32_t J ;
for (J=0;J<time_delay;J++)
{
systic_unit(160000); //delay 10 milliseconds
}}
