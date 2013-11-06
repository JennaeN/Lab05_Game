Lab05_Game
==========

Creation of a simple game.

--------------------------------------------------------------------------------------------------------------------
GRADING

Item          	Grade                                                                                 	Points       	Out of 	    Date Due

Prelab        	On-Time: 0 ---- Check Minus ---- Check ---- Check Plus                     		5      		BOC L29

Required Functionality 	On-Time-- Late: 1Day ---- 2Days ---- 3Days ---- 4+Days 		35 		    COB L30

B Functionality 	On-Time---------- Late: 1Day ---- 2Days ---- 3Days ---- 4+Days 		10 		     COB L30

A Functionality 	On-Time---------- Late: 1Day ---- 2Days ---- 3Days ---- 4+Days 		10 		    COB L30

Use of Git / Github 	On-Time: 0 ---- Check Minus ---- Check ---- Check Plus 		     10     		BOC L27

Lab Notebook 	On-Time: 0 ---- Check Minus ---- Check ---- Check Plus ----- 
                          Late: 1Day ---- 2Days ---- 3Days ---- 4+Days 		                              30     		COB L31
                          
Total                                                                   100 	


DETAILS: THE BASIC IDEA
--------------------------------------------------------------------------------------------------------------------

In this game, the player will be represented by an asterisk (*), starting in the top left corner of the screen. By using push buttons, the player advances through the board. The goal of the player is to make it to the bottom right corner of the screen.

On your Geek Box, PB1=right, PB2=left, PB3=up, and PB4=down. Your game shouldn't allow players to go outside the bounds of the screen.

If the player doesn't move within 2 seconds, the game ends - display "GAME" on the first line and "OVER!" on the second line. If the player reaches the bottom right corner of the screen, they won - display "YOU" on the first line" and "WON!" on the second line. At this point, pushing any of the buttons should initiate a new game.

To achieve A and B functionality, you'll have to add some additional features to the game.


REQUIRED FUNCTIONALITY
---------------------------------------------------------------------------------------------------------------------

To achieve Required Functionality, you must implement the game as described in The Basic Idea.

To allow you to move faster, I've provided you skeleton code.

You'll also need to interface with the LCD and buttons. In the spirit of open source software, you're free to use any LCD library you find on Github. You can also use any button library you find on Github.

The two-second between-move time limit must also be handled via an interrupt and the Timer_A subsystem.

Remember, program time spent inside ISRs should be minimized! Remember to effectively debounce your buttons!

You must place your code under version control on git and push your repo to Github.


B FUNCTIONALITY
---------------------------------------------------------------------------------------------------------------------

Your game must handle button presses via interrupts! Functionality will be identical to Required.

Remember the strategy we discussed in class! We can initially trigger a button on the falling edge, then switch the trigger to rising edge to detect the release. Here's some sample code:



void testAndRespondToButtonPush(char buttonToTest)

{

    if (buttonToTest & P1IFG)

    {

        if (buttonToTest & P1IES)

        {

            movePlayerInResponseToButtonPush(buttonToTest);

            clearTimer();

        } else

        {

            debounce();

        }    

        P1IES ^= buttonToTest;

        P1IFG &= ~buttonToTest;

    }

}


A FUNCTIONALITY
---------------------------------------------------------------------------------------------------------------------

Mines (10pts)

Your game must place two mines in random positions on the board (represented by an "x") that players must navigate around. If a player navigates onto a bomb, the game is over. Display a creative message if a player steps on a mine (i.e. "BOOM!"), then the game over screen.

Randomness is hard! I found a random number library for the MSP430 on github: https://github.com/0/msp430-rng . Use it! The rand() function generates a random seed. prand() uses this seed to generate subsequent pseudorandom numbers. You should only call rand() once, then let prand() generate additional numbers you may need. You'll need to store each result of prand() to use as the seed for the next time you call prand().

Include logic in your program that ensures the game is winnable! You can't have obstacles stacked on top of or diagonal to one another - players couldn't get around them.
Additional Feature (5pts)

This game is too easy! Add an additional feature that makes the game more challenging. Be creative! Simply making the time limit shorter will not receive any credit.

Note: There's the potential for 5 bonus points if you implement all of the features of A Functionality.




TESTING
---------------------------------------------------------------------------------------------------------------------

1. Ran program to see if screen would initialize. Then continued by printing to the screen and working with buttons. 
2. Button/string functionality began working with little effort.
3. Discovered problems stopping the game - added B functionality code and made major changes to the layout of my code.
4. After the code was runnable again, I had some problems with the buttons moving the player and the clock restarting. To fix these problems I had to rearrange my code so that everything was being called in the correct order.
5. When restarting the game after a win, game over would still be shown after 2 seconds. I had to stop the clock from counting to 4 (so I set the flag equal to 5).
6. More problem occured with the time of the game restart. In the end, I made while loops that did nothing until a button was pushed, then set if statements for when the flag was great than 4 to completely fix the code.
7. A functionality tested by playing the game. Only 1 mine appearing, and game over/win not being detected.
8. Got 2 mines to appear by updating random in the while loop. This also helped with detecting game over/wins on the first iteration of the game.  No mines appear after the first win/loss.
9. Detection working. Must add "BOOM!" and make the mines more random.
10. "BOOM!" added and working properly. Mines have been made more random. A functionality working properly.
