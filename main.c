#include <msp430.h> 
#include "LCD.h"
#include "game.h"
/*
 * main.c
 */
char location = 0x80;
char flag = 0;         // global variable to share info between main and ISR

void init_timer();
void init_buttons() {
	P1DIR &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P1IES |= BIT1 | BIT2 | BIT3 | BIT4;
	P1IFG &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P1IE |= BIT1 | BIT2 | BIT3 | BIT4;
}

//void restartOnButtonPush() {
	//TACTL |= TAIE;
	//LCDclr();
	//location = initPlayer();
	//printPlayer(location);
	//flag = 0;
//}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char * you = "YOU     ";
	char * win = "WIN!    ";
	char * game = "GAME    ";
	char * over = "OVER!   ";

	init_timer();

	init_buttons();

	__enable_interrupt();

	//SPI initialization function
	initSPI();

	// LCD initialization function
	LCDinit();

	//Clear the LCD screen
	LCDclr();

	printPlayer(location);

	while (1) {
		//__interrupt void PORT1_ISR();
		if (location == 0xC7) {
			print2LineMessage(you, win);
			//Figure out some way to make the flag stay 0 here, so winner is displayed only.
			flag = 5;
			//TACTL &= ~(MC1 | MC0);
			//restartOnButtonPush();

			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
		}
		if (flag == 4) {
			print2LineMessage(game, over);
			flag = 5;
			//Used to restart game
			//restartOnButtonPush();
			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
			flag = 0;
		}
	}

}

//timer A ISR

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	flag++;
}

void init_timer() {
	TACTL &= ~(MC1 | MC0);        // stop timer

	TACTL |= TACLR;             // clear TAR

	TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?

	TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?

	TACTL &= ~TAIFG;            // clear interrupt flag

	TACTL |= MC1;               // set count mode to continuous

	TACTL |= TAIE;              // enable interrupt
}

void clearTimer() {
	flag = 0;
}

void testAndRespondToButtonPush(char buttonToTest) {
	if (buttonToTest & P1IFG) {

		if (buttonToTest & P1IES) {

			if (flag == 5) {
				LCDclr();
				location = initPlayer();
				printPlayer(location);
				flag = 0;
			}

			else {
				int mod = 0;
				mod = movePlayerInResponseToButtonPush(buttonToTest);
				clearPlayer(location);
				location += mod;
				location = movePlayer(location, mod);
				//printPlayer(location);
				clearTimer();
			}
		} else {
			debounce();
		}

		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR() {
	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);

}
