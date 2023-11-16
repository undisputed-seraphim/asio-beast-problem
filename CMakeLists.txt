cmake_minimum_required(VERSION 3.20)
project(asio-problem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system json)

file(GLOB common_src "${CMAKE_SOURCE_DIR}/common/*")
add_library(common STATIC ${common_src})
target_include_directories(common PUBLIC "${CMAKE_SOURCE_DIR}/common")
target_link_libraries(common PUBLIC Boost::system Boost::boost Boost::json)

file(GLOB client_src "${CMAKE_SOURCE_DIR}/client/*")
add_executable(client ${client_src})
target_link_libraries(client PRIVATE common)

file(GLOB server_src "${CMAKE_SOURCE_DIR}/server/*")
add_executable(server ${server_src})
target_link_libraries(server PRIVATE common)