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
  State state(log);
  Server server(std::cin, std::cout, log, state);

  try {
    server.start();
  } catch (...) {
    log.log("lets_ls failed");
    return 1;
  }

  return 0;
}
