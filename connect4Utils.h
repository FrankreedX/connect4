int canPlay(int** board, int col, int row);

void copyBoard(int** source, int** dest, int col, int row);

int** createPointTable(int col, int row);

void printBoard(int** board, int col, int row);

int** createBoard(int col, int row);

int addPiece(int** board, int piece, int col, int row, int& count);

int checkWin(int** board, int col, int row, int moveCol, int moveRow, int count, int hypothetical = 0);

void delete2D(int** board, int row);