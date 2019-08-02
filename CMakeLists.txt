cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Architecture
set (ARCH amd64)

set (CMAKE_CXX_STANDARD 14)

set (JSON_INCLUDE /usr/local/include/nlohmann)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

set(LIB_FILES
    src/afmSocket.cpp
    src/afmClientSocket.cpp
    src/afmServerSocket.cpp
)

set(SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
    common
    ${JSON_INCLUDE}
)

link_directories (
    ${LIB_DIR}
)

add_library (AFMSocket ${LIB_FILES})
add_executable (AFMSocketClientTest ${SRC_FILES})

target_link_libraries(
    AFMSocketClientTest
    AFMSocket
    pthread
)

install (TARGETS AFMSocketClientTest DESTINATION ${BIN_DIR})

