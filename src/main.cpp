#include <iostream>
#include <string>

#include <argparse/argparse.hpp>

#include "analysis.h"
#include "log.h"
#include "server.h"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("lets_ls");

  program.add_argument("-f", "--file").help("file to log");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  std::string log_file("/dev/null");
  if (auto file = program.present("-f")) {
    log_file = file.value();
  }

  FileLogger log(log_file);
  State state;
  Server server(std::cin, std::cout, log, state);

  server.start();

  return 0;
}
