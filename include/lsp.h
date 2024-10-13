#ifndef LETS_LSP_LSP_H
#define LETS_LSP_LSP_H

#include <string>
#include <variant>

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

  static std::unique_ptr<Message> parse(const std::string &msg);
};

using MessageId = std::variant<int, std::string>;

class RequestMessage : public Message {
public:
  RequestMessage(MessageId id, std::string method, json data)
      : id(id), method(method), data(std::move(data)) {}
  MessageId id;
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

struct TextDocumentItem {
  std::string uri;
  std::string language_id;
  int version;
  std::string text;
};

class DidOpenTextDocumentParams {
public:
  TextDocumentItem text_document_item;
};

struct TextDocumentContentChangeEvent {
  // TODO: support incremental change
  std::string text;
};

struct VersionedTextDocumentIdentifier {
  std::string uri;
  int version;
};

class DidChangeTextDocumentParams {
public:
  VersionedTextDocumentIdentifier text_document;
  std::vector<TextDocumentContentChangeEvent> content_changes;
};

struct TextDocumentIdentifier {
  std::string uri;
};

struct Position {
  unsigned int line;
  unsigned int character;
};

struct Range {
  Position start;
  Position end;
};

struct Location {
  std::string uri;
  Range range;
};

struct TextDocumentPositionParams {
  TextDocumentIdentifier text_document;
  Position position;
};

class HoverParams : public TextDocumentPositionParams {};

struct HoverResult {
  std::string contents;
};

class DefinitionParams : public TextDocumentPositionParams {};

struct DefinitionResult {
  Location location;
};

class ResponseMessage : public Message {
public:
  explicit ResponseMessage(const MessageId id) : id(id) {}
  ResponseMessage(const MessageId id, json result)
      : id(id), result(std::move(result)) {}
  MessageId id;
  json result;

  Kind get_kind() const override { return Kind::Response; }
  json to_json() const override;
};

class NotificationMessage : public Message {
public:
  NotificationMessage(std::string method, json data)
      : method(method), data(std::move(data)) {}

  std::string method;
  json data;

  Kind get_kind() const override { return Kind::Notification; }
  json to_json() const override;
};

class InitializeResponse : public ResponseMessage {
public:
  explicit InitializeResponse(MessageId id) : ResponseMessage(id) {}

  json to_json() const override;
};

class HoverResponse : public ResponseMessage {
public:
  explicit HoverResponse(MessageId id, std::optional<HoverResult> result)
      : ResponseMessage(id), result(std::move(result)) {}

  json to_json() const override;

  std::optional<HoverResult> result;
};

class DefinitionResponse : public ResponseMessage {
public:
  explicit DefinitionResponse(MessageId id,
                              std::optional<DefinitionResult> result)
      : ResponseMessage(id), result(std::move(result)) {}

  json to_json() const override;

  std::optional<DefinitionResult> result;
};

void from_json(const json &j, InitializeParams &params);
void from_json(const json &j, TextDocumentItem &item);
void from_json(const json &j, DidOpenTextDocumentParams &params);
void from_json(const json &j, DidChangeTextDocumentParams &params);
void from_json(const json &j, TextDocumentContentChangeEvent &event);
void from_json(const json &j, VersionedTextDocumentIdentifier &identifier);
void from_json(const json &j, HoverParams &params);
void from_json(const json &j, DefinitionParams &params);
void from_json(const json &j, TextDocumentIdentifier &identifier);
void from_json(const json &j, Position &pos);

void to_json(json &j, const HoverResult &result);
void to_json(json &j, const DefinitionResult &result);
void to_json(json &j, const Range &range);
void to_json(json &j, const Location &loc);
void to_json(json &j, const Position &pos);

} // namespace lsp

namespace nlohmann {
template <> struct adl_serializer<lsp::MessageId> {
  static void to_json(json &j, const lsp::MessageId &value) {
    j = std::visit([](auto &&value) -> json { return value; }, value);
  }

  static void from_json(const json &j, lsp::MessageId &value) {
    if (j.is_number()) {
      value = j.get<int>();
    } else if (j.is_string()) {
      value = j.get<std::string>();
    } else {
      throw std::invalid_argument("Unsupported JSON type for MessageId");
    }
  }
};
} // namespace nlohmann

#endif
