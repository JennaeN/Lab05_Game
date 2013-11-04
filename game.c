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

void writeCommandByte(char commandByte);
void writeDataByte(char dataByte);
void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char location, unsigned char buttonPushed) {

	if (buttonPushed == 1 && location != 0x87) {
		clearPlayer(location);
		location++;
		printPlayer(location); //Not yet created - going to replace the following in each if statement below

	}

	else if (buttonPushed == 2 && (location != 0x80 || location != 0xC0)) {
		clearPlayer(location);
		location--;
		printPlayer(location);
	}

	else if (buttonPushed == 3 && location > 0x87) {
		clearPlayer(location);
		location = location - 0x40;
		printPlayer(location);
	}

	else if (buttonPushed == 4 && location < 0xC0) {
		clearPlayer(location);
		location = location + 0x40;
		printPlayer(location);
	}


return location;
}

char didPlayerWin(unsigned char player) {
return player == 0xC7;
}

void LCDclr();
void print2LineMessage(char * string1, char * string2){
LCDclr();
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
