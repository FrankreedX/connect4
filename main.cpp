#include <iostream>
#include <cstring>
#include <cmath>
#include "helper_functions.h"
#include "connect4Utils.h"

int heuristic(int** board, int** pointTable, int col, int row);

int* alphabeta(int** board, int** pointTable, int* order, int col, int row, int depth, int alpha, int beta, bool red);

int* generateOrder(int col);

int main(int argc, char *argv[]) {
	//error if there are not enough arguments
	if(argc < 3){
		std::cout << "Expecting more arguments" << std::endl;
		return 0;
	}
	//filtering each inputs
	int twoPlayers = checkArgumentNumber(argv[1], 1, 2, "First argument must be either 1 or 2") - 1;

	int col = checkArgumentNumber(argv[2], 1, 20,"Second argument must be between 1 and 20");

	int row = checkArgumentNumber(argv[3], 1, 20, "Third argument must be between 1 and 20");

	//creating constant lists
	int** pointTable = createPointTable(col, row);
	int* order = generateOrder(col);

	//game loop
	while(1){
		int** board = createBoard(col, row); //gameboard creation
		printBoard(board, col, row);

		int win = -1;

		int turnCount = 0;

		bool red = true;

		//single round loop
		for(int count = 0; win == -1; count++){

			std::cout << (red ? "Red" : "Yellow") << "'s turn\n";

			if(!red && !twoPlayers) std::cout << "Thinking...\n"; //print if computer is playing

			int moveCol = (red || twoPlayers) ? askForInt("Enter column to drop ", 0, col-1) : alphabeta(board, pointTable, order, col, row, 9, -UINT16_MAX, UINT16_MAX, red)[0]; //moveCol becomes inputted number or computer generated

			if(!red && !twoPlayers) std::cout << "Computer moved at column: " << moveCol << std::endl;

			int moveRow = addPiece(board, red + 1, moveCol, row, turnCount); //play move
			if(moveRow == -1) {
				std::cout << "Invalid column\n";
				continue;
			}
			printBoard(board, col, row);

			win = checkWin(board, col, row, moveCol, moveRow, turnCount); red = !red; //check win condition
		}

		std::cout << (win==0 ? "draw" : (win==1 ? "yellow win" : "red win")) << std::endl;
		if(!askForInt("Play again? (1 for yes, 0 for no) ", 0, 1)) {
			delete2D(board, row);
			delete2D(pointTable, row);
			delete [] order;
			return 0;
		}
	}
}


/*******************************************************************
** Function: heuristic
** Description: given the gameboard and pointTable, grade the current position
** Input: gameboard, pointTable, gameboard dimensions
** Output: score of the board
*******************************************************************/


int heuristic(int** board, int** pointTable, int col, int row){
	int sum = 0;
	for(int j = 0; j < col; j++)
		for(int i = 0; i < row; i++)
			if(board[i][j] > 0)
				switch(checkWin(board, col, row, j, i, 0)){
					case 1: return UINT16_MAX;
					case 2: return -UINT16_MAX;
					case 0: return 0;
					default: sum += (board[i][j]==1 ? 1 : -1) * pointTable[i][j];//if the piece is yellow, add the corresponding score from the pointTable, red piece will subtract score from pointTable.
				}
	return sum;
}

/*******************************************************************
** Function: generateOrder
** Description: given an int, generate a list of int sorted from middle out (7 -> 3,4,2,5,1,6,0)
** Input: int
** Output: list pointer on heap
*******************************************************************/

int* generateOrder(int col){
	int* arr = new int[col];

	int index = 0;
	int counter = 1;

	if(col % 2 == 1) arr[index++] = col/2;
	else {
		arr[index++] = col/2;
		arr[index++] = col/2-1;
	}

	while(index < col){
		arr[index++] = double(col/2.0) + counter;
		arr[index++] = double(col/2.0) - .5 - counter;
		counter++;
	}
	return arr;
}

/*******************************************************************
** Function: alphabeta
** Description: given a lot of information, perform a minimax/alpha beta pruning search of all relatively good moves with a certain depth and return the best column to move and the score associated with it. May take a couple of seconds to run.
** Input: gameboard, the pointTable, gameboard's dimensions, depth of search, alpha and beta (limits board evaluation and speeds up searching process) and color
** Output: list pointer on heap containing the column(index 0) and score (index 1)
*******************************************************************/

int* alphabeta(int** board, int** pointTable, int* order, int col, int row, int depth, int alpha, int beta, bool red){

	int eval = heuristic(board, pointTable, col, row);

	if(depth == 0 || eval == 0 || abs(eval) > 500) {//if the current gameboard is game ending, return the evaluation
		int* returnArray = new int[2];
		returnArray[0] = -1;
		returnArray[1] = eval;
		return returnArray;
	}

	if(!red){//computer is always yellow so yellow is always the max player
		int value = -UINT16_MAX;
		int bestCol = -1;
		for(int j = 0; j < col; j++){
			int i = order[j]; //consider from the middle out, middle columns usually gets chosen so it cuts down on consider time
			int can = canPlay(board, i, row);
			if(can != -1){
				//simulate a move
				int** node = createBoard(col, row);
				copyBoard(board, node, col, row);
				int a;
				addPiece(node, red + 1, i, row, a);

				int* b = alphabeta(node, pointTable, order, col, row, depth - 1, alpha, beta, !red);

				//max player always choose the higheset scoring move
				if(b[1] > value){
					value = b[1];
					bestCol = i;
				}

				alpha = std::max(value, alpha);

				delete2D(node, row);
				delete [] b;

				if(alpha >= beta) break;

			}
		}

		int* returnArray = new int[2];
		returnArray[0] = bestCol;
		returnArray[1] = value;
		return returnArray;

	} else {//similar to above
		int value = UINT16_MAX;
		int bestCol = -1;

		for(int j = 0; j < col; j++){
			int i = order[j];
			int can = canPlay(board, i, row);
			if(can != -1){

				int** node = createBoard(col, row);
				copyBoard(board, node, col, row);
				int a;
				addPiece(node, red + 1, i, row, a);

				int* b = alphabeta(node, pointTable, order, col, row, depth - 1, alpha, beta, !red);

				//min player always choose the lowest score
				if(b[1] < value){
					value = b[1];
					bestCol = i;
				}

				beta = std::min(value, beta);
				
				delete2D(node, row);
				delete [] b;

				if(beta <= alpha) break;

			}
		}
		
		int* returnArray = new int[2];
		returnArray[0] = bestCol;
		returnArray[1] = value;
		return returnArray;
	}
}