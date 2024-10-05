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
  return lsp::Message::parse(content.get());
}

std::unique_ptr<char[]>
Connection::read_content(const lsp::MessageHeader &header) {
  auto content = std::make_unique<char[]>(header.content_length + 1);
  in->read(content.get(), header.content_length);
  content[header.content_length] = '\n';
  logger.log(
      "Got message: Content-Length: " + std::to_string(header.content_length) +
      "\r\n\r\n" + std::string(content.get()));

  return content;
}

// Read and parse header: Content-Lenght: number\r\n\r\n
lsp::MessageHeader Connection::read_header() {
  lsp::MessageHeader header;

  // TODO: debug this
  // TODO: test it

  // TODO: maybe rewrite with getline
  while (true) {
    // 1. Read header
    // assume Content
    if (in->peek() == 'C') {
      // Ignore 'Content-' part
      in->ignore(8);
      // assume Length
      if (in->peek() == 'L') {
        // Ignore 'Length: ' part
        in->ignore(8);
        // TODO: how does this conversion work ?
        (*in) >> header.content_length;
      }
    }

    // assume \r and \n delimiters
    char c1 = in->get();
    char c2 = in->get();

    if (c1 == '\r' && c2 == '\n') {
      char c3 = in->get();
      char c4 = in->get();
      if (c3 == '\r' && c4 == '\n') {
        // Done parsing header
        return header;
      }
    }
  }
  return header;
}

void Connection::write(const lsp::Message &msg) {
  const auto data = msg.to_json();
  const auto raw = data.dump();
  (*out) << "Content-Length: " << raw.size() << "\r\n\r\n" << raw << std::flush;
};
