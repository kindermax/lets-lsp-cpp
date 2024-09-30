#ifndef LETS_LSP_SERVER_H
#define LETS_LSP_SERVER_H

#include <iostream>

#include "rpc.h"
#include "log.h"

// TODO: use namespace ?

class Server {
public:
    Server(std::istream& in, std::ostream& out, Logger& logger)
    : conn(in, out, logger),
      logger(logger) {}

    void start();
    void stop();

    void handler(const Message &message);
    void handle_request(const RequestMessage &message);
    void handle_notification(const NotificationMessage &message);
private:
    Connection conn;
    Logger& logger;
};

#endif