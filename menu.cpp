#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>

#include "game.h"
#include "menu.h"

#define VERSION "0.1.0"

void setSettingsFromFile(const char* fileName, struct settings* setgs);
void printSettings(struct settings* set);
void printSetHelp(void);
void saveSettings(const char* fileName, struct settings* set);

void printHead(void){
    system("clear");
    for(int i = 0; i < 31; i++)
        std::cout << "=";
    std::cout << std::endl << "=";
    for(int i = 0; i < 29; i++)
        std::cout << " ";
    std::cout << "=" << std::endl;
    std::cout << "=         Tic Tac Toe         =" << std::endl << "=";
    for(int i = 0; i < 29; i++)
        std::cout << " ";
    std::cout << "=" << std::endl << "=";
    for(int i = 0; i < 29; i++)
        std::cout << "_";
    std::cout << "=\n=";
    for(int i = 0; i < 29; i++)
        std::cout << " ";
    std::cout << "=" << std::endl;
    std::cout << "=   Created by Marian Sabat   =\n";
    std::cout << "=";
    for(int i = 0; i < 29; i++)
        std::cout << " ";
    std::cout << "=" << std::endl;
    std::cout << "=       Version:  " << VERSION << "       =\n=";
    for(int i = 0; i < 29; i++)
        std::cout << " ";
    std::cout << "=\n";
    for(int i = 0; i < 31; i++)
        std::cout << "=";
    std::cout << std::endl;
    std::cout << "\nFor help type HELP\n";
}

void printHelp(void){
    std::cout << "Comands:\n";
    std::cout << "HELP: Displays help page.\n";
    std::cout << "RUN: Runs game for 2 players on one computer.\n";
    std::cout << "HOST: Runs game for 2 players as a host. First you will be asked to enter port. Use ports higher than 1024.\n";
    std::cout << "JOIN: You will be asked to enter IP addres and port of hosting game and then you will be join to this game.\n";
    std::cout << "EXIT: End porgram.\n";
    std::cout << "CLEAR: Clears page.\n";
    std::cout << "SETTINGS: Show current settings and gives you oportunity to change them.\n\n\n";

    std::cout << "Game rules:\n";
    std::cout << "Players take turns in placing their mark on grid. The player who succeeds in placing set number of their marks in a horizontal, verical, or diagonal row wins the game.\n";
    std::cout << "In every turn player enter x and y coordinates where he wants put his mark.\n";
    std::cout << "Grid is count on x axis from left to right and on y axis from up to down.\n";
    std::cout << "[1,1] [2,1] [3,1]\n";
    std::cout << "[1,2] [2,2] [3,2]\n";
    std::cout << "[1,3] [2,3] [3,3]\n";
    std::cout << "For terminating game input coordinates -1 -1\n\n";
}

void executeRun(void){
    struct settings set;
    setSettingsFromFile("settings.txt", &set);

    int status;

    //create and play game
    Game game{set.player1, set.player2, set.width, set.height, set.win};
    status = game.play1PC();

    //actions after closing game
    if(status != 0){
        system("clear");
        game.showBoard();

        if(status < 3)
            std::cout << "Player" << status << " wins\n";
        else
            std::cout << "Nobody wins\n";
    }
}

void executeHost(void){
    int status;
    struct settings set;
    setSettingsFromFile("settings.txt", &set);

    Game game{set.player1, set.player2, set.width, set.height, set.win};

    status = game.hostGame(set.port);

    //actions after closing game
    if(status != 0){
        system("clear");
        game.showBoard();

        if(status < 3)
            std::cout << "Player" << status << " wins\n";
        else
            std::cout << "Nobody wins\n";
    }
}

void executeJoin(void){
    struct settings set;
    setSettingsFromFile("settings.txt", &set);

    int ret_val = -1;

    int status;
    struct addrinfo host_info;
    struct addrinfo* host_info_list;

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(set.ip, set.port, &host_info, &host_info_list);

    int socketfd;
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if(socketfd == -1)
        std::cout << "socket error" << std::endl;

    status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if(status == -1)
        std::cout << "connect error" << std::endl;

    //receiving instruction for creating game
    ssize_t bytes_received;
    char incomming_data_buffer[20];
    bytes_received = recv(socketfd, incomming_data_buffer, 20, 0);
    if(bytes_received == 0)
        std::cout << "Host shut down." << std::endl;
    if(bytes_received == -1)
        std::cout << "receive error" << std::endl;
    incomming_data_buffer[bytes_received] = '\0';
            

    //parsing received message
    sscanf(incomming_data_buffer, "%d %d %c %c %d", &set.width, &set.height,
            &set.player1, &set.player2, &set.win);

    //creating game
    Game game{set.player1, set.player2, set.width, set.height, set.win};


    //play game
    int x = 0, y = 0, state;

    while(ret_val != 0){
        system("clear");
        game.showBoard();

        std::cout << ">>> Player1 is making his move... " << std::endl;

        //receiving player1 move
        ssize_t bytes_received;
        char incomming_data_buffer[20];
        bytes_received = recv(socketfd, incomming_data_buffer, 20, 0);
        if(bytes_received == 0)
            std::cout << "Host shut down." << std::endl;
        if(bytes_received == -1)
            std::cout << "receive error" << std::endl;
        incomming_data_buffer[bytes_received] = '\0';
            

        //parsing received message
        sscanf(incomming_data_buffer, "%d %d", &x, &y);

        game.makeMove(1, x, y);

        state = game.checkWinPlayer1();
        if(state == 1){
            ret_val = state;
            break;
        }
        if(state == 2){
            ret_val = state + 1;
            break;
        }

        do{
            system("clear");
            game.showBoard();

            std::cout << ">>> Player2: ";
            std::cin >> x >> y;
            if(x == -1 && y == -1){
                freeaddrinfo(host_info_list);
                ret_val = 0;
                break;
            }
        }while(game.makeMove(2, x, y) != 0);

        //parsing move to string
        std::string par_msg = std::to_string(x) + " " + std::to_string(y);

        //sending move
        const char* msg = par_msg.c_str();
        int len;
        ssize_t bytes_sent;
        len = strlen(msg);
        bytes_sent = send(socketfd, msg, len, 0);

        state = game.checkWinPlayer2();
        if(state == 1 || state == 2){
            ret_val = state + 1;
            break;
        }
    }

    freeaddrinfo(host_info_list);
    
    //actions after closing game
    if(ret_val != 0){
        system("clear");
        game.showBoard();

        if(status < 3)
            std::cout << "Player" << status << " wins\n";
        else
            std::cout << "Nobody wins\n";
    }
}

void executeSettings(void){
    struct settings set;
    setSettingsFromFile("settings.txt", &set);

    std::string input;

    while(input.compare("save") != 0){
        std::cout << ">>> ";
        std::getline(std::cin, input);

        char* command = (char*)calloc(20, sizeof(char));
        char* value = (char*)calloc(20, sizeof(char));

        if(input.compare("sett") == 0)
            printSettings(&set);
        else if(input.compare("help") == 0)
            printSetHelp();
        else if(input.compare("clear") == 0)
            system("clear");
        else if(input.compare("back") == 0)
            return;
        else{
            sscanf(input.c_str(), "%s %s", command, value);

            if(strcmp(command, "width") == 0){
                set.width = std::stoi(std::string(value), nullptr);
            }
            if(strcmp(command, "height") == 0){
                set.height = std::stoi(std::string(value), nullptr);
            }
            if(strcmp(command, "player1") == 0){
                set.player1 = value[0];
            }
            if(strcmp(command, "palyer2") == 0){
                set.player2 = value[0];
            }
            if(strcmp(command, "win") == 0){
                set.win = std::stoi(std::string(value), nullptr);
            }
            if(strcmp(command, "ip") == 0){
                strcpy(set.ip, value);
            }
            if(strcmp(command, "port") == 0){
                strcpy(set.port, value);
            }
        }
    }

    saveSettings("settings.txt", &set);
}

void setSettingsFromFile(const char* fileName, struct settings* setgs){
    
    std::string settings;
    std::string strPort;
    std::string strIp;
    std::ifstream setfile(fileName);
    if(setfile.is_open()){
        getline(setfile, settings);
        getline(setfile, strPort);
        getline(setfile, strIp);
        setfile.close();
    }
    else
        std::cout << "Unable to open settings file" << std::endl;

    sscanf(settings.c_str(), "%d %d %c %c %d", &setgs->width, &setgs->height,
            &setgs->player1, &setgs->player2, &setgs->win);

    setgs->port = (char*)calloc(strPort.length(), sizeof(char));
    setgs->ip = (char*)calloc(strIp.length(), sizeof(char));

    strcpy(setgs->port, strPort.c_str());
    strcpy(setgs->ip, strIp.c_str());
}

void printSettings(struct settings* set){
    std::cout << "GAME BOARD:\n";
    std::cout << "Width: " << set->width << std::endl;
    std::cout << "Height: " << set->height << std::endl << std::endl;
    std::cout << "PLAYERS:\n";
    std::cout << "Player1 character: " << set->player1 << std::endl;
    std::cout << "Player2 character: " << set->player2 << std::endl << std::endl;
    std::cout << "WIN CONDITIONS:\n";
    std::cout << "Win strike: " << set->win << std::endl << std::endl;
    std::cout << "SERVER:\n";
    std::cout << "Server IP: " << set->ip << std::endl;
    std::cout << "Server port: " << set->port << std::endl << std::endl;
}

void printSetHelp(void){
    std::cout << "For changing value type: \'NAME VALUE\'\nExample:\n";
    std::cout << "width 5\nheight 4\nplayer1 H\nplayer2 @\nwin 10\nip 192.168.1.13\nport 5434\n";
    std::cout << "For saving and returning to menu type: save\n";
    std::cout << "For returning to menu type: back\n";
    std::cout << "For printing current settings type: sett\n";
    std::cout << "For printing help page type: help\n";
    std::cout << "For clearing page type: clear\n\n";
}

void saveSettings(const char* fileName, struct settings* set){
    std::ofstream setFile;
    setFile.open(fileName);

    setFile << set->width << " " << set->height << " " << set->player1 << " " << set->player2 << " " << set->win << std::endl;
    setFile << set->port << std::endl;
    setFile << set->ip;

    setFile.close();
}
