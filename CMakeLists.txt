cmake_minimum_required(VERSION 3.0)
project(cpplibsocket_examples VERSION 1.0.0 LANGUAGES C CXX)

if (UNIX)
	include("cmake/Linux-compiler-config.txt")
elseif (MSVC)
    include("cmake/Windows-compiler-config.txt")
endif()

add_executable(client
    src/client.cpp
)

add_executable(server
    src/server.cpp
)

include_directories(
    include
)

add_subdirectory(
    include/third_party/cpplibsocket
)

target_link_libraries(client
    PRIVATE cpplibsocket
)

target_link_libraries(server
    PRIVATE cpplibsocket
)
