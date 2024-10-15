#include "server.h"
#include "lsp.h"

void Server::start() {
  logger.log("lets_ls server started");
  while (true) {
    auto message = conn.read();
    if (message != nullptr) {
      handler(*message);
    }
  }
}

void Server::stop() {
  logger.log("lets_ls server stopped");
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
  } else if (message.method == "textDocument/hover") {
    auto params = message.data.template get<lsp::HoverParams>();
    logger.log("Hover: " + params.text_document.uri);

    auto hover_result = state.hover(params.text_document.uri, params.position);
    lsp::HoverResponse response(message.id, hover_result);
    conn.write(response);
  } else if (message.method == "textDocument/definition") {
    auto params = message.data.template get<lsp::DefinitionParams>();
    logger.log("Definition: " + params.text_document.uri);

    auto definition_result =
        state.definition(params.text_document.uri, params.position);
    lsp::DefinitionResponse response(message.id, definition_result);
    logger.log("Sending response to textDocument/definition ",
               response.to_json());
    conn.write(response);
  } else {
    // TODO: handle shutdown method
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
  } else if (message.method == "textDocument/didChange") {
    auto params = message.data.template get<lsp::DidChangeTextDocumentParams>();
    logger.log("Changed: " + params.text_document.uri);
    for (auto const &change : params.content_changes) {
      state.update_document(params.text_document.uri, change.text);
    }

    // for (auto it = params.content_changes.begin(); it !=
    // params.content_changes.end(); ++it) {

    // }
    // std::vector<lsp::DidChangeTextDocumentParams::TextDocumentContentChangeEvent>::iterator
    // it = params.content_changes.begin();
    // std::for_each(params.content_changes.begin(),
    // params.content_changes.end(), [](const auto& event) {

    // });
  } else {
    logger.log("Unknown method ", message.method);
  }
}
