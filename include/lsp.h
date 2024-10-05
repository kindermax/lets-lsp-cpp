#ifndef LETS_LSP_LSP_H
#define LETS_LSP_LSP_H

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace lsp {
// jsonrpc messages

class MessageHeader {
public:
  int content_length = 0;
};

// Abstract Message type
class Message {
public:
  enum Kind { Request, Response, Notification };

  virtual ~Message() = default;
  virtual Kind get_kind() const = 0;
  virtual json to_json() const = 0;

  // TODO: maybe make parse a pure virtual function
  static std::unique_ptr<Message> parse(const std::string& msg);
};

class RequestMessage : public Message {
public:
  RequestMessage(int id, std::string method, json data)
      : id(id), method(method), data(data) {}
  // TODO: id can be int or string. For now since we are hooked it with nvim it
  // will be int. Use variant later.
  int id;
  std::string method;
  // data holds whole json message
  json data;

  Kind get_kind() const override { return Kind::Request; }
  json to_json() const override;
};

class InitializeParams {
public:
  struct ClientInfo {
    std::string name;
    std::string version;
  } client_info;
};

inline void from_json(const json &j, InitializeParams &params) {
  const json &client_info = j.at("params").at("clientInfo");
  client_info.at("name").get_to(params.client_info.name);
  client_info.at("version").get_to(params.client_info.version);
}

class DidOpenTextDocumentParams {
public:
  struct TextDocumentItem {
    std::string uri;
    std::string language_id;
    int version;
    std::string text;
  } text_document_item;
};

inline void from_json(const json &j, DidOpenTextDocumentParams &params) {
  const json &text_document = j.at("params").at("textDocument");
  text_document.at("uri").get_to(params.text_document_item.uri);
  text_document.at("languageId").get_to(params.text_document_item.language_id);
  text_document.at("version").get_to(params.text_document_item.version);
  text_document.at("text").get_to(params.text_document_item.text);
}

class ResponseMessage : public Message {
public:
  explicit ResponseMessage(const int id) : id(id) {}
  ResponseMessage(const int id, json result)
      : id(id), result(std::move(result)) {}
  int id;
  json result;

  Kind get_kind() const override { return Kind::Response; }
  json to_json() const override;
};

class NotificationMessage : public Message {
public:
  NotificationMessage(std::string method, json data)
      : method(method), data(data) {}

  std::string method;
  json data;

  Kind get_kind() const override { return Kind::Notification; }
  json to_json() const override;
};

// TODO: now it would be nice to reuse ResponseMessage to json here
// TODO: maybe use autoconversion
// https://json.nlohmann.me/features/arbitrary_types/
class InitializeResponse : public ResponseMessage {
public:
  explicit InitializeResponse(int id) : ResponseMessage(id) {}
  InitializeResponse(int id, const json &result)
      : ResponseMessage(id, result) {}

  json to_json() const override;
};

} // namespace lsp

#endif
