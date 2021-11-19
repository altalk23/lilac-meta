cmake_minimum_required(VERSION 3.8)

project(lilac_meta LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories("${CMAKE_SOURCE_DIR}/include")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("test")
endif()
