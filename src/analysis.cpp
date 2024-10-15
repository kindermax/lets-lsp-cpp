#include <sstream>
#include <string>

#include <tree_sitter/api.h>

#include "analysis.h"
#include "lsp.h"

#include "fmt/base.h"
#include <fmt/core.h>

extern "C" {
TSLanguage *tree_sitter_yaml();
}

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

/**
 * Handle go to definition for:
 * - mixins
 * - depends
 */
std::optional<lsp::DefinitionResult>
State::definition(const std::string &uri, const lsp::Position &position) {
  if (documents.find(uri) == documents.end()) {
    return std::nullopt;
  }

  auto &doc = documents.at(uri);

  std::istringstream stream(doc);
  std::string line;
  std::optional<std::string> target_file;
  int line_index = 0;
  while (std::getline(stream, line, '\n')) {
    if (line_index == position.line) {
      logger.log("Found line: ", line);
      target_file =
          line.substr(line.find("-") + 2); // 4 is a ident, this is temp
      logger.log("Found target_file: ", target_file.value());
      break;
    }
    line_index++;
  }

  if (target_file == std::nullopt) {
    return std::nullopt;
  }

  // TODO: find better way to work with file paths
  std::size_t last_slash_pos = uri.find_last_of("/");
  std::string dir = uri.substr(0, last_slash_pos + 1);
  std::string new_uri = dir + target_file.value();

  return lsp::DefinitionResult{lsp::Location{
      new_uri, lsp::Range{lsp::Position{0, 0}, lsp::Position{0, 0}}}};
};
