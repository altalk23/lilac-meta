cmake_minimum_required(VERSION 3.8)

project(lilac_meta LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory("test")