#ifndef LETS_LSP_LSP_H
#define LETS_LSP_LSP_H

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
  static std::unique_ptr<Message> parse(char const *data);
};

class RequestMessage : public Message {
public:
  RequestMessage(int id, std::string method)
      : id(id), method(method) {}
  // TODO: id can be int or string. For now since we are hooked it with nvim it
  // will be int
  int id;
  std::string method;
  // TODO: implement params for request message

  Kind get_kind() const override { return Kind::Request; }
  json to_json() const override;
};

class ResponseMessage : public Message {
public:
  explicit ResponseMessage(const int id) : id(id) {}
  ResponseMessage(const int id, json result) : id(id), result(std::move(result)) {}
  int id;
  json result;

  Kind get_kind() const override { return Kind::Response; }
  json to_json() const override;
};

class NotificationMessage : public Message {
public:
  NotificationMessage(std::string method)
      : method(method) {}

  std::string method;

  Kind get_kind() const override { return Kind::Notification; }
  json to_json() const override;
};

// TODO: now it would be nice to reuse ResponseMessage to json here
class InitializeResponse : public ResponseMessage {
public:
  explicit InitializeResponse(int id): ResponseMessage(id) {}
  InitializeResponse(int id, const json &result)
    : ResponseMessage(id, result) {}

  json to_json() const override;
};


#endif