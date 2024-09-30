#include "analysis.h"

void State::open_document(const std::string uri, const std::string content) {
  documents[std::move(uri)] = std::move(content);
}
