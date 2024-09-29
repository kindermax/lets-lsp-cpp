#ifndef LETS_LSP_LOG_H
#define LETS_LSP_LOG_H

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Logger {
public:
  explicit Logger(const std::string &filename);
  void log(const std::string &msg);
  void log(const std::string &msg, const json &data);

private:
  std::string filename;
  std::ofstream logfile;
};

#endif
