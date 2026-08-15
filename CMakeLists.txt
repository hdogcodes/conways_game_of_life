cmake_minimum_required(VERSION 3.22)
project(untitled10)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(untitled10 main.cpp)
target_link_libraries(untitled10 PRIVATE SFML::Graphics SFML::Window SFML::System)