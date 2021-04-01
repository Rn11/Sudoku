// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "io.h"
#include "fcntl.h"
#include "wchar.h"
#include "windows.h"


// Arrays that are used to print the indexes of rows / columns
const char colIndexLabel[9] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I' };
const char rowIndexLabel[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const char* fillGameboard() {
	// we need a static value here since we can't return an array in C, see https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm
	
	// we'll start with a valid sudoku board
	static char gameBoard[9][9] =
	{
		{ '1','2','3','4','5','6','7','8','9' },
		{ '4','5','6','7','8','9','1','2','3' },
		{ '7','8','9','1','2','3','4','5','6' },
		{ '2','3','1','5','6','4','8','9','7' },
		{ '5','6','4','8','9','7','2','3','1' },
		{ '8','9','7','2','3','1','5','6','4' },
		{ '3','1','2','6','4','5','9','7','8' },
		{ '6','4','5','9','7','8','3','1','2' },
		{ '9','7','8','3','1','2','6','4','5' },
	};

	/** note that the game board array does NOT reset as it's static.
	Running this method more than once will ultimately result in a full grid.
	It will only replace values of grid fields. To empty grid / gameBoard, iterate all fields (rows + cols)
	and assign '\0' !
	*/

	int seed = rand() % 333 + 1 + '0';
	// run this loop 100 times and call methods to randomize game board (swapping, permutation)
	for (int y = 0; y < 100; y++) {
			// calculate a new value to re-seed
			seed = (seed % 101) + (((y*y) * y) * 3) - (rand() % 35 + 1 + '0');

			// calling permutation method to swap values
			permutateNumbers(gameBoard, seed);
		}
	return gameBoard;
}

/**
Permutates 2 numbers within the game board array
@param static game board generated for each session
@param specifies the input used to get a new seed
*/
int permutateNumbers(char gameBoard[9][9], int seed) {
	//re-seed random
	srand(seed);

	// get two that will be swapped within the array
	char firstReplacementValue = (rand() % 9 + 1) + '0';
	char secondReplacementValue = (rand() % 9 + 1) + '0';

	// iterate all cells of array and swap values
	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			// if value is equal to our first value, change it to the second value
			if (gameBoard[x][y] == firstReplacementValue) {
				gameBoard[x][y] = secondReplacementValue;
			}
			// if the current value is equal to our second value already, make it the first value instead
			else if (gameBoard[x][y] == secondReplacementValue) {
				gameBoard[x][y] = firstReplacementValue;
			}
		}
	}
}

void printBoard(char gameBoard[9][9]) {
	// print column index label first
	// print spaces to adjust offset of column label
	wprintf(L"    ");
	for (int i = 0; i < 9; i++) {
		// print a pipe followed by the column index label ( | A | B | ...)
		if (i % 3 == 0 && i != 0) {
			// print block borders bold and red
			setColor('r');
			wprintf(L" ┃ ");
			setColor('d');
		}
		else if (i != 0) {
			wprintf(L"   ");
		}
		wprintf(L"%c", colIndexLabel[i]);
	}

	// outer for loop, handling the rows
	for (int y = 0; y < 9; y++) {
		// print in next row
		wprintf(L"\n");

		// every 3rd row, print different border seperator so we can build segments / blocks and color them red
		if (y % 3 == 0 && y != 0) {
			setColor('r');
			wprintf(L"━━┿━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┥");
			setColor('d');
		}
		else if (y == 0) {
			setColor('r');
			wprintf(L"  ┌───┬───┬───╂───┬───┬───╂───┬───┬───┐");
			setColor('d');
		}
		// otherwise use normal row seperator
		else {
			// we need to print 1st, 3rd and 5th vertical separator with a different char(s) and in red
			// starting with leading separator
			setColor('r');
			wprintf(L"  │");
			setColor('d');

			wprintf(L"───┼───┼───"); // inner separator for 1st block column

			// border separator of 1st block
			setColor('r');
			wprintf(L"╂");
			setColor('d');

			wprintf(L"───┼───┼───"); // inner separator for 2nd block column

			// border separator of 2nd block
			setColor('r');
			wprintf(L"╂");
			setColor('d');

			wprintf(L"───┼───┼───"); // inner separator for 3rd block column

			// outer border separator of 3rd (last) block
			setColor('r');
			wprintf(L"│");
			setColor('d');
		}
		// move to a new line and print row index label
		wprintf(L"\n");
		wprintf(L"%c", rowIndexLabel[y]);

		// inner loop handling column / cell printing
		for (int x = 0; x < 9; x++) {
			// every 3rd column print block separator in red
			if (x % 3 == 0 && !(x == 0)) {
				setColor('r');
				wprintf(L" ┃ ");
				setColor('d');
			}
			else if (x == 0) {
				setColor('r');
				wprintf(L" │ ");
				setColor('d');
			}
			else {
				wprintf(L" │ "); // use standard column separator for all other cells
			}
			// since we're using char arrays now, we need to check for null values ('\0')
			if (gameBoard[y][x] != '\0') {
				wprintf(L"%c", gameBoard[y][x]); // print out value of game board
			}
			else {
				wprintf(L"%c", ' '); // print out spacing if value is '\0'
			}
		}
		// print trailing outmost right border separator after each row
		setColor('r');
		wprintf(L" │");
		setColor('d');
	}
	// print trailing table border
	setColor('r');
	wprintf(L"\n  └───┴───┴───┸───┴───┴───┸───┴───┴───┘");
	setColor('d');
}



/**
Changes output color of the console
@param sets color, possible values are r, g, y, b, d
*/
setColor(char col) {
	switch (col) {
		// red
	case 'r': wprintf(L"\x1b[31m");; break;
		// green
	case 'g': wprintf(L"\x1b[32m");; break;
		// yellow
	case 'y': wprintf(L"\x1b[33m"); break;
		// blue
	case 'b': wprintf(L"\x1b[34m"); break;
		// default color
	case 'd': wprintf(L"\x1b[m"); break;
	default: wprintf("Error setting color"); break;
	}
}


//////////////////////////////////////////////////////
// Start of game logic block ////////////////////////
////////////////////////////////////////////////////
/* TODO: Write proper methods to check after AND before current position of x, instead of iterating through whole
	column / row each time. These 2 methods below are only useful, if we want to check a complete row / col instead of
	checking before and after a certain position, which is needed when generating. However, code might be useful, somehow (?)
*/
// use this to check col if duplicate occured, probably unnecessary? 
bool isDuplicateVertically(char gameBoard[9][9], int yCurrIndex) {
	int count = 9;
	for (int x = 0; x < count - x; x++) {
		for (int j = x + 1; j < count - 1; j++) {
			if (gameBoard[yCurrIndex][x] == gameBoard[yCurrIndex][j] && gameBoard[yCurrIndex][x] != '\0') {
				return true;
			}
		}
	}
	return false;
}
// use this to check row if duplicate occured, probably unnecessary? 
bool isDuplicateHorizontally(char gameBoard[9][9], int xCurrIndex) {
	int count = 9;
	for (int y = 0; y < count - y; y++) {
		for (int j = y + 1; j < count - 1; j++) {
			if (gameBoard[y][xCurrIndex] == gameBoard[y][xCurrIndex] && gameBoard[y][xCurrIndex] != '\0') {
				return true;
			}
		}
	}
	return false;
}

// TODO: Code below isn't working at all, hard to iterate through block properly with the least effort. Need to rewrite code from scratch probably
bool isDuplicateInBlock(char gameBoard[9][9], int xCurrIndex, int yCurrIndex) {
	// correct starting postion of block accordingly
	// if we're in first column of blocks
	if (xCurrIndex <= 2) {
		xCurrIndex = 0;
	}
	// if we're in second column of blocks
	else if (xCurrIndex <= 5) {
		xCurrIndex = 3;
	}
	// if we're in third column of blocks
	else {
		xCurrIndex = 6;
	}

	// if we're in first row of blocks
	if (yCurrIndex <= 2) {
		yCurrIndex = 0;
	}
	// if we're in second row of blocks
	else if (yCurrIndex <= 5) {
		yCurrIndex = 3;
	}
	// if we're in third row of blocks
	else {
		yCurrIndex = 6;
	}
	// we need to iterate all 9 fields
	int count = 9;
	for (int cellOfBlock = 0; cellOfBlock < 9; cellOfBlock++) {
		for (int x = xCurrIndex; x < x + 3; x++) {
			for (int y = 0; y < y + 2; y++) {
				if (gameBoard[y][x] == gameBoard[y + 1][x] && gameBoard[y][x] != '\0')
				{
					return true;
				}
				// if we're not in the last row but in last column, reset column
				if (x == 2 && y != 2) {
					x = 0;
				}
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////
// End of game logic block //////////////////////////
////////////////////////////////////////////////////

int main()
{
	// we can use utf16 chars by setting mode here
	_setmode(_fileno(stdout), _O_U16TEXT);


	// definition is needed, otherwise ENABLE_VIRTUAL_TERMINAL_PROCESSING is undefined 
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	char *gameBoard;	// board array 9x9

	bool playGame = true; // bool to control if user wants to play again

	// main game loop
	while (playGame) {
		srand(time(0)); // randomize seed

		// memcpy doesn't work for some reason :-)
		//memcpy(gameBoard, fillGameboard(), sizeof (gameBoard));

		gameBoard = fillGameboard(); // get new gameboard

		wprintf(L"\nSpiel startet, viel Erfolg!\n\n");

		printBoard(gameBoard); // print out board formatted with separators


		// testing if game logic method works, seems to work but needs changes to our use-case
		isDuplicateVertically(gameBoard, 0);


		// ask user if he wants to play again
		wprintf(L"\n\nNeue Runde? (y/n)\n");
		char input; // TODO: move this to the beginning of the code and rename(?)

					/* notice the leading space before %c, we need that otherwise printing
					a newline would be read as input somehow, effectively skipping user input*/
		wscanf_s(L" %c", &input);

		// TODO: Application crashes when user entered 'n', idk why
		// to prevent that from happening, we call exit(0);
		if (input == 'n')
		{
			exit(0);
			playGame = false; // if app should close / exit after user chose 'n', this code can be removed
		}
	}
	return 0;
}