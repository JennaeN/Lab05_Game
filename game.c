/*
 * game.c
 *
 *  Created on: Nov 3, 2013
 *      Author: C15Jennae.Steinmiller
 *
 *      Description:Contains the definitions of functions that can be found in game.h.
 */

#include <msp430g2553.h>
#include "game.h"
#include "LCD.h"

//
//Initialized the player to the starting position
//
unsigned char initPlayer() {
	return 0x80;
}

void writeCommandByte(char commandByte);
void writeDataByte(char dataByte);

//
//Draw the player in the specified position
//
void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

//
//Draw the mine in the specified position
//
void printMine(unsigned char mine) {
	writeCommandByte(mine);
	writeDataByte('X');
}

//
//Clears the LCD screen of the old position
//
void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}

//
//Checks to see if the player will be moving out of bounds before moving the player
//
char movePlayer(char location, int mod) {

	//If the player location is within bounds, print the updated player.
	if (location == 0x87 || location == 0x86 || location == 0x85
			|| location == 0x84 || location == 0x83 || location == 0x82
			|| location == 0x81 || location == 0x80 || location == 0xC7
			|| location == 0xC6 || location == 0xC5 || location == 0xC4
			|| location == 0xC3 || location == 0xC2 || location == 0xC1
			|| location == 0xC0) {

		printPlayer(location);

		//If not, leave the location the same as it was and print the player there
	} else {
		location -= mod;
		printPlayer(location);
	}
	return location;
}

//
//Function used to print 'you win,' 'game over,' and 'boom'
//
void LCDclr();
void print2LineMessage(char * string1, char * string2) {
	LCDclr();
	cursorToLineOne();
	writeString(string1);
	cursorToLineTwo();
	writeString(string2);
}

//
//Moves player by changing the value of modification only if a button has been pushed.
//
char movePlayerInResponseToButtonPush(unsigned char buttonToTest) {

	char modification = 0;
	if (buttonToTest == BIT1) {
		modification++;
	}

	else if (buttonToTest == BIT2) {
		modification--;
	}

	else if (buttonToTest == BIT3) {
		modification = modification - 0x40;
	}

	else if (buttonToTest == BIT4) {
		modification = modification + 0x40;
	}

	return modification;
}

//
//Allows time for debounce to occur
//
void debounce() {
	__delay_cycles(1000);
}
