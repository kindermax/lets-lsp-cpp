#include <cassert>

#include "lsp.h"

int main(int argc, char *argv[]) {

  std::string content = R"({
    "method": "initialize",
    "id": 1,
    "params": {
      "clientInfo": {
        "name": "lets-lsp",
        "version": "0.1.0"
      }
    }
  })";

  auto msg = lsp::Message::parse(content.c_str());
  assert(msg != nullptr);
  assert(msg->get_kind() == lsp::Message::Kind::Request);

  auto request = static_cast<lsp::RequestMessage *>(msg.get());
  assert(request->id == 1);
  assert(request->method == "initialize");
  assert(request->data["params"]["clientInfo"]["name"] == "lets-lsp");
  assert(request->data["params"]["clientInfo"]["version"] == "0.1.0");

  return 0;
}
