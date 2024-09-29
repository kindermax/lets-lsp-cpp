#ifndef LETS_LSP_RPC_H
#define LETS_LSP_RPC_H

#include <iostream>

#include "lsp.h"
#include "log.h"

// connection

class Connection {
public:
  Connection(std::istream &in, std::ostream &out, Logger &logger)
      : in(&in), out(&out), logger(logger) {}

  // If the message is not valid (can not be parsed) return nullptr
  std::unique_ptr<Message> read();
  MessageHeader read_header();
  void write(const Message& msg);

private:
  std::istream *in;
  std::ostream *out;
  Logger &logger;
};

#endif
