class Game{
        char** board;
        char player1, player2;
        int bWidth, bHeight, winCount;

    public:
        Game(int count = 3);
        Game(char p1, char p2, int count = 3);
        Game(int widht, int height, int count = 3);
        Game(char p1, char p2, int width, int height, int count = 3);

        ~Game();

        void showBoard(void);
        
        //Ads player move to board if it is posible
        //player = 1 or 2 (whitch player is making move)
        //positionX, positonY = position where should be add move on board
        //return 0 if move was add, 1 otherwise
        int makeMove(int player, int positionX, int positionY);
   
        //Checks if player1 win
        //return 1 if player1 win, 2 for draw, 0 otherwise
        int checkWinPlayer1(void);

        //Checks if player2 win
        //return 1 if player2 win, 2 for draw, 0 otherwise
        int checkWinPlayer2(void);

        //run game for 1 PC
        //return 1 if player1 win, 2 if player2 win, 3 for draw, 0 otherwise
        int play1PC(void);

        //run game for 2 PCs as server
        //return 1 if player1 win, 2 if player2 win, 3 for draw, 0 otherwise
        int hostGame(char* port);  
};
