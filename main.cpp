#include <iostream>
#include <cstring>
#include <cmath>
#include "helper_functions.h"
#include "connect4Utils.h"

int heuristic(int** board, int** pointTable, int col, int row);

int* alphabeta(int** board, int** pointTable, int col, int row, int depth, int alpha, int beta, bool red);

int* generateOrder(int col);

int main(int argc, char *argv[]) {

	if(argc < 3){
		std::cout << "Expecting more arguments" << std::endl;
		return 0;
	}

	int twoPlayers = checkArgumentNumber(argv[1], 1, 2, "First argument must be either 1 or 2") - 1;

	int col = checkArgumentNumber(argv[2], 1, 20,"Second argument must be between 1 and 20");

	int row = checkArgumentNumber(argv[3], 1, 20, "Third argument must be between 1 and 20");

	int** pointTable = createPointTable(col, row);

	while(1){
		int** board = createBoard(col, row);
		printBoard(board, col, row);

		int win = -1;

		int turnCount = 0;

		bool red = true;

		for(int count = 0; win == -1; count++){

			std::cout << (red ? "Red" : "Yellow") << "'s turn\n";

			int moveCol = (red || twoPlayers) ? askForInt("Enter column to drop ", 0, col-1) : alphabeta(board, pointTable, col, row, 9, -UINT16_MAX, UINT16_MAX, red)[0];

			if(!red) std::cout << "Computer moved at column: " << moveCol << std::endl;

			int moveRow = addPiece(board, red + 1, moveCol, row, turnCount);
			if(moveRow == -1) {
				std::cout << "Invalid column\n";
				continue;
			}
			printBoard(board, col, row);

			win = checkWin(board, col, row, moveCol, moveRow, turnCount); red = !red;
		}

		std::cout << (win==0 ? "draw" : (win==1 ? "yellow win" : "red win")) << std::endl;
		if(!askForInt("Play again? (1 for yes, 0 for no) ", 0, 1)) return 0;
	}

}

int heuristic(int** board, int** pointTable, int col, int row){
	int sum = 0;
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++)
			if(board[i][j] > 0)
				switch(checkWin(board, col, row, j, i, 0)){
					case 1: return UINT16_MAX;
					case 2: return -UINT16_MAX;
					case 0: return 0;
					default: sum += (board[i][j]==1 ? 1 : -1) * pointTable[i][j];
				}
	return sum;
}

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

int* alphabeta(int** board, int** pointTable, int col, int row, int depth, int alpha, int beta, bool red){

	int eval = heuristic(board, pointTable, col, row);
	if(depth == 0 || eval == 0 || abs(eval) > 500) {
		int* returnArray = new int[2];
		returnArray[0] = -1;
		returnArray[1] = eval;
		return returnArray;
	}

	int* order = generateOrder(col);

	if(!red){
		int value = -UINT16_MAX;
		int bestCol = -1;
		for(int j = 0; j < col; j++){
			int i = order[j];
			int can = canPlay(board, i, row);
			if(can != -1){

				int** node = createBoard(col, row);
				copyBoard(board, node, col, row);
				int a;
				addPiece(node, red + 1, i, row, a);

				int* b = alphabeta(node, pointTable, col, row, depth - 1, alpha, beta, !red);

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
	} else {
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

				int* b = alphabeta(node, pointTable, col, row, depth - 1, alpha, beta, !red);

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