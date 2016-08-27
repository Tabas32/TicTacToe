#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <string>

#include "game.h"
#include "board.h"

Game::Game(int count){
    player1 = 'X';
    player2 = 'O';
    winCount = count;

    bWidth = 3;
    bHeight = 3;

    board = createBoard(bWidth, bHeight);
}

Game::Game(char p1, char p2, int count){
    player1 = p1;
    player2 = p2;
    winCount = count;

    bWidth = 3;
    bHeight = 3;

    board = createBoard(bWidth, bHeight);

}

Game::Game(int widht, int height, int count){
    player1 = 'X';
    player2 = 'O';
    winCount = count;

    bWidth = widht;
    bHeight = height;

    board = createBoard(bWidth, bHeight);

}

Game::Game(char p1, char p2, int width, int height, int count){
    player1 = p1;
    player2 = p2;
    winCount = count;

    bWidth = width;
    bHeight = height;

    board = createBoard(bWidth, bHeight);

}

Game::~Game(){
    destroyBoard(this->board, this->bHeight);
}

void Game::showBoard(void){
    displayBoard(board, bWidth, bHeight);
}

int Game::makeMove(int player, int positionX, int positionY){
    if((player != 1) && (player != 2))
        return 1;

    if((positionX > bWidth) || (positionX < 1))
        return 1;

    if((positionY > bHeight) || (positionY < 1))
        return 1;

    if (board[positionY-1][positionX-1] != ' '){
        return 1;
    }

    if(player == 1){
        board[positionY-1][positionX-1] = player1;
    }
    else{
        board[positionY-1][positionX-1] = player2;
    }

    return 0;
}

int Game::checkWinPlayer1(void){    
    if(checkVerticalAxis(board, bWidth, bHeight, player1, winCount) ||
       checkHorizontalAxis(board, bWidth, bHeight, player1, winCount) ||
       checkDiagonalULDR(board, bWidth, bHeight, player1, winCount) ||
       checkDiagonalDLUR(board, bWidth, bHeight, player1, winCount))
    {
        return 1;
    }

    if(checkForDraw(board, bWidth, bHeight))
            return 2;

    return 0;
}

int Game::checkWinPlayer2(void){    
    if(checkVerticalAxis(board, bWidth, bHeight, player2, winCount) ||
       checkHorizontalAxis(board, bWidth, bHeight, player2, winCount) ||
       checkDiagonalULDR(board, bWidth, bHeight, player2, winCount) ||
       checkDiagonalDLUR(board, bWidth, bHeight, player2, winCount))
    {
        return 1;
    }

    if(checkForDraw(board, bWidth, bHeight))
            return 2;

    return 0;
}

int Game::play1PC(void){
    int x = 0, y = 0, state;

    while(1){
        do{
            system("clear");
            this->showBoard();

            std::cout << ">>> Player1: ";
            std::cin >> x >> y;
            if(x == -1 && y == -1)
                return 0;
        }while(this->makeMove(1, x, y) != 0);

        state = this->checkWinPlayer1();
        if(state == 1)
            return state;
        if(state == 2)
            return state + 1;

        do{
            system("clear");
            this->showBoard();

            std::cout << ">>> Player2: ";
            std::cin >> x >> y;
            if(x == -1 && y == -1)
                return 0;
        }while(this->makeMove(2, x, y) != 0);

        state = this->checkWinPlayer2();
        if(state == 1 || state == 2)
            return state + 1;
    }
}

int Game::hostGame(char* port){
    int ret_val = 0;

    //create struct info abouth socket
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;

    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;

    status = getaddrinfo(NULL, port, &host_info, &host_info_list);
    if(status != 0)
        std::cout << "getaddrinfo error" << gai_strerror(status);


    //create socket descripter
    int socketfd;
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if(socketfd == -1)
        std::cout << "socket error" << std::endl;


    //binding
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                        sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if(status == -1)
        std::cout << "bind error" << std::endl;


    //listening for client
    std::cout << "Waiting for player to join...." << std::endl;
    status = listen(socketfd, 5);
    if(status == -1)
        std::cout << "listen error" << std::endl;

    int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
    if(new_sd == -1)
        std::cout << "listen error" << std::endl;
    else
        std::cout << "Connection accepted." << std::endl;

    //sending information to create game
    std::string par_msg = std::to_string(bWidth) + " " + std::to_string(bHeight) + " " + player1 + player2 + std::to_string(winCount);
    const char* msg = par_msg.c_str();
    int len;
    ssize_t bytes_sent;
    len = strlen(msg);
    bytes_sent = send(new_sd, msg, len, 0);

    //play game
    int x = 0, y = 0, state;

    while(1){
        do{
            system("clear");
            showBoard();

            std::cout << ">>> Player1: ";
            std::cin >> x >> y;
            if(x == -1 && y == -1){
                freeaddrinfo(host_info_list);
                return 0;
            }
        }while(makeMove(1, x, y) != 0);

        //parsing move to string
        std::string par_msg = std::to_string(x) + " " + std::to_string(y);

        //sending move
        const char* msg = par_msg.c_str();
        int len;
        ssize_t bytes_sent;
        len = strlen(msg);
        bytes_sent = send(new_sd, msg, len, 0);

        state = checkWinPlayer1();
        if(state == 1){
            ret_val = state;
            break;
        }
        if(state == 2){
            ret_val = state + 1;
            break;
        }

        system("clear");
        showBoard();

        std::cout << ">>> Player2 is making his move..." << std::endl;

        //receiving player2 move
        ssize_t bytes_received;
        char incomming_data_buffer[20];
        bytes_received = recv(new_sd, incomming_data_buffer, 20, 0);
        if(bytes_received == 0)
            std::cout << "oponent shut down" << std::endl;
        if(bytes_received == -1)
            std::cout << "receive error" << std::endl;
        incomming_data_buffer[bytes_received] = '\0';

        //parsing received message
        sscanf(incomming_data_buffer, "%d %d", &x, &y);

        makeMove(2, x, y);

        state = checkWinPlayer2();
        if(state == 1 || state == 2){
            ret_val = state + 1;
            break;
        }
    }

    freeaddrinfo(host_info_list);

    return ret_val;
}

