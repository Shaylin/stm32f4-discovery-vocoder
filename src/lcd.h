#include <stm32f4xx.h>
#include <system_stm32f4xx.h>
#define LCD_RS 0x400     //PE10


#define DATA_PORT GPIOE->ODR
#define CMD_PORT GPIOE->ODR

#define DATA 11 //start writing into pin 11
#define DATA_CLR 0xFFFF87FF

#define LCD_E  0x200     //PE9
#define LCD_RS 0x400     //PE10

#define CMD 0
#define TXT 1

#define        LINE1    0x80        // Start address of first line
#define        LINE2    0xC0        // Start address of second line

void WaitLCDBusy(void);
void initLCD(void);
void LCD_INIT(void);
void LCD_DATA(unsigned char data, unsigned char type);
void LCD_NYB(unsigned char nyb, unsigned char type);
void LCD_STR(const char *text);
void LCD_LINE(char line);
void DelayMS(uint32_t milli);
void ClearDisplay(void);
void initDiscovery(void);
void LCD_Digit(float freqFloat,int line);
void LCD_Freq(uint32_t freqInt, char * lcdLine2);


void initLCD(void)
{
	initDiscovery();
	LCD_INIT();
}

/*
 * @brief function which initializes the LCD
 */
void LCD_INIT()
{
    CMD_PORT &= ~(LCD_E);  //LCD_E = 0;                    //clear enable
    CMD_PORT &= ~(LCD_RS); //LCD_RS = 0;                 //going to write command

    DelayMS(30);                //delay for LCD to initialise.
    LCD_NYB(0x30,0);              //Required for initialisation
    DelayMS(5);                 //required delay
    LCD_NYB(0x30,0);              //Required for initialisation
    LCD_NYB(0x30,0);            //Required for initialisation
    DelayMS(5);                 //required delay
    LCD_NYB(0x30,0);            //Required for initialisation
    DelayMS(1);                 //required delay
    LCD_DATA(0x02,0);           //set to 4 bit interface, 1 line and 5*7 font
    LCD_DATA(0x28,0);           //set to 4 bit interface, 2 line and 5*10 font
    LCD_DATA(0x0c,0);           //set to 4 bit interface, 2 line and 5*7 font
    LCD_DATA(0x01,0);           //clear display
    LCD_DATA(0x06,0);           //move cursor right after write
}

/*
 * @brief function which clears the LCD display
 */
void ClearDisplay(){
	 LCD_DATA(0x01,0);
}


/*
 * @brief function which writes a single character to LCD
 * @param 8 bit representation of the character ACSII
 * @param Mode of data sent to LCD.
 */
void LCD_DATA(unsigned char data,unsigned char type){

    WaitLCDBusy();                  //TEST LCD FOR BUSY

    if(type == CMD){
        CMD_PORT &= ~(LCD_RS);                 //COMMAND MODE
    } else {
        CMD_PORT |= LCD_RS;                 //CHARACTER/DATA MODE
    }

    LCD_NYB(data>>4,type);               //WRITE THE UPPER NIBBLE
    LCD_NYB(data,type);                  //WRITE THE LOWER NIBBLE
}

/*
 * @brief function to wait some seconds in case the LCD is busy with something else
 */

void WaitLCDBusy(void){
    DelayMS(1);              //DELAY 1 MilliSeconds
}
/*
 * @brief function to write 4 bits of data for a character to LCD
 * @param 4 bit (half of the 8) ASCII representation of character
 * @param whether command or data to display on LCD
 */

void LCD_NYB(unsigned char nyb,unsigned char type){
    DATA_PORT &= DATA_CLR;    //LCD_PORT &= 0xF0;                     //CLEAR LOWER PORT NIBBLE
    DATA_PORT |= (nyb<<DATA); //LCD_PORT |= (nyb & 0x0F);             //SEND DATA LINE THE INFO

    if(type == CMD){
        CMD_PORT &= ~(LCD_RS);                 //COMMAND MODE
    } else {
        CMD_PORT |= LCD_RS;                 //CHARACTER/DATA MODE
    }

    CMD_PORT |= LCD_E;    //LCD_E = 1;          //ENABLE LCD DATA LINE
    DelayMS(2);                //SMALL DELAY
    CMD_PORT &= ~(LCD_E); //LCD_E = 0;       //DISABLE LCD DATA LINE
}

/*
 *@brief function which takes pointer to text to get ASCII representation of each character
 *@param pointer to text
 */
void LCD_STR(const char *text){
    while(*text){
        LCD_DATA(*text++,1);
    }
}
/*
 * @brief function which converts float to a string to output to LCD
 * @param the float to show to the LCD
 * @param the line on LCD the float should be shown
 */

void LCD_Digit(float freqFloat,int line){
	char freqString[16];
	int i = 0;
	uint32_t freqInt=0;

	for(i=0;i<16;i++){
		freqString[i]=' ';
	}
	freqString[4]='.';

	//This is to allow for 2 decimal places
	freqInt=(uint32_t)(freqFloat*100);
	//To get each digit's character add 48 to the digit
	//Thousands
	freqString[0]=(freqInt/100000)+48;
	//Hundreds
	freqString[1]=((freqInt/10000)%10)+48;
	//Tens
	freqString[2]=((freqInt/1000)%10)+48;
	//Units
	freqString[3]=((freqInt/100)%10)+48;
	//Tenths
	freqString[5]=((freqInt/10)%10)+48;
	//Hundreths
	freqString[6]=((freqInt)%10)+48;

	LCD_LINE(line);
	LCD_STR(freqString);

}

/*
 * @brief function which converts an int to a string and adds it to the character array which is displayed on line 2 of LCD
 * @param the int to show to the LCD
 * @param pointer to the character array that stores the characters to be displayed on line 2 of the lcd
 */
void LCD_Freq(uint32_t freqInt, char * lcdLine2) {
	int i = 11, j; // Int which keeps track of the position where the character must be place in the array
	int temp; // Temporarily stores the digit to be added to the character array
	int placedFirstVal = 0; // If 0, the first value has not been placed yet, if 1, the first value has been placed

	//To get each digit's character add 48 to the digit
	//Thousands
	temp = (freqInt/1000)+48;
	if (temp > 48) {
		lcdLine2[i] = temp;
		++i;
		placedFirstVal = 1;
	}
	//Hundreds
	temp = ((freqInt/100)%10)+48;
	if (temp > 48 || placedFirstVal == 1) {
		lcdLine2[i] = temp;
		++i;
		placedFirstVal = 1;
	}
	//Tens
	temp = ((freqInt/10)%10)+48;
	if (temp > 48 || placedFirstVal == 1) {
		lcdLine2[i] = temp;
		++i;
		placedFirstVal = 1;
	}
	//Units
	temp = (freqInt%10)+48;
	// Units are always displayed, so no need for if statement
	lcdLine2[i] = temp;
	++i;

	// Clear any remaining characters
	for (j = i; j < 15; ++j) {
		lcdLine2[j] = ' ';
	}
}

void LCD_STR2(unsigned char *text){
    while(*text)
    {
        LCD_DATA(*text++,1);
    }
}
/*
 * @brief function to send a command to LCD showing the line to put next string on LCD
 * @param the line on which the next string is to be shown on the LCD (1 OR 2)
 */
void LCD_LINE(char line){
    switch(line){
        case 0:
        case 1:
            LCD_DATA(LINE1,0);
            break;
        case 2:
            LCD_DATA(LINE2,0);
            break;
    }
}
/*
 * @brief function to delay processor
 */
void DelayMS(uint32_t milli)
{
	//Volaltile so compiler leaves it alone
	volatile uint32_t delay = milli * 17612;              // approximate loops per ms at 168 MHz, Debug config
	for(; delay != 0; delay--);
}
/*
 * @brief function to initialise the peripherals to be used for the LCD
 */
void initDiscovery(void)
{

  RCC->AHB1ENR = RCC_AHB1ENR_GPIOEEN;

  while((RCC->CR & RCC_CR_HSERDY)==0);

  GPIOE->MODER |= 0x15540000; //using port A pins 9 to 14

  GPIOE->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR10_1 | GPIO_OSPEEDER_OSPEEDR11_1 | GPIO_OSPEEDER_OSPEEDR12_1
		  	  	  	  | GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1);

}

