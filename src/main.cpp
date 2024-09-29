#include <iostream>
#include <string>

#include "log.h"
#include "server.h"

int main(int, char**){
    Logger log("/Users/max/code/projects/lets-lsp/log.txt");
    Server server(std::cin, std::cout, log);

    server.start();

    return 0;
}
