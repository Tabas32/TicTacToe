struct settings{
    char player1, player2;
    int width, height, win;
    char* port;
    char* ip;
};

//prints name of game, author and version
void printHead(void);

//prints list of usabale commands and rules of game
void printHelp(void);

//executes RUN command
void executeRun(void);

//executes HOST command
void executeHost(void);

//executes JOIN command
void executeJoin(void);

//executes SETTINGS command
void executeSettings(void);
