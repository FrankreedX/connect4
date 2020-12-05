#include <iostream>
#include <cstring>
#include <cmath>
#include "helper_functions.h"

/*******************************************************************
** Function: checkWin
** Description: given a gameboard and a coordinate, this function can check if the move at the coordinate will cause the game to end and determine what the ending is (red win, yellow win, draw or neutral). It can simulate a move at the given coordinate with both color.
** Input: gameboard, gameboard dimensions, move coordinate, hypothetical color to look for (0 if using color on the gameboard)
** Output: 1: victory for yellow, 2: victory for red, 0 draw, -1: neutral
*******************************************************************/

int checkWin(int** board, int col, int row, int moveCol, int moveRow, int count, int hypothetical){
	if(count >= col*row) return 0;

	int currentColor = hypothetical == 0 ? board[moveRow][moveCol] : hypothetical;

	int lineScore = 0;

	for(int i = moveCol+1; i <= clamp(moveCol+3, 0, col-1) && board[moveRow][i] == currentColor; i++) lineScore++;
	for(int i = moveCol-1; i >= clamp(moveCol-3, 0, col-1) && board[moveRow][i] == currentColor; i--) lineScore++;

	if(lineScore>=3) return currentColor;
	lineScore = 0;

	for(int i = moveRow+1; i <= clamp(moveRow+3, 0, row-1) && board[i][moveCol] == currentColor; i++) lineScore++;
	for(int i = moveRow-1; i >= clamp(moveRow-3, 0, row-1) && board[i][moveCol] == currentColor; i--) lineScore++;

	if(lineScore>=3) return currentColor;
	lineScore = 0;

	for(int i = moveCol+1; i <= clamp(moveCol+3, 0, col-1) && moveRow + i - moveCol <= clamp(moveRow+3,0,row-1) && board[moveRow + i - moveCol][i] == currentColor; i++) lineScore++;
	for(int i = moveCol-1; i >= clamp(moveCol-3, 0, col-1) && moveRow + i - moveCol >= clamp(moveRow-3,0,row-1) && board[moveRow + i - moveCol][i] == currentColor; i--) lineScore++;

	if(lineScore>=3) return currentColor;
	lineScore = 0;

	for(int i = moveCol+1; i <= clamp(moveCol+3, 0, col-1) && moveRow - i + moveCol >= clamp(moveRow-3,0,row-1) && board[moveRow - i + moveCol][i] == currentColor; i++) lineScore++;
	for(int i = moveCol-1; i >= clamp(moveCol-3, 0, col-1) && moveRow - i + moveCol <= clamp(moveRow+3, 0, row-1) && board[moveRow - i + moveCol][i] == currentColor; i--) lineScore++;

	if(lineScore>=3) return currentColor;
	return -1;
}

/*******************************************************************
** Function: canPlay
** Description: given a gameboard and a column, check to see if that column can be played and which row on the column can be played
** Input: gameboard, column to check, gameboard's total rows
** Output: playable row, -1 otherwise
*******************************************************************/

int canPlay(int** board, int col, int row){
	for(int i = row-1; i >= 0; i--){
		if(board[i][col] == 0) return i;
	}
	return -1;
}

/*******************************************************************
** Function: addPiece
** Description: given a gameboard, a column and a piece, insert that piece into that column and update a count reference to count total gamerounds (for draws)
** Input: gameboard, piece to play (1 for yellow, 2 for red), column to play, gameboard's total rows, the count integer
** Output: the row the piece is placed on
*******************************************************************/

int addPiece(int** board, int piece, int col, int row, int &count){
	count++;
	int can = canPlay(board, col, row);
	if(can > -1) {board[can][col] = piece; return can;}
	else return -1;
}

/*******************************************************************
** Function: addPiece
** Description: given a dimension for a gameboard, create an empty board on the heap and return the address
** Input: gameboard dimensions
** Output: heap address for the board created
*******************************************************************/

int** createBoard(int col, int row){
	int** board = new int*[row];
	for(int i = 0; i < row; i++){
		board[i] = new int[col];
		for(int j = 0; j < col; j++){
			board[i][j] = 0;
		}
	}
	return board;
}

/*******************************************************************
** Function: copyBoard
** Description: given pointers to 2 gameboards, copy from the source board to destination board.
** Input: source address, destination address, gameboard dimensions
** Output: NA
*******************************************************************/

void copyBoard(int** source, int** dest, int col, int row){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			dest[i][j] = source[i][j];
		}
	}
}

/*******************************************************************
** Function: createPointTable
** Description: given gameboard's dimensions, create a point value table for the AI to evaluate a gamestate
** Input: gameboard dimensions
** Output: heap address for the board created
*******************************************************************/

int** createPointTable(int col, int row){
	int** pointTable = new int*[row];
	for(int i = 0; i < row; i++){
		pointTable[i] = new int[col];
		for(int j = 0; j < col; j++){
			pointTable[i][j] = 0;
		}
	}
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++)
			for(int k = 0; k < 4; k++){
				if(j < col - 3) pointTable[i][j+k]++;
				if(j < col - 3 && i < row - 3)pointTable[i+k][j+k]++;
				if(i < row - 3) pointTable[i+k][j]++;
				if(j > 2 && i < row - 3) pointTable[i+k][j-k]++;
			}

	return pointTable;
}

/*******************************************************************
** Function: printBoard
** Description: given a gameboard, beautifully print out its contents
** Input: gameboard, gameboard dimensions
** Output: NA
*******************************************************************/

void printBoard(int** board, int col, int row){
	bool doubleDigits = col > 9;
	std::cout << (doubleDigits ? "    " : "   ");
	for(int i = 0; i < col; i++){
		std::cout << std::to_string(i).append(doubleDigits + 2 - std::to_string(i).length(), ' ');
	}
	std::cout << std::endl;
	for (int i=0; i<row; i++){
			std::cout << std::to_string(i).append(doubleDigits + 1 - std::to_string(i).length(), ' ') << " ";
		for (int j=0; j<col; j++){
			switch(board[i][j]){
				case 1: std::cout << "\033[93;44m "; break;
				case 2: std::cout << "\033[31;44m "; break;
				default: std::cout << "\033[30;44m "; break;
			}
			std::cout << "●" << (doubleDigits ? " " : "") << "\033[0m";
		}
	if(!doubleDigits)std::cout << "\033[33;44m \033[0m";
	std::cout << std::endl;
	}
}