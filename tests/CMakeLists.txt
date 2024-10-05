if(BUILD_TESTING)
  # TODO: make code as library and link to tests
  add_executable(test_message test_message.cpp ../src/lsp.cpp)

  target_link_libraries(
    test_message
    PRIVATE nlohmann_json::nlohmann_json
    GTest::gtest_main
  )

  include(GoogleTest)

  gtest_discover_tests(test_message)
endif()
