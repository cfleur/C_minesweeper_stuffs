//////////////////////////////////////////////////////////////////
//								//
//		Welcome to Cat's minesweeper code.		//
//								//
//								//
//	 ^,^	 ^,^	 ^,^	 ^,^	 ^,^	 ^,^	 ^,^	//
//								//		
//				So what's this code about?	//	
//	Glad you asked. This code creates two 2D arrays, 	//
//	one to hold the locations of the mines, and one to 	//
//	display the board to the user. The heart of this 	//
//	program is a function which uses an 2 algorithms to	//
//	check cells until mines are found. When mines are	//
//	found around a cell, the checked cell will display 	//
//	to the user how many mines are around it. Game-play 	//
// 	levels are directly perportional to size of the 	//
//	"minefield". User selects a cell by entering the 	//
//	corresponding row and column number. They are able 	//
//	to either flag or open that cell. When a mine-cell 	//
//	is opened, the game is over, and the board is 		//
//	revealed and score is revealed (number of correctly	//
//	flagged mines)? The user is then promped to play 	//
//				again. ^,^			//
//								//
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>  
#include <time.h>
#include "cat_minesweeper_clrscn.c"
//#include "cat_minesweeper_minefieldon.c"
//#include "cat_minesweeper_printboard.c"
//#include "cat_minesweeper_assigncleanboard.c"
//#include "test.c"



// Global constraints 
#define MAXSIZE 16 	// Maximum size for game play is 12, 
					// + 2 buffer rows / columns on each side.


// Global variables 
char displayboard [MAXSIZE][MAXSIZE] = {};
char mineboard [MAXSIZE][MAXSIZE] = {};
int maxmines;

// Functions 
void assigncleanboard(int size);
int cellminecount(int i, int j);
int secondarycellminecount(int i, int j);
int checkcellstatus(int i, int j);
void continuechecking(int i, int j);
void printboard();
void minefieldon(int size);
void revealboard();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														//
//					 		Main 							//
//														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main () {


	int bs=0; // bs = board size
	int i=0, j=0; // just indexes
	char colletter, rowletter; // coordinates to get from user
	int fo=0; // fo = find or open
	int allowedflags=0; // Only allow user to flag as many mines as there are, cannot be greater than maxmines
	int correctflags=0; // Count of cells correctly flagged
	int row=0, col=0; // more indexes
	int cellstatus=0; // if 1 cell is a mine, if 2 cell has already been opened and its a invalid choice,
					// if 3 cell is not a mine and choice is valid.
	int checkingerror=0; // Function continuechecking only has a certain reach so some iterations need to be performed
						 // i.e board size (aka bs) - 3	
	int playagain=0; // If playagain == 1 then run play loop again


/*********************** Start game, get game size from user, print new board. ***************************/	
do {
	clrscn();
	printf("Welcome to Minesweeper, the Cat edition.\n");
	printf("When prompted, you can either flag ( press 1 ) or open ( press 0 ) a cell.\n");
	printf("Use flaggin when you think a cell holds a mine to remind yourself which cells not to select.\n");
	printf("To unflag a cell, just select it next time and use the open option.\n");
	printf("More instructions coming soon. ^,^\n");
	
	printf("Enter the size of your minefield ~ the bigger, the harder ~ ( 6-12 ): ");
	do {

		scanf("%d", &bs);
		if(bs<(MAXSIZE-3) && bs>(MAXSIZE-11)) {
			assigncleanboard(bs);
			printboard();
		}	
		else {
			printf("Invalid number. Size can only be from 6 - 12. Try again: ");
			bs=1;
		}

	} while(bs == 1);	// Problem: if bs != a number, the loop is infinate.

/********************************************************************************************************/


/********************************************** Game play ***********************************************/	

	minefieldon(bs); // Place mines on board

// In this section:
//   Get correct input from user
// + Check selected cell status 
// 		- If the cell is a mine, game is over. Show score. Play again?
//		- If cell is not, use checking function. Then ask for input agian. 


do {
	
	/************************************ Get correct input from user ***********************************/	
	
	do {
		printf("\nSelect a cell to play.\nEnter a row letter (lowercase) : ");
		scanf(" %c", &rowletter);
		printf("\nEnter a column letter (uppercase) : ");
		scanf(" %c", &colletter);
		printf("Would you like to flag this cell or open this cell?\nFor open, enter 0, for flag enter 1: ");
		scanf("%d", &fo);

		// Convert characters from user to valid co-ordinate numbers
		i = rowletter - 95; 
		j = colletter - 63;
//		printf("%d\t%d\n", i, j);

		if(i > bs+1 || i < 2 || j > bs+1 || j < 2 || fo > 1 || fo < 0) // Input is not a valid row or column on the gameboard.
			printf("Invalid choice, try again.");

	} while(i > bs+1 || i < 2 || j > bs+1 || j < 2 || fo > 1 || fo < 0 || allowedflags > maxmines);	


	/**************************************** Do stuff with input ***************************************/	

	if(fo == 1) { // The user wants to flag the cell only.
		if(allowedflags >= maxmines) {
			printf("You ran out of flags. Play a flagged cell to get them back.\n");
			printf("allowed flags used = %d/%d\n", allowedflags, maxmines);
		}	
		else if(displayboard[i][j] == 70)
			printf("There is already a flag there, try again.\n");
		else {
			displayboard[i][j] = 70;
			printf("you flagged a cell.\n");
			allowedflags++;	
			printf("allowed flags used = %d/%d\n", allowedflags, maxmines);	
		
		if (mineboard[i][j] == 42) // See if user correctly flagged a cell.
			correctflags++; // If they change their mind later about a flagged cell and lose, not our problem.
		printf("correct flags = %d\n", correctflags);
		}
	}	

	else {
		printf("You entered row %d, column %d, and %d.\n", i,j,fo);
		
		if(displayboard[i][j] == 70)
			allowedflags--;
		
	//	printf("mine cell %c %d\n", mineboard[i][j],mineboard[i][j]);

		cellstatus = checkcellstatus(i,j);
		printf("\nCellstatus = %d\n", cellstatus);

		if(cellstatus == 1) { // Its a mine, game over.
			printf("\nSorry, you blew up. Game over. You got %d correct flags.\nPlay again? Press 1, any other key to exit.\n", correctflags);
			revealboard(); //Reveal board
			printboard();
			scanf("%d", &playagain);
		}
		else if(cellstatus == 2) // Cell has already been checked
			printf("\nTry again, that cell has already been checked.\n");
			// Get input again.

		else if(cellstatus == 0) { // Its not a mine
			printf("\nNot a mine, checking stuff\n");
	//		cellminecount(i, j);
			int zeromines = cellminecount(i,j);
				if(zeromines == 1) {
					printf("zeromines = %d\n", zeromines);
					printf("gonna check again here\n");
					continuechecking(i,j);
						for(checkingerror=0; checkingerror<(bs-3); checkingerror++) {
							for(row = 0; row<MAXSIZE; row++) {
        						for(col = 0; col<MAXSIZE; col++) {
									if(displayboard[row][col] == 48)
										continuechecking(row,col);
								}	
							}	
    					}
    			}		
		}
		else
			printf("hmm, what happened?\n^,^");

	}


	/*************************************** Win game, play again ***************************************/

	if(correctflags == maxmines) {
		printf("You Win!\nCongratulations, you survived this mine field and flagged all %d mines! Play again? Press 1, any other key to exit.\n", correctflags);
		scanf("%d", &playagain);
		revealboard();	
	}	

	printboard();

} while(cellstatus != 1); 

} while(playagain == 1);

//	if(GAMEOVER){}


	int n=0;


	/////////////////////////////////
	do {
		printf("Press 1 to continue ( any other number to exit ) ");
		scanf("%d", &n);
		clrscn();
		//printboard(bs);
	} while(n == 1);
	
	printf("\n");

	if( n!= 1)
		printf("Thank you for playine minesweeper, Cat edition!\n");

	return 0;
}

//////////////////////////////////////////////////////////
//														//
//						Functions						//
//														//
//////////////////////////////////////////////////////////



/****************************************** ASSIGNCLEANBOARD ********************************************/	

// assigncleanboard: this function is used to initiate the display board 
// by assigning question marks to all cells in the display board at a new game. 

void assigncleanboard(int size) {

//	char displayboard[MAXSIZE][MAXSIZE] = {};
	int row=0, col=0;



		// Assign initial characters to display board
		 for(row=0; row<=size+3; row++) {
			for(col=0; col<=size+3; col++) {
				if(col == 0 || col == 1 || col == size+2 || col == size+3 && row == 0 || row == 1 || row == size+2 || row == size+3) 
					// First two row and column cells are assigned blank spaces.
					displayboard[row][col] = 32;

				else if(row == 0)
					// First row cells are assigned incrememting capital letters for column coordinates
					displayboard[row][col] = col+63;
				else if(col == size+3)
					// Last column cells are assigned incrementing lowercase small letters for row coordinates
					displayboard[row][size+3] = 95+row;		
				else
					// Cells not including the first two rows and columns are assigned question marks
					displayboard[row][col] = 63;
				
				
				//printf(" %c ", displayboard[row][col]);
			}		
			//printf("\n");
		}			
}



/******************************************* CELLMINECOUNT **********************************************/	

// cellminecount: this function checks if the surrounding cells of a selected cell have any mines. If any
// of them contain mines, a counter is incremented. If the counter is equal to zero, the function returns 
// 1, which is then an indication to continue checking.

int cellminecount(int i, int j) {

	int numofmines=0;

	// Check adjacent cells on same row
	if (mineboard[i][j+1] == 42)
		numofmines++;
	if (mineboard[i][j-1] ==42)
		numofmines++;

	// Check adjacent cells on same column	
	if (mineboard[i+1][j] == 42)
		numofmines++;
	if (mineboard[i-1][j] == 42)
		numofmines++;

	// Check corner cells
	if (mineboard[i+1][j+1] == 42) //NE corner
		numofmines++;	
	if (mineboard[i-1][j-1] == 42) //SW corner
		numofmines++;	
	if (mineboard[i-1][j+1] == 42) //SE corner
		numofmines++;	
	if (mineboard[i+1][j-1] == 42) //NW corner
		numofmines++;	

	if(displayboard[i][j] == 63 || displayboard[i][j] == 70)
		displayboard[i][j]  = numofmines+48;


/*	if(numofmines != 0) {
		printf("Number of mines = %d\n", numofmines);
		printf("^,^\n");
		return 0;
	}
*/
	if(numofmines == 0) {
//		printf("No mines in this cell\n");
		return 1; // This return value of 1 means there are no mines surrounding the cell
	}

}



/**************************************** SECONDARYCELLMINECOUNT ****************************************/	

// cellminecount: this function checks if the surrounding cells of a selected cell have any mines. If any
// of them contain mines, a counter is incremented. If the counter is equal to zero, the function returns 
// 1, which is then an indication to continue checking.

int secondarycellminecount(int i, int j) {

	int numofmines=0;

	// Check adjacent cells on same row
	if (mineboard[i][j+1] == 42)
		numofmines++;
	if (mineboard[i][j-1] ==42)
		numofmines++;

	// Check adjacent cells on same column	
	if (mineboard[i+1][j] == 42)
		numofmines++;
	if (mineboard[i-1][j] == 42)
		numofmines++;

	// Check corner cells
	if (mineboard[i+1][j+1] == 42) //NE corner
		numofmines++;	
	if (mineboard[i-1][j-1] == 42) //SW corner
		numofmines++;	
	if (mineboard[i-1][j+1] == 42) //SE corner
		numofmines++;	
	if (mineboard[i+1][j-1] == 42) //NW corner
		numofmines++;	

	if(displayboard[i][j] == 63 && displayboard[i][j] != 70)
		displayboard[i][j]  = numofmines+48;


/*	if(numofmines != 0) {
		printf("Number of mines = %d\n", numofmines);
		printf("^,^\n");
		return 0;
	}
*/
	if(numofmines == 0) {
//		printf("No mines in this cell\n");
		return 1; // This return value of 1 means there are no mines surrounding the cell
	}

}







/***************************************** CHECKCELLSTATUS **********************************************/	

// checkcellstatus: this function checks if the selected cell has a value of a mine, a cell that has 
// already been opened, or a non-mine cell by referencing the corresponding cell on the mineboard.

int checkcellstatus(int i, int j) {

	if(mineboard[i][j] == 42) // Its a mine
		return 1;
	else if (displayboard[i][j] > 47 && displayboard[i][j] < 58) // Its already been checked
		return 2;
	else if (mineboard[i][j] == 45) // Its not a mine
		return 0;
	else
		printf("hmm, what happened?\n^,^");	
}	




	
/***************************************** CONTINUECHECKING *********************************************/	

void continuechecking(int i, int j) {

	// Same row
	int E = secondarycellminecount(i,j+1); //E
		if(E == 1)
			printf("E cell has no mines, exit status %d\n", E);

	int W = secondarycellminecount(i,j-1); //W
		if(W == 1)
			printf("W cell has no mines, exit status %d\n", W);


	// Same column
	int S = secondarycellminecount(i+1,j); //S
		if(S == 1)
			printf("S cell has no mines, exit status %d\n", S);

	int N = secondarycellminecount(i-1,j); //N
		if(N == 1)
			printf("N cell has no mines, exit status %d\n", N);


	// Corners
	int SE = secondarycellminecount(i+1,j+1); //SE
		if(SE == 1)
			printf("SE cell has no mines, exit status %d\n", SE);

	int NW = secondarycellminecount(i-1,j-1); //NW
		if(NW == 1)
			printf("NW cell has no mines, exit status %d\n", NW);

	int NE = secondarycellminecount(i-1,j+1); //NE
		if(NE == 1)
			printf("NE cell has no mines, exit status %d\n", NE);

	int SW = secondarycellminecount(i+1,j-1); //SW
		if(SW == 1)
			printf("SW cell has no mines, exit status %d\n", SW);



}



/********************************************* PRINTBOARD ***********************************************/	

// printboard: this function prints the current board.

void printboard() 
{

    int row=0, col=0,n=0;

    printf("\n\n\n\n\n\n\n\n\n\n");

    for(row = 0; row<MAXSIZE; row++) {
        for(col = 0; col<MAXSIZE; col++)
            printf(" %c ", displayboard[row][col]);
        printf("\n"); 
    }

    printf("\n\n\n\n\n\n\n\n\n");
}




	
/********************************************** MINEFIELDON *********************************************/	

// minefieldon: this function puts mines on the field by generating random numbers in the range 2 - size+1. The random numers 
// are used as gameboard coordinates for locations of mines. Amount of mines is size + a constant which increses
// with size as follows: if size == 6 then 	mines = 4,	~11% mines 	(easy)
//									7				6, 	~12%
//									8				8, 	~13%
//									9				9, 	~14%
//									10				15,	 15%
//									11				19,	~16%
//									12				24,	~17%		(hard)
//	
// So there will be 2 random numbers for every mine location, one for row and one for column. 	

void minefieldon(int size) {

	int row=0, col=0, mines=0;

	// Set entire board equal to some value.

    for(row = 0; row<MAXSIZE; row++) {
        for(col = 0; col<MAXSIZE; col++) {
        	mineboard[row][col] = 45;
//            printf(" %c ", mineboard[row][col]);
        }
//        printf("\n"); 
    }

	srand(time(NULL));



	if(size == 6) {
		maxmines=3;


		for(mines=0; mines<maxmines;) {
			row = rand() %6 +2;
			col = rand() %6 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}		

	else if(size == 7) {
		maxmines=5;

		for(mines=0; mines<maxmines;) {
			row = rand() %7 +2;
			col = rand() %7 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}

	else if(size == 8) {
		maxmines=7;

		for(mines=0; mines<maxmines;) {
			row = rand() %8 +2;
			col = rand() %8 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}

	else if(size == 9) {
		maxmines=9;

		for(mines=0; mines<maxmines;) {
			row = rand() %9 +2;
			col = rand() %9 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}

	else if(size == 10) {
		maxmines=13;

		for(mines=0; mines<maxmines;) {
			row = rand() %10 +2;
			col = rand() %10 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}
	else if(size == 11) {
		maxmines=17;

		for(mines=0; mines<maxmines;) {
			row = rand() %11 +2;
			col = rand() %11 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}
	else if(size == 12) {
		maxmines=22;

		for(mines=0; mines<maxmines;) {
			row = rand() %12 +2;
			col = rand() %12 +2;
//			printf("row = %d\tcol =%d\n", row, col);
				if(mineboard[row][col] != 42)
					mines++;
			mineboard[row][col] = 42;
//			displayboard[row][col] = mineboard[row][col];
//			printboard();
		} 
		printf("This board has %d mines.", maxmines);
	}
	else
		printf("hmm, what happened?\n^,^");	

	
/*	printf("\n"); 
	for(row = 0; row<MAXSIZE; row++) {
        for(col = 0; col<MAXSIZE; col++) {
            printf(" %c ", mineboard[row][col]);
        }
    	printf("\n"); 
    }*/

}





/*********************************************** REVEALBOARD ********************************************/

void revealboard() {

int row=0, col=0;

    for(row = 0; row<MAXSIZE; row++) {
        for(col = 0; col<MAXSIZE; col++) { 
			if(mineboard[row][col] == 42)
				displayboard[row][col] = mineboard[row][col];
		}
	}	
}




/****************************** ^,^   ^,^	 ^,^	 ^,^	 ^,^	 ^,^ ********************************/
