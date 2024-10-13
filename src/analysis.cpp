#include "analysis.h"
#include "lsp.h"

#include "fmt/base.h"
#include <fmt/core.h>

void State::open_document(const std::string uri, const std::string content) {
  documents[std::move(uri)] = std::move(content);
}

void State::update_document(const std::string uri, const std::string content) {
  documents[std::move(uri)] = std::move(content);
}

std::optional<lsp::HoverResult> State::hover(const std::string &uri,
                                             const lsp::Position &position) {
  if (documents.find(uri) == documents.end()) {
    return std::nullopt;
  }

  auto &doc = documents.at(uri);
  return lsp::HoverResult{fmt::format("URI: {}, Size: {}", uri, doc.size())};
};

std::optional<lsp::DefinitionResult>
State::definition(const std::string &uri, const lsp::Position &position) {
  if (documents.find(uri) == documents.end()) {
    return std::nullopt;
  }

  auto &doc = documents.at(uri);

  // TODO: find better way to work with file paths
  std::size_t last_slash_pos = uri.find_last_of("/");
  std::string dir = uri.substr(0, last_slash_pos + 1);
  // TODO: hardcoded file name until we have a way to get the actual file name
  // from the document;
  std::string new_uri = dir + "lets.my.yaml";

  return lsp::DefinitionResult{lsp::Location{
      new_uri, lsp::Range{lsp::Position{0, 0}, lsp::Position{0, 0}}}};
};
