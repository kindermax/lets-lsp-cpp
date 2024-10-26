// #include <cctype>
#include <string>

#include <gtest/gtest.h>
#include <tree_sitter/api.h>

#include "analysis.h"
#include "lsp.h"

extern "C" {
TSLanguage *tree_sitter_yaml();
}

using Predicate = std::function<int(int)>;

void trim(std::string &str, Predicate const &pred = isspace) {
  // ltrim
  str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), pred));
  // rtrim
  str.erase((std::find_if_not(str.rbegin(), str.rend(), pred)).base(),
            str.end());
}

TEST(Analysis_GoToDefinition, DetectMixinsNode) {
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_yaml());

  std::string doc = R"(
shell: bash
mixins:
  - lets.my.yaml
commands:
  test:
    cmd: echo Test
  )";
  trim(doc);

  TSTree *tree =
      ts_parser_parse_string(parser, nullptr, doc.c_str(), doc.length());
  TSNode root_node = ts_tree_root_node(tree);

  using test = std::pair<lsp::Position, bool>;

  const std::vector<test> tests{
      {lsp::Position{0, 0}, false}, {lsp::Position{1, 0}, true},
      {lsp::Position{2, 0}, true},  {lsp::Position{2, 15}, true},
      {lsp::Position{3, 0}, false},
  };

  for (const auto &test : tests) {
    ASSERT_EQ(is_mixins_root_node(root_node, doc, test.first),
              test.second);
  }

  // Cleanup
  ts_tree_delete(tree);
  ts_parser_delete(parser);
}

TEST(Analysis_GoToDefinition, ExtractFilenameFromMixinsNode) {
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_yaml());

  std::string doc = R"(
shell: bash
mixins:
  - lets.my.yaml
commands:
  test:
    cmd: echo Test
  )";
  trim(doc);

  TSTree *tree =
      ts_parser_parse_string(parser, nullptr, doc.c_str(), doc.length());
  TSNode root_node = ts_tree_root_node(tree);

  using test = std::pair<lsp::Position, std::optional<std::string>>;

  const std::vector<test> tests{
      {lsp::Position{1, 0}, std::nullopt},
      {lsp::Position{2, 0}, "lets.my.yaml"},
      {lsp::Position{2, 15}, "lets.my.yaml"},
  };

  for (const auto &test : tests) {
    ASSERT_EQ(extract_filename(root_node, doc, test.first),
              test.second);
  }

  // Cleanup
  ts_tree_delete(tree);
  ts_parser_delete(parser);
}
