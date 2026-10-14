cmake_minimum_required(VERSION 3.18)
project(udp.connectivity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Library
add_library(${PROJECT_NAME}_lib
        include/UDPListener.h
        include/UDPBroadcaster.h
        src/UDPListener.cpp
        src/UDPBroadcaster.cpp)
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

# Executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)
install(TARGETS ${PROJECT_NAME})
