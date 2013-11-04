/*
 * game.c
 *
 *  Created on: Nov 3, 2013
 *      Author: C15Jennae.Steinmille
 */

#include <msp430g2553.h>
#include "game.h"
#include "LCD.h"

unsigned char initPlayer() {
	return 0x80;
}

void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char buttonPushed) {

	if (buttonPushed == 1 && location != 0x87) {
		location += location;
		printPlayer(); //Not yet created - going to replace the following in each if statement below
		LCDclr;
		writeCommandByte(location);
		writeString(player);
	}

	else if (buttonPushed == 2 && (location != 0x80 || location != 0xC0)) {
		location -= location;
		printPlayer();
	}

	else if (buttonPushed == 3 && location > 0x87) {
		location = location - 0x40;
		printPlayer();
	}

	else if (buttonPushed == 4 && location < 0xC0) {
		location = location + 0x40;
		printPlayer();
	}
}

return player;
}

char didPlayerWin(unsigned char player) {
return player == 0xC7;
}

void print2LineMessage(char * string1, char * string2){
cursorToLineOne();
writeString(string1);
cursorToLineTwo();
writeString(string2);
}

//Returns a different integer corresponding to the button pushed.
int pollButton4() {

	int pollBtn = 0;
	while (pollBtn == 0) {
		if ((P1IN & BIT1)== 0){
			pollBtn = 1;
			return pollBtn;
		}

		if ((P1IN & BIT2)== 0){
			pollBtn = 2;
			return pollBtn;
		}

		if ((P1IN & BIT3)== 0){
			pollBtn = 3;
			return pollBtn;
		}

		if ((P1IN & BIT4)== 0){
					pollBtn = 4;
					return pollBtn;
				}
	}
	return pollBtn;
}
