#include <msp430.h> 
#include "LCD.h"
#include "game.h"
/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	//unsigned char win = 0;

	unsigned char location = 0x80;
	int buttonPushed = 0;
	char * player = "*";
	char * you = "YOU";
	char * win = "WIN!";
	char * game = "GAME";
	char * over = "OVER!";

	//SPI initialization function
	initSPI();

	// LCD initialization function
	LCDinit();

	//Clear the LCD screen
	LCDclr();

	while (true) {
		buttonPushed = pollButton4();

		if (location == C7) {
			print2LineMessage(you, won);

			//win =1;
			//while(win == 1){
			//A different value is returned depending on which button is pushed.
			buttonPushed = pollButton4();
			initPlayer();
			//	win = 0;
			//}
		} else {
			movePlayer(buttonPushed);//Not yet created - going to contain all of the below
		}

	}

	if (interrupt == 1) {
		print2LineMessage(game, over);

		//Used to restart game
		buttonPushed = pollButton4();
		initPlayer();
	}
	return 0;
}
