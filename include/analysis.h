#ifndef LETS_LSP_ANALYSIS_H
#define LETS_LSP_ANALYSIS_H

#include <map>

class State {
private:
  std::map<std::string, std::string> documents;

public:
  void open_document(const std::string uri, const std::string content);
};

#endif
