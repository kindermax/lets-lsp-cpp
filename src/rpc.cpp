// #include <cassert>
#include <memory>
#include <ostream>
#include <string>

#include "log.h"
#include "rpc.h"

std::unique_ptr<lsp::Message> Connection::read() {
  logger.log("Reading message");

  auto header = read_header();
  if (header.content_length == 0) {
    logger.log("Invalid message");
    return nullptr;
  }

  auto content = read_content(header);
  return lsp::Message::parse(content);
}

std::string Connection::read_content(const lsp::MessageHeader &header) {
  std::string content;
  content.resize(header.content_length);
  in->read(&content[0], header.content_length);
  logger.log("Got message: Content-Length: " +
             std::to_string(header.content_length) + "\r\n\r\n" + content);

  return content;
}

// Read and parse header: Content-Lenght: number\r\n\r\n
lsp::MessageHeader Connection::read_header() {
  lsp::MessageHeader header;

  if (in->eof()) {
    throw "Connection lost";
  }

  std::string line;
  while (std::getline(*in, line, '\r')) {
    const auto sepIdx = line.find(":");
    if (sepIdx != std::string::npos) {
      const auto key = line.substr(0, sepIdx);
      const auto value = line.substr(sepIdx + 1);
      if (key == "Content-Length") {
        header.content_length = std::stoi(value);
      }
    }

    if (line == "\n") {
      break;
    }
  }

  in->get(); // consume \n
  return header;
}

void Connection::write(const lsp::Message &msg) {
  const auto data = msg.to_json();
  const auto raw = data.dump();
  (*out) << "Content-Length: " << raw.size() << "\r\n\r\n" << raw << std::flush;
};
