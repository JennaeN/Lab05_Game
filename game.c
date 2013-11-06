/*
 * game.c
 *
 *  Created on: Nov 3, 2013
 *      Author: C15Jennae.Steinmille
 */

#include <msp430g2553.h>
#include "game.h"
#include "LCD.h"

//Initialized the player to the starting position
unsigned char initPlayer() {
	return 0x80;
}

void writeCommandByte(char commandByte);
void writeDataByte(char dataByte);

//Draw the player in the specified position
void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

//Clears the LCD screen of the old position
void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}


char movePlayer(char location, int mod) {

	//If the player location is within bounds, print the updated player.
	if (location == 0x87 || location == 0x86 || location == 0x85
			|| location == 0x84 || location == 0x83 || location == 0x82
			|| location == 0x81 || location == 0x80 || location == 0xC7 || location == 0xC6
			|| location == 0xC5 || location == 0xC4 || location == 0xC3
			|| location == 0xC2 || location == 0xC1 || location == 0xC0) {

		printPlayer(location);
	}
	else {
		location -= mod;
		printPlayer(location);
	}
 return location;
}

char didPlayerWin(unsigned char player) {
	return player == 0xC7;
}

void LCDclr();
void print2LineMessage(char * string1, char * string2) {
	LCDclr();
	cursorToLineOne();
	writeString(string1);
	cursorToLineTwo();
	writeString(string2);
}

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

void debounce(){
	__delay_cycles(1000);
}

//Returns a different integer corresponding to the button pushed.
int pollButton4() {

	int pollBtn = 0;
	while (pollBtn == 0) {
		if ((P1IN & BIT1)== 0){
			pollBtn = 1;
		}

		if ((P1IN & BIT2)== 0){
			pollBtn = 2;
		}

		if ((P1IN & BIT3)== 0){
			pollBtn = 3;
		}

		if ((P1IN & BIT4)== 0){
			pollBtn = 4;
		}
	}

	return pollBtn;
}
