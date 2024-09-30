#include <iostream>
#include <string>

#include "analysis.h"
#include "log.h"
#include "server.h"

int main(int, char **) {
  Logger log("/Users/max/code/projects/lets-lsp/log.txt");
  State state;
  Server server(std::cin, std::cout, log, state);

  server.start();

  return 0;
}
