#ifndef LETS_LSP_RPC_H
#define LETS_LSP_RPC_H

#include <iostream>

#include "log.h"
#include "lsp.h"

class Connection {
public:
  Connection(std::istream &in, std::ostream &out, Logger &logger)
      : in(&in), out(&out), logger(logger) {}

  // If the message is not valid (can not be parsed) return nullptr
  std::unique_ptr<lsp::Message> read();
  std::string read_content(const lsp::MessageHeader &header);
  lsp::MessageHeader read_header();
  void write(const lsp::Message &msg);

private:
  std::istream *in;
  std::ostream *out;
  Logger &logger;
};

#endif
