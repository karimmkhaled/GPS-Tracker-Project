#include "tm4c123gh6pm.h"
#include "systic.h"
#define lcdClear() lcdCmd(0x01)
#define lcdHome() lcdCmd(0x02)
#define lcdOn() lcdCmd(0x0C)
#define lcdClearLine(line) lcdPrint((char *)" 	",line)
/*
 * Function:  lcdCmd
 * --------------------
 * Sends command (byte) to the LCD module in 4bit mode (nibble by nibble).
 *
 * 
 *  cmd: Command to send to the LCD.
 *
 *  returns: Nothing.
 */
void lcdCmd(char cmd)
{
      
  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  GPIO_PORTB_DATA_R =(cmd>>4);  //Send the upper nibble of the command
  GPIO_PORTA_DATA_R |=(1<<3); //EN HIGH

  delay(1);
  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  GPIO_PORTB_DATA_R =(cmd); //Send the lower nibble of the command
  GPIO_PORTA_DATA_R |=(1<<3); //EN HIGH

  delay(1);
  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  delay(2);

      
}
/*
 * Function:  lcdInit
 * --------------------
 * Initializes the microcontroller and the LCD.
 *
 *
 *  returns: Nothing.
 */
void lcdInit(void)
{
  /*
    Initializing Ports B and A
  */
  SYSCTL_RCGCGPIO_R |= 0x03;      //Enable the clock for GPIO ports A and B (bits 0 and 1)
  while ((SYSCTL_PRGPIO_R&0x03)==0){};    //Wait for GPIO clock to start
  GPIO_PORTB_AFSEL_R &= ~0x0F;
	GPIO_PORTB_PCTL_R  &= ~0x0000FFFF;
  GPIO_PORTB_AMSEL_R &= ~0x0F;
	GPIO_PORTB_DIR_R |= 0x0F;     //Set the direction for PB0-3 as output
  GPIO_PORTB_DEN_R |= 0x0F;     //Enables the digital I/O for PB0-3
	GPIO_PORTA_AFSEL_R &=~(0x0C);
	GPIO_PORTA_PCTL_R  &=~(0x0000FF00);
	GPIO_PORTA_AMSEL_R &=~(0x0C);
  GPIO_PORTA_DATA_R  &=~(0x0C);
	GPIO_PORTA_DIR_R |= 0x0C;     //Set the direction for PA2,3 as output
  GPIO_PORTA_DEN_R |= 0x0C;     //Enables the digital I/O for PA2,3

  /*
    Initializing the LCD
  */
  GPIO_PORTA_DATA_R &=~(0x0C);      //Clear RS and EN
  GPIO_PORTB_DATA_R = 0x00;       //Send the upper nibble for the 4bit mode command (0010)
  GPIO_PORTA_DATA_R |=(1<<3);       //EN HIGH
  delay(5);
  GPIO_PORTA_DATA_R &=~(0x0C);      //Clear RS and EN

  delay(20);
  GPIO_PORTA_DATA_R &=~(0x0C);      //Clear RS and EN
  GPIO_PORTB_DATA_R = 0x2;      //Send the lower nibble for the 4bit mode command (0010)
  GPIO_PORTA_DATA_R |=(1<<3);       //EN HIGH
  delay(5);
  GPIO_PORTA_DATA_R &=~(0x0C);      //Clear RS and EN

  lcdCmd(0x28);         //2 line, 5*7 matrix in 4-bit mode
  lcdClear();
  lcdHome();
  lcdOn();
}
void lcdData(char data)
{
       

  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  GPIO_PORTA_DATA_R |= (1<<2);  //RS HIGH
  GPIO_PORTB_DATA_R =(data>>4); //Write the upper nibble of the data
  GPIO_PORTA_DATA_R |=(1<<3); //EN HIGH

  delay(2);
  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  GPIO_PORTA_DATA_R |= (1<<2);  //RS HIGH
  GPIO_PORTB_DATA_R = data; //Write the lower nibble of the data
  GPIO_PORTA_DATA_R |=(1<<3); //EN HIGH

  delay(2);
  GPIO_PORTA_DATA_R &=~(0x0C);  //Clear RS and EN
  delay(4);

        
}





/*
 * Function:  lcdPrint
 * --------------------
 * Prints the given string on the LCD.
 *
 * 
 *  str: Pointer to the location of the string.
 *  line: Determines the line position on the lCD. 0: Upper line, 1: Lower line. 
 *
 *  returns: Nothing.
 */
void lcdPrint(char *str , int line)
{
	 int i = 0;
  if(!line){
    lcdCmd(0x02);   //Places the cursor on the upper line
  }else{
    lcdCmd(0xC0);     //Places the cursor on the lower line
  }
 
  while(str[i] != '\0'){    //Iterates over the string, till a null character appears
    lcdData(str[i]);  //Writes the character on the lcd
    i++;
  }
}





