#include "Io.h"
#include "gps.c"
#include "tm4c123gh6pm.h"


int main(){
PF_Init();

	lcdInit();
	UART_INIT(UART0);
	UART_INIT(UART2);
	lcdCmd(0x28);
	lcdCmd(0x06);
	lcdCmd(0x0C);
	lcdCmd(0x01);
	lcdPrint("Loading...",0);
	while(1){ 
		
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
																//END OF INTERRUPT LOOP
}}
