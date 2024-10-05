#ifndef LETS_LSP_LOG_H
#define LETS_LSP_LOG_H

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Logger {
public:
  virtual void log(const std::string &msg) = 0;
  virtual void log(const std::string &msg, const json &data) = 0;
};

// FileLogger is a logger that writes to a file.
class FileLogger : public Logger {
public:
  explicit FileLogger(const std::string &filename);
  void log(const std::string &msg);
  void log(const std::string &msg, const json &data);

private:
  std::string filename;
  std::ofstream logfile;
};

// NullLogger is a logger that does nothing.
class NullLogger : public Logger {
public:
  void log(const std::string &msg);
  void log(const std::string &msg, const json &data);
};

#endif
