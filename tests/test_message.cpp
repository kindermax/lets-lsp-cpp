#include <gtest/gtest.h>

#include "lsp.h"

TEST(Message, ParseRequestMessageJson) {
  std::string content = R"({
    "method": "initialize",
    "id": 1,
    "params": {
      "clientInfo": {
        "name": "lets-ls",
        "version": "0.1.0"
      }
    }
  })";

  auto msg = lsp::Message::parse(content);
  ASSERT_NE(msg, nullptr);
  EXPECT_EQ(msg->get_kind(), lsp::Message::Kind::Request);

  auto request = static_cast<lsp::RequestMessage *>(msg.get());
  EXPECT_EQ(request->id, 1);
  EXPECT_EQ(request->method, "initialize");
  EXPECT_EQ(request->data["params"]["clientInfo"]["name"], "lets-ls");
  EXPECT_EQ(request->data["params"]["clientInfo"]["version"], "0.1.0");
}

TEST(Message, ParseNotificationMessageJson) {
  std::string content = R"({
    "method": "initialized",
    "params": {}
  })";

  auto msg = lsp::Message::parse(content);
  ASSERT_NE(msg, nullptr);
  EXPECT_EQ(msg->get_kind(), lsp::Message::Kind::Notification);

  auto request = static_cast<lsp::NotificationMessage *>(msg.get());
  EXPECT_EQ(request->method, "initialized");
  EXPECT_EQ(request->data["params"].size(), 0);
}
