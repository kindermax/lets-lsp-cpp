#include <gtest/gtest.h>

#include "log.h"
#include "lsp.h"
#include "rpc.h"

TEST(Connection, Read) {
  auto logger = NullLogger();

  std::string raw_header("Content-Length: 52\r\n\r\n");
  std::string payload(raw_header + R"({"params":{},"method":"initialized","jsonrpc":"2.0"})");

  auto in = std::istringstream(payload);
  auto out = std::ostringstream();

  Connection conn(in, out, logger);

  auto header = conn.read_header();
  ASSERT_EQ(header.content_length, 52);

  auto content = conn.read_content(header);

  auto msg = lsp::Message::parse(content.get());

  ASSERT_NE(msg, nullptr);
  EXPECT_EQ(msg->get_kind(), lsp::Message::Kind::Notification);
}
