#ifndef LETS_LSP_ANALYSIS_H
#define LETS_LSP_ANALYSIS_H

#include <map>
#include <optional>

#include "tree_sitter/api.h"

#include "log.h"
#include "lsp.h"

class State {
private:
  std::map<std::string, std::string> documents;
  Logger &logger;

public:
  State(Logger &logger) : logger(logger) {}

  void open_document(const std::string &uri, const std::string &content);
  void update_document(const std::string &uri, const std::string &content);
  std::optional<lsp::HoverResult> hover(const std::string &uri,
                                        const lsp::Position &position);
  std::optional<lsp::DefinitionResult>
  definition(const std::string &uri, const lsp::Position &position);
};

bool is_mixins_root_node(TSNode root_node, const std::string &doc,
                         const lsp::Position &position);

std::optional<std::string> extract_filename(TSNode root_node,
                                            const std::string &yaml_content,
                                            const lsp::Position &pos);
#endif
