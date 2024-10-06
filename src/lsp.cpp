#include <lsp.h>

namespace lsp {

// Parse bytes into json and construct a concrete subclass of Message
std::unique_ptr<Message> Message::parse(const std::string& msg) {
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
  return json{
      {"jsonrpc", "2.0"},
      {"method", method},
      {"params", data}
  };
}

json InitializeResponse::to_json() const {
  json data = ResponseMessage::to_json();
  data["result"] = {
      {"capabilities",
       {
           {"textDocumentSync", 1} // 1 == Full
       }},
      {"serverInfo", {{"name", "lets-ls"}, {"version", "0.1.0"}}}};

  return data;
}

void from_json(const json &j, InitializeParams &params) {
    const json &client_info = j.at("params").at("clientInfo");
    client_info.at("name").get_to(params.client_info.name);
    client_info.at("version").get_to(params.client_info.version);
}

void from_json(const json &j, DidOpenTextDocumentParams &params) {
  const json &text_document = j.at("params").at("textDocument");
  text_document.at("uri").get_to(params.text_document_item.uri);
  text_document.at("languageId").get_to(params.text_document_item.language_id);
  text_document.at("version").get_to(params.text_document_item.version);
  text_document.at("text").get_to(params.text_document_item.text);
}

} // namespace lsp
