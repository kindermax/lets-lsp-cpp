if(BUILD_TESTING)
  # TODO: add GTest
  # TODO: make code as library and link to tests
  add_executable(test_parse_message test_parse_message.cpp ../src/lsp.cpp)

  target_link_libraries(test_parse_message
    PRIVATE nlohmann_json::nlohmann_json )

  add_test (TestParseMessage test_parse_message)
endif()
