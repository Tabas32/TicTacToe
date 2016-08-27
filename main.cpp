#include <iostream>
#include <string>

#include "board.h"
#include "game.h"
#include "menu.h"

int main(){
    std::string input;
    printHead();

    while(input.compare("EXIT") != 0){
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if(input.compare("HELP") == 0)
            printHelp();
        if(input.compare("RUN") == 0)
            executeRun();
        if(input.compare("HOST") == 0)
            executeHost();
        if(input.compare("JOIN") == 0)
            executeJoin();
        if(input.compare("SETTINGS") == 0)
            executeSettings();
        if(input.compare("CLEAR") == 0)
            system("clear");
    }
}
