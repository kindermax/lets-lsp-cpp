#include <lsp.h>

namespace lsp {

// Parse bytes into json and construct a Message
// TODO: maybe use std::optional ?
std::unique_ptr<Message> Message::parse(const std::string& msg) {
  json data = json::parse(msg);
  if (data.contains("method")) {
    // TODO: "Notification" message has no "id"
    auto method = data["method"].get<std::string>();
    if (data.contains("id")) {
      auto id = data["id"].get<std::uint32_t>();
      return std::make_unique<RequestMessage>(id, method, data);
    } else {
      return std::make_unique<NotificationMessage>(method, data);
    }
  }

  return nullptr;
}

// TODO: rewrite this with json lib specific to_json
json RequestMessage::to_json() const {
  json data = {
      {"jsonrpc", "2.0"},
      {"id", id},
      {"method", method},
  };
  return data;
}

json ResponseMessage::to_json() const {
  json data = {
      {"jsonrpc", "2.0"},
      {"id", id},
      {"result", result},
  };
  return data;
}

json NotificationMessage::to_json() const {
  // TODO: finish
  json data = {
      {"jsonrpc", "2.0"},
  };
  return data;
}

json InitializeResponse::to_json() const {
  json data = ResponseMessage::to_json();
  data["result"] = {
      {"capabilities",
       {
           {"textDocumentSync", 1} // 1 == Full
       }},
      {"serverInfo", {{"name", "lets-lsp"}, {"version", "0.1.0"}}}};

  return data;
}
} // namespace lsp
