#include "server.h"
#include "lsp.h"

void Server::start() {
  logger.log("lets-ls server started");
  while (true) {
    auto message = conn.read();
    if (message != nullptr) {
      handler(*message);
    }
  }
}

void Server::stop() {
  logger.log("lets-ls server stopped");
  // TODO: some signal handling ? ctrl+c ?
}

void Server::handler(const lsp::Message &message) {
  if (message.get_kind() == lsp::Message::Kind::Request) {
    if (auto request = static_cast<const lsp::RequestMessage *>(&message)) {
      handle_request(*request);
    }
  } else if (message.get_kind() == lsp::Message::Kind::Notification) {
    if (auto notification =
            static_cast<const lsp::NotificationMessage *>(&message)) {
      handle_notification(*notification);
    }
  }
}

void Server::handle_request(const lsp::RequestMessage &message) {
  if (message.method == "initialize") {
    auto params = message.data.template get<lsp::InitializeParams>();
    logger.log("Connected client: " + params.client_info.name + " " +
               params.client_info.version);
    lsp::InitializeResponse response(message.id);
    logger.log("Sending response ", response.to_json());
    conn.write(response);
  } else {
    logger.log("Unknown method ", message.method);
  }
}

void Server::handle_notification(const lsp::NotificationMessage &message) {
  if (message.method == "initialized") {
    logger.log("Client connection initialized");
  } else if (message.method == "textDocument/didOpen") {
    auto params = message.data.template get<lsp::DidOpenTextDocumentParams>();
    state.open_document(params.text_document_item.uri,
                        params.text_document_item.text);
    logger.log("Opened: " + params.text_document_item.uri);
  } else {
    logger.log("Unknown method ", message.method);
  }
}
