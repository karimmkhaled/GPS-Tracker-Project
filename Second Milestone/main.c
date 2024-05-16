#include "Io.h"
#include "gps.c"
#include "tm4c123gh6pm.h"


int main(){
PF_Init();
GPIO_PORTF_IS_R= ~0X10 ;							// PF4 EDGE SENSITIVE
GPIO_PORTF_IBE_R=~0X10 ;							// PF4 ONE EDGE NOT BOTH EDGES
GPIO_PORTF_IEV_R=~0x10 ;							// PF4 FALLING EDGE EVENT
GPIO_PORTF_IM_R= 0X10;								// PF4 INTERRUPT UNMASKED(ENABLED)
	
NVIC_PRI7_R |=0X00200000;							// PRIORITY 1
NVIC_EN0_R  |=0X40000000;							// ENABLE INTERRUPT 30 IN NVIC
	
__asm("CPSIE i");								// ENABLE GLOBAL INTERRUPT FLAG(ASSEMBLY)

	lcdInit();
	UART_INIT(UART0);
	UART_INIT(UART2);
	lcdCmd(0x28);
	lcdCmd(0x06);
	lcdCmd(0x0C);
	lcdCmd(0x01);
	lcdPrint("Loading...",0);
	while(1){ 
	while(fish){		//LOOP FOR INTERRUPT	
	GPS_read();
		if(Valid=='A'){ // checking Validity of Data recieved
	SPEED=atof(speed);
	if (is_first==1){ // first data received 
	first_lat= currentLat;
	first_long=currentlong;
	LATs[0]=toDegrees(currentLat); 
	LONGs[0]=toDegrees(currentlong);
	is_first=0;
	DIST[0]=0;
		max_speed=SPEED;
	}else { //Any data received
		DIST[coordinate_index]=DIST[coordinate_index-1]+haversine(previouslat,previouslong,currentLat,currentlong); //accumulating Distance
	coordinate_index++; //increasing the index 
}
		lcdClear();	
		lcdPrint("Distance: ",0);
		sprintf(dis,"%f",DIST[coordinate_index-1]);lcdPrint(dis,1);
	
}
delay(50);
// putting current coordinates in previous
previouslat=currentLat; 
previouslong=currentlong;
}																//END OF INTERRUPT LOOP
}}
