#include "Io.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "Lcd.h"
#include "stdio.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#define pi 3.14159265358979323846
#define EARTH_RADIUS_M 6371000
float LATs[1000]={0},LONGs[1000]={0},DIST[1000]={0};
float start_time,end_time,max_speed,SPEED,first_lat,first_long;
uint8_t coordinate_index=1;
uint8_t flag_Validity=0;
char dis[15]={0};
volatile float distance_acc;
char noOfTokenStrings;
char Valid ;
char Lat[15];
char Long[15];
char speed[15];
char time [6]; 
char North;
char East;
char recieved_char;
volatile char fish =1;
uint8_t is_first=1;
char i=0;
uint8_t No_commas;
uint8_t comma_index;
char flag=1;
char GPS_logname[7]="$GPRMC,"; //Used to compare in the if condition
char GPS[80];
float currentlong , currentLat , currentspeed;
float Firstlong , FirstLat;
float previouslat,previouslong;


void RecieveString(char *token, char stop_char){
    while(1){
    char recieve=UART_receive(UART2);
    if(recieve!=stop_char){
*token=recieve;
token++;
}
else break;
    }}

void GPS_read(){
//checking if i recieved the correct log name from the gps 
	do{
		flag=1;
        for ( i=0; i<7; i++){
	if (UART_receive(UART2) !=GPS_logname[i]){
	flag = 0; break;
		}
	}
	} while (flag==0);
	RecieveString(GPS,'*');
	/////////////////////// example:$GPRMC,203522.00,A,5109.0262308,N,11401.8407342,W,0.004,133.4,130522,0.0,E,D*2B
	No_commas=0; //counter of commas
	comma_index=0; // commaindex less than start by 1
	for(i=0;(i<80)&&(No_commas<7);i++){
		if (GPS[i]==','){
		No_commas++;
		comma_index=i;
		continue; //to finish the for and enter the switch case
		}
		switch (No_commas){
			case 0:
				time[i]=GPS[i];
			break;
			case 1:
					Valid=GPS[i];
			if (Valid == 'V')flag_Validity=1; //checking for data validity 
			break;
			case 2:   
					Lat[i-comma_index-1]=GPS[i]; //recieving the latitude 
			break;
			case 3:
					North=GPS[i];
			break;
			case 4:
					Long[i-comma_index-1]=GPS[i]; //recieving the longitude 
			break;
			case 5:
					East=GPS[i];
			break;
			case 6:
					speed[i-comma_index-1]=GPS[i];
			break;
					}
		if (flag_Validity) {flag_Validity=0;break;} // checks if data is not valid return to start of the while
	}
}
