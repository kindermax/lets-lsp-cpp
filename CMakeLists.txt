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

FetchContent_Declare(
    treesitter
    GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter
    GIT_TAG        v0.24.3
)

FetchContent_Declare(
    treesitter-yaml
    GIT_REPOSITORY https://github.com/ikatyang/tree-sitter-yaml.git
    GIT_TAG        v0.5.0
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(argparse)
FetchContent_MakeAvailable(treesitter)
FetchContent_MakeAvailable(treesitter-yaml)

# setup tree-sitter
add_library(tree-sitter ${treesitter_SOURCE_DIR}/lib/src/lib.c)
target_include_directories(tree-sitter PUBLIC ${treesitter_SOURCE_DIR}/lib/src ${treesitter_SOURCE_DIR}/lib/include)

# setup tree-sitter-yaml
add_library(tree-sitter-yaml
  ${treesitter-yaml_SOURCE_DIR}/src/parser.c
  ${treesitter-yaml_SOURCE_DIR}/src/scanner.cc
)

target_include_directories(tree-sitter-yaml
  PRIVATE
    # parser.h is stored within the src directory, so we need to include
    # src in the search paths
    $<BUILD_INTERFACE:${treesitter-yaml_SOURCE_DIR}/src>
)

target_link_libraries(tree-sitter-yaml INTERFACE tree-sitter)

# link dependencies to main target
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
    argparse
    tree-sitter
    tree-sitter-yaml
)

# Testing
include(CTest)
enable_testing()
add_subdirectory(tests)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
