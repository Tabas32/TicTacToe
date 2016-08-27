//Create board with width of width and height of height
//Board is full of ' '
char** createBoard(int width, int height);

//Destroy board
void destroyBoard(char** board, int height);

//Display curent state of board
void displayBoard(char** board, int width, int height);

//Check for "count" same "character" in the row at vertical axis
int checkVerticalAxis(char** board, int width, int height, char character, int count);

//Check for "count" same "character" in the row at horizontal axis
int checkHorizontalAxis(char** board, int width, int height, char character, int count);

//Check for "count" same "character" in the row in diagonal up left to down
//right
int checkDiagonalULDR(char** board, int width, int height, char character, int count);

//Check for "count" same "character" in the row in diagonal down left to up
//right
int checkDiagonalDLUR(char** board, int width, int height, char character, int count);

//Check if is any space for move
//Returns 1 if it find draw state, 0 otherwise
int checkForDraw(char** board, int width, int height);
