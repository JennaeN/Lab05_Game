/*******************************************
 * Author: C2C Jennae Steinmiller
 * Created: 03 November, 2013
 * Description: Main.c is used to handle the overall flow of information in Lab05_Game.
 * It calls game.h and game.c in order to carry out functionality. As a whole, Lab05_Game
 * is a game in which the user tries to maneuver to the end point, winning the game. The are two random mines placed in each level which move every two seconds. The top mine moves left while the bottom mine moves right.
 * The player maneuvers by pushing one of four pushbuttons at a time. Button 1 moves the player to the right, button 2 moves the player to the left, button 3 moves the player up, and button 4 moves the player down.
 * If the player does not move within 2 seconds of their previous turn, the game is over. The other method for ending the game is to hit one of the mines.
 * Documentation: I received a lot of instructor help.
 *******************************************/

#include <msp430.h> 
#include "LCD.h"
#include "game.h"
#include "random.h"
/*
 * main.c
 */

//Create global variables
char location = 0x80;
char flag = 0;
char increment = 0;

//Define functions
void init_timer();

void init_buttons() {
	P1DIR &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P1IES |= BIT1 | BIT2 | BIT3 | BIT4;
	P1IFG &= ~(BIT1 | BIT2 | BIT3 | BIT4);
	P1IE |= BIT1 | BIT2 | BIT3 | BIT4;
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//Use seed to create a random number
	static const char seed = 1234;

	//Define all of the strings that will be used in the program
	char * you = "YOU     ";
	char * win = "WIN!    ";
	char * game = "GAME    ";
	char * over = "OVER!   ";
	char * boom = "BOOM!   ";
	char * space = "        ";

	//Initialize the timer
	init_timer();

	//Initialize the buttons
	init_buttons();

	//Enable the interrupt
	__enable_interrupt();

	//SPI initialization function
	initSPI();

	// LCD initialization function
	LCDinit();

	//Clear the LCD screen
	LCDclr();

	//Place the player at the starting location
	printPlayer(location);

	//Create random mine placement
	char random = prand(seed);
	random = prand(random);
	char mine1Location = 0x81 + random % 7;
	random = prand(random);
	char mine2Location = 0xC0 + random % 7;

	//print the first mine, and check to make sure the second mine is printed in a place that the user can still win the game
	printMine(mine1Location);
	while (mine2Location == mine1Location
			|| mine2Location == (mine1Location + 0x40)
			|| mine2Location == (mine1Location + 0x41)
			|| mine2Location == (mine1Location + 0x3F)) {
		random = prand(random);
		mine2Location = 0xC0 + random % 7;
	}
	printMine(mine2Location);

	//during the rest of the game
	while (1) {

		//This is the end of the level - player wins!
		if (location == 0xC7) {
			print2LineMessage(you, win);
			//Set flag and increment to a value above 4 so that 'game over' is not printed
			flag = 5;
			increment = 5;

			//Reset the game for the next level
			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
			mine1Location = 0x81 + random % 7;
			printMine(mine1Location);
			while (mine2Location == mine1Location
					|| mine2Location == (mine1Location + 0x40)
					|| mine2Location == (mine1Location + 0x41)
					|| mine2Location == (mine1Location + 0x3F)) {
				random = prand(random);
				mine2Location = 0xC0 + random % 7;
			}
			printMine(mine2Location);
			increment = 0;
		}

		//The game is over if the player hits a mine
		if (location == mine1Location || location == mine2Location) {
			flag = 4;
			print2LineMessage(boom, space);
			__delay_cycles(444444);
		}

		//If two seconds have passed, increment the mines to move in the opposite directions of each other
		if (increment == 4) {
			clearPlayer(mine1Location);
			clearPlayer(mine2Location);
			mine1Location--;
			mine2Location++;
			printMine(mine1Location);
			printMine(mine2Location);
			increment = 0;
		}

		//If the player loses, print 'game over'
		if (flag == 4) {
			print2LineMessage(game, over);
			flag = 5;
			increment = 5;

			//Reset the game for a new level
			while (flag > 4) {
			}
			LCDclr();
			location = initPlayer();
			printPlayer(location);
			flag = 0;
			increment = 0;
			mine1Location = 0x81 + random % 7;
			printMine(mine1Location);
			while (mine2Location == mine1Location
					|| mine2Location == (mine1Location + 0x40)
					|| mine2Location == (mine1Location + 0x41)
					|| mine2Location == (mine1Location + 0x3F)) {
				random = prand(random);
				mine2Location = 0xC0 + random % 7;
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
	increment++;
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

//Used to reset the timer
void clearTimer() {
	flag = 0;
}

//Function used to determine which button has been pushed without polling
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
				//This code is used to prevent the player (*) from moving outside of the game bounds.
				int mod = 0;
				mod = movePlayerInResponseToButtonPush(buttonToTest);
				clearPlayer(location);
				location += mod;
				location = movePlayer(location, mod);
				clearTimer();
			}

			//Debounces the button
		} else {
			debounce();
		}

		//Toggles between reading rising and falling edge
		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}

}

//Create an interrupt which will test each button to see if it has been pushed.
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR() {
	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);

}
