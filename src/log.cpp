#include <iostream>
#include <string>

#include "log.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Logger::Logger(const std::string &filename) : filename(filename) {
  logfile.open(filename, std::ios::binary | std::ios::trunc);
  if (!logfile.is_open()) {
    std::cerr << "Failed to open log file: " << filename << std::endl;
  }
}

void Logger::log(const std::string &msg) { logfile << msg << std::endl; }
void Logger::log(const std::string &msg, const json &data) { logfile << msg << " " << data.dump() << std::endl; }
