/*
 * game.h
 *
 *  Created on: Nov 3, 2013
 *      Author: C15Jennae.Steinmiller
 *
 *      Description: This header is the link between main.c and game.c.
 *      It initializes functions called in main.c and defined in game.c.
 */

#ifndef GAME_H_
#define GAME_H_

#define ROW_MASK 0x40

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char player);

//
//Prints a mine (X) at the location passed in.
//
void printMine(unsigned char mine);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char player);

//
// Given a player's current position and a direction, returns an updated player position.
//
char movePlayer( char location, int mod);

//
//Prints a message on the top and bottom line of the LCD screen
//
void print2LineMessage(char * string1, char * string2);

//
// Moves the player only after a button has been pushed
//
char movePlayerInResponseToButtonPush(unsigned char buttonToTest);

//
// Delays to allow for debounce
//
void debounce();

#endif /* GAME_H_ */
