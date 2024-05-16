#include "Io.h"
#include "gps.h"
#include "tm4c123gh6pm.h"

void PF_Init(){							//FUNCTION FOR INITIALIZING PORTF PINS 0->4
SYSCTL_RCGCGPIO_R |= 0x20; // Clock init for port F
while((SYSCTL_PRGPIO_R & 0x20) == 0); // delay
GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // Lock disable
GPIO_PORTF_CR_R |= 0x1F;						//ALLOW CHANGES PORTF FOR PINS 0->4
GPIO_PORTF_AFSEL_R &= ~0x1F;				//DISABLE ALTERNATE FUNCTION PORTF FOR PINS 0->4
GPIO_PORTF_AMSEL_R &= ~0x1F;				//DISABLE ANALOG PORTF FOR PINS 0->4
GPIO_PORTF_DEN_R |= 0x1F;						//ENABLE DIGITAL PORTF FOR PINS 0->4
GPIO_PORTF_PCTL_R &= ~0x000FFFFF;		//MAKING PORTF GPIO PF4-0
GPIO_PORTF_DIR_R |= 0x0E;						// SETTING PINS 123 OUTPUT, PINS 0,4 INPUT
GPIO_PORTF_PUR_R |= 0x11;						//ENABLING PULL UP RESISTANCE PORTF PINS 0,4
GPIO_PORTF_DATA_R &= ~0X0E;					//INITIALIZE ALL RGB TO BE OFF
}
float Displacement=0;
float avg_speed;

void GPIOF_Handler(){					// FUNCTION OF THE INTERRUPT

if(GPIO_PORTF_MIS_R&0x10){			//LOOP FOR CHECKING IF THE INTERRUPT HAS OCCURED

Displacement=	haversine(first_lat,first_long,currentLat,currentlong);	
avg_speed=DIST[coordinate_index-1]/ ((coordinate_index+1)*0.5);
	
fish=0;									//RESETTING FLAG TO STOP EXECUTING THE PROGRAM WHEN INTERRUPT OCCUR
GPIO_PORTF_DATA_R = 0X08;						//CHANGING THE RED LIGHT TO GREEN WHEN PRESSING SWITCH
while(1){														// LOOP FOR REPEATING THE DISPLAYED MESSAGES
lcdClear();lcdPrint("Distance: ",0);sprintf(dis,"%f",DIST[coordinate_index-1]);lcdPrint(dis,1);
delay(100);	
//memset(dis,0,15);
lcdClear();lcdPrint("Displacement: ",0);sprintf(dis,"%f",Displacement);lcdPrint(dis,1);
delay(100);	
//memset(dis,0,15);	
lcdClear();lcdPrint("Avg Speed: ",0);sprintf(dis,"%f",avg_speed);lcdPrint(dis,1);
delay(100);	
//	memset(dis,0,15);


	if(UART_receive(UART0)=='U'){ //checking to send to PC
		char test[15]={0};int J;
	lcdClear();
	lcdPrint("U is Pressed",0);
	lcdPrint("SENDING DATA",1);
		
		UART_send(UART0,'\n'); 
		UART_send(UART0,'\r');
		for(J=0;J<=coordinate_index &&(LATs[J]!=0 )&& (LONGs[J]!=0);J++){ //sending the trajectory
		sprintf(test,"%f",LATs[J]);
			UART_OutPutStr(UART0,test);
			memset(test,0,15);
			UART_send(UART0,',');
			sprintf(test,"%f",LONGs[J]);
			UART_OutPutStr(UART0,test);
			memset(test,0,15);
			UART_send(UART0,'\r');
			UART_send(UART0,'\n');
		}

	
		
	}
	
	


GPIO_PORTF_ICR_R |=0x10;}}						// CLEAR THE CORRESPONDING EDGE TRIGGERED INTERRUPT
}

int main(){
PF_Init();
GPIO_PORTF_IS_R= ~0X10 ;			// PF4 EDGE SENSITIVE
GPIO_PORTF_IBE_R=~0X10 ;			// PF4 ONE EDGE NOT BOTH EDGES
GPIO_PORTF_IEV_R=~0x10 ;			// PF4 FALLING EDGE EVENT
GPIO_PORTF_IM_R= 0X10;				// PF4 INTERRUPT UNMASKED(ENABLED)

NVIC_PRI7_R |=0X00200000;			// PRIORITY 1
NVIC_EN0_R  |=0X40000000;			// ENABLE INTERRUPT 30 IN NVIC

__asm("CPSIE i");							// ENABLE GLOBAL INTERRUPT FLAG(ASSEMBLY)
	lcdInit();
	UART_INIT(UART0);
	UART_INIT(UART2);
	lcdCmd(0x28);
	lcdCmd(0x06);
	lcdCmd(0x0C);
	lcdCmd(0x01);
	lcdPrint("Loading...",0);
	while(1){ 
	while(fish){							//LOOP FOR INTERRUPT	
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
