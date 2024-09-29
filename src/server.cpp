#include "server.h"

void Server::start() {
  logger.log("lets-lsp server started");
  while (true) {
    auto message = conn.read();
    if (message != nullptr) {
      handler(*message);
    }
    // TODO: handle message/send response
  }
}

void Server::stop() {
  logger.log("lets-lsp server stopped");
  // TODO: some signal handling ? ctrl+c ?
}

void Server::handler(const Message &message) {
  if (message.get_kind() == Message::Kind::Request) {
    if (auto request = static_cast<const RequestMessage *>(&message)) {
      handle_request(*request);
    }
  }
}

void Server::handle_request(const RequestMessage &message) {
  if (message.method == "initialize") {
    InitializeResponse response(message.id);
    logger.log("Sending response ", response.to_json());
    conn.write(response);
  } else {
    logger.log("Unknown method ", message.method);
  }
}
