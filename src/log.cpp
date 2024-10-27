#include <iostream>
#include <string>

#include "log.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

FileLogger::FileLogger(const std::string &filename) : filename(filename) {
  logfile.open(filename, std::ios::binary | std::ios::trunc);
  if (!logfile.is_open()) {
    std::cerr << "Failed to open log file: " << filename << std::endl;
  }
}

void FileLogger::log(const std::string &msg) { logfile << msg << std::endl; }
void FileLogger::log(const std::string &msg, const json &data) {
  logfile << msg << " " << data.dump() << std::endl;
}

void NullLogger::log(const std::string & /*msg*/) {}
void NullLogger::log(const std::string & /*msg*/, const json & /*data*/) {}
