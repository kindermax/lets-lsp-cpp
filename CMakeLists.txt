cmake_minimum_required(VERSION 3.5.0)
project(lets-lsp VERSION 0.1.0 LANGUAGES C CXX)

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
include_directories(
    include/
)

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
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Testing
include(CTest)
enable_testing()
add_subdirectory(tests)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
