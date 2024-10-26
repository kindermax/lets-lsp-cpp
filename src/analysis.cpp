#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>

#include <tree_sitter/api.h>

#include "analysis.h"
#include "lsp.h"

#include "fmt/base.h"
#include <fmt/core.h>

extern "C" {
TSLanguage *tree_sitter_yaml();
}

void State::open_document(const std::string& uri, const std::string& content) {
  documents[uri] = content;
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

std::vector<TSQueryCapture>
run_query(TSNode root_node, const std::string &query_string) {
  uint32_t error_offset = 0;
  TSQueryError error_type;

  TSQuery *query = ts_query_new(tree_sitter_yaml(), query_string.c_str(),
                                query_string.size(), &error_offset, &error_type);

  if (error_type != TSQueryErrorNone) {
    std::cerr << "Error while creating query: " << error_type << std::endl;
  }

  TSQueryCursor *query_cursor = ts_query_cursor_new();

  ts_query_cursor_exec(query_cursor, query, root_node);

  std::vector<TSQueryCapture> captures;

  uint32_t capture_index = 0;
  TSQueryMatch match;

  while (ts_query_cursor_next_capture(query_cursor, &match, &capture_index)) {
    TSQueryCapture capture = match.captures[capture_index];
    captures.push_back(capture);
  }

  // Cleanup
  ts_query_cursor_delete(query_cursor);
  ts_query_delete(query);

  return captures;
}

const std::string mixins_node_query = R"(
  (block_mapping_pair
    key: (flow_node) @key
    value: (block_node (block_sequence
      (block_sequence_item (flow_node) @value)
    ))
    (#eq? @key "mixins")
  )
)";

std::string get_node_text(TSNode node, const std::string &doc) {
  uint32_t start_byte = ts_node_start_byte(node);
  uint32_t end_byte = ts_node_end_byte(node);
  return doc.substr(start_byte, end_byte - start_byte);
}

// Function to check if the cursor is within a node
bool is_cursor_within_node(TSNode node, const lsp::Position &pos) {
  TSPoint start_point = ts_node_start_point(node);
  TSPoint end_point = ts_node_end_point(node);
  return pos.line >= start_point.row && pos.line <= end_point.row &&
         pos.character >= start_point.column &&
         pos.character <= end_point.column;
}

// Function to check if the current node is part of the 'mixins' block
bool is_mixins_root_node(TSNode root_node, const std::string &doc, const lsp::Position &pos) {
  auto captures = run_query(root_node, mixins_node_query);

  for (const auto &capture : captures) {
    TSNode captured_node = capture.node;

    TSNode parent = ts_node_parent(captured_node);

    if (!ts_node_is_null(parent) &&
        strcmp(ts_node_type(parent), "block_mapping_pair") == 0 &&
        get_node_text(captured_node, doc) == "mixins") {

      if (is_cursor_within_node(parent, pos)) {
        return true;
      }
    }
  }
  return false;
}

// Function to extract the filename from the 'mixins' block if the cursor is in
// that context
std::optional<std::string> extract_filename(TSNode root_node, const std::string &yaml_content, const lsp::Position &pos) {
  auto captures = run_query(root_node, mixins_node_query);

  for (const auto &capture : captures) {
    TSNode captured_node = capture.node;

    TSNode parent = ts_node_parent(captured_node);

    if (!ts_node_is_null(parent) &&
        strcmp(ts_node_type(parent), "block_sequence_item") == 0) {

      TSPoint start_point = ts_node_start_point(captured_node);
      TSPoint end_point = ts_node_end_point(captured_node);

      if (pos.line == start_point.row && pos.line == end_point.row) {
        // Extract the filename
        return get_node_text(captured_node, yaml_content);
      }
    }
  }
  return std::nullopt;
}

std::string go_to_def_filename(const std::string &uri,
                               const std::string &filename) {
  auto dir = std::filesystem::path(uri).parent_path();
  return dir / filename;
}

// TODO create Parser class with destructor to cleanup

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

  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_yaml());

  TSTree *tree =
      ts_parser_parse_string(parser, nullptr, doc.c_str(), doc.length());
  TSNode root_node = ts_tree_root_node(tree);

  logger.log(fmt::format("Parsed doc {}", uri));

  if (is_mixins_root_node(root_node, doc, position)) {
    // TODO: support remote files
    std::optional<std::string> filename =
        extract_filename(root_node, doc, position);
    if (filename) {
      auto new_uri = go_to_def_filename(uri, *filename);
      return lsp::DefinitionResult{lsp::Location{
          new_uri, lsp::Range{lsp::Position{0, 0}, lsp::Position{0, 0}}}};
    }
  }

  // Cleanup
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  return std::nullopt;
};
