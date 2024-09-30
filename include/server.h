#ifndef LETS_LSP_SERVER_H
#define LETS_LSP_SERVER_H

#include <iostream>

#include "log.h"
#include "rpc.h"

class Server {
public:
  Server(std::istream &in, std::ostream &out, Logger &logger)
      : conn(in, out, logger), logger(logger) {}

  void start();
  void stop();

  void handler(const lsp::Message &message);
  void handle_request(const lsp::RequestMessage &message);
  void handle_notification(const lsp::NotificationMessage &message);

private:
  Connection conn;
  Logger &logger;
};

#endif
