if(BUILD_TESTING)
  # TODO: make code as library and link to tests
  add_executable(test_message test_message.cpp ../src/lsp.cpp)
  add_executable(test_connection test_connection.cpp ../src/lsp.cpp ../src/rpc.cpp ../src/log.cpp)

  target_link_libraries(
    test_message
    PRIVATE nlohmann_json::nlohmann_json
    GTest::gtest_main
  )

  target_link_libraries(
    test_connection
    PRIVATE nlohmann_json::nlohmann_json
    GTest::gtest_main
  )

  include(GoogleTest)

  gtest_discover_tests(test_message)
  gtest_discover_tests(test_connection)
endif()
