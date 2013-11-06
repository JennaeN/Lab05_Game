#include <msp430.h> 
#include "LCD.h"
#include "game.h"
#include "random.h"
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


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	static const char seed = 1234;

	char * you = "YOU     ";
	char * win = "WIN!    ";
	char * game = "GAME    ";
	char * over = "OVER!   ";
	char * boom = "BOOM!   ";
	char * space = "        ";

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

	char random = prand(seed);	//after this, random = prand(random);
	//random = prand(random);
	char mine1Location = 0x81 + random%7;
	random = prand(random);
	char mine2Location = 0xC0 + random%7;


	printMine(mine1Location);
	while(mine2Location == mine1Location || mine2Location == (mine1Location +0x40) || mine2Location == (mine1Location +0x41) || mine2Location == (mine1Location +0x3F)){
		random = prand(random);
		mine2Location = 0xC0 + random%7;
	}
	printMine(mine2Location);

	while (1) {

		if (location == 0xC7) {
			print2LineMessage(you, win);
			//Figure out some way to make the flag stay 0 here, so winner is displayed only.
			flag = 5;


			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
			mine1Location = 0x81 + random%7;
			printMine(mine1Location);
			while(mine2Location == mine1Location || mine2Location == (mine1Location +0x40) || mine2Location == (mine1Location +0x41) || mine2Location == (mine1Location +0x3F)){
				random = prand(random);
				mine2Location = 0xC0 + random%7;
			}
			printMine(mine2Location);
		}

		if (location == mine1Location || location == mine2Location){
			flag = 4;
			print2LineMessage(boom, space);
			__delay_cycles(444444);
		}

		if (flag == 4) {
			print2LineMessage(game, over);
			flag = 5;

			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
			flag = 0;
			mine1Location = 0x81 + random%7;
			printMine(mine1Location);
			while(mine2Location == mine1Location || mine2Location == (mine1Location +0x40) || mine2Location == (mine1Location +0x41) || mine2Location == (mine1Location +0x3F)){
				random = prand(random);
				mine2Location = 0xC0 + random%7;
			}
			printMine(mine2Location);
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
