cmake_minimum_required(VERSION 3.5.0)
project(lets-lsp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include/
)

set(BIN_SOURCES
    src/main.cpp
    src/log.cpp
    src/rpc.cpp
    src/lsp.cpp
    src/server.cpp
)

add_executable(lets-lsp ${BIN_SOURCES})

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(lets-lsp PRIVATE nlohmann_json::nlohmann_json)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
