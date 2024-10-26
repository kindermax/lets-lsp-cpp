#include <lsp.h>

namespace lsp {

// Parse bytes into json and construct a concrete subclass of Message
std::unique_ptr<Message> Message::parse(const std::string &msg) {
  json data = json::parse(msg);
  if (data.contains("method")) {
    auto method = data["method"].get<std::string>();
    if (data.contains("id")) {
      auto id = data["id"].template get<MessageId>();
      return std::make_unique<RequestMessage>(id, method, std::move(data));
    } else {
      return std::make_unique<NotificationMessage>(method, std::move(data));
    }
  }

  return nullptr;
}

json RequestMessage::to_json() const {
  return json{
      {"jsonrpc", "2.0"},
      {"id", id},
      {"method", method},
  };
}

json ResponseMessage::to_json() const {
  return json{
      {"jsonrpc", "2.0"},
      {"id", id},
      {"result", result},
  };
}

json NotificationMessage::to_json() const {
  return json{{"jsonrpc", "2.0"}, {"method", method}, {"params", data}};
}

json InitializeResponse::to_json() const {
  json data = ResponseMessage::to_json();
  data["result"] = {
      {"capabilities",
       {
           {"textDocumentSync", 1}, // 1 == Full
           {"hoverProvider", true},
           {"definitionProvider", true},
       }},
      {"serverInfo", {{"name", "lets-ls"}, {"version", "0.1.0"}}}};

  return data;
}

json HoverResponse::to_json() const {
  json data = ResponseMessage::to_json();

  if (result) {
    data["result"] = result.value();
  } else {
    data["result"] = nullptr;
  }

  return data;
}

void to_json(json &j, const HoverResult &result) {
  j = json{{"contents", result.contents}};
}

json DefinitionResponse::to_json() const {
  json data = ResponseMessage::to_json();

  if (result) {
    data["result"] = result.value();
  } else {
    data["result"] = nullptr;
  }

  return data;
}

void to_json(json &j, const DefinitionResult &result) { j = result.location; }

void to_json(json &j, const Location &loc) {
  j = json{{"uri", loc.uri}, {"range", loc.range}};
}

void to_json(json &j, const Range &range) {
  j = json{{"start", range.start}, {"end", range.end}};
}

void to_json(json &j, const Position &pos) {
  j = json{{"line", pos.line}, {"character", pos.character}};
}

void from_json(const json &j, InitializeParams &params) {
  const json &client_info = j.at("params").at("clientInfo");
  client_info.at("name").get_to(params.client_info.name);
  client_info.at("version").get_to(params.client_info.version);
}

void from_json(const json &j, TextDocumentItem &item) {
  j.at("uri").get_to(item.uri);
  j.at("languageId").get_to(item.language_id);
  j.at("version").get_to(item.version);
  j.at("text").get_to(item.text);
}

void from_json(const json &j, DidOpenTextDocumentParams &params) {
  j.at("params").at("textDocument").get_to(params.text_document_item);
}

void from_json(const json &j, DidChangeTextDocumentParams &params) {
  j.at("params").at("textDocument").get_to(params.text_document);
  j.at("params").at("contentChanges").get_to(params.content_changes);

  // for (const auto& item : j.at("params").at("contentChanges")) {
  //       DidChangeTextDocumentParams::TextDocumentContentChangeEvent event;
  //       from_json(item, event);
  //       params.content_changes.push_back(event);
  //   }
}

void from_json(const json &j, TextDocumentContentChangeEvent &event) {
  j.at("text").get_to(event.text);
}

void from_json(const json &j, VersionedTextDocumentIdentifier &identifier) {
  j.at("uri").get_to(identifier.uri);
  j.at("version").get_to(identifier.version);
}

void from_json(const json &j, HoverParams &params) {
  j.at("params").at("textDocument").get_to(params.text_document);
  j.at("params").at("position").get_to(params.position);
}

void from_json(const json &j, DefinitionParams &params) {
  j.at("params").at("textDocument").get_to(params.text_document);
  j.at("params").at("position").get_to(params.position);
}

void from_json(const json &j, TextDocumentIdentifier &identifier) {
  j.at("uri").get_to(identifier.uri);
}

void from_json(const json &j, Position &pos) {
  j.at("line").get_to(pos.line);
  j.at("character").get_to(pos.character);
}

} // namespace lsp
