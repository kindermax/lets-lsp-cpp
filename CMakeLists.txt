cmake_minimum_required(VERSION 3.5.0)
project(lets_ls VERSION 0.1.0 LANGUAGES C CXX)

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
include_directories(include)

set(BIN_SOURCES
    src/main.cpp
    src/log.cpp
    src/rpc.cpp
    src/lsp.cpp
    src/analysis.cpp
    src/server.cpp
)

add_executable(${PROJECT_NAME} ${BIN_SOURCES})

# Dependencies
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        11.0.2
)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG        v3.1
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(argparse)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json fmt::fmt argparse)

# Testing
include(CTest)
enable_testing()
add_subdirectory(tests)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
