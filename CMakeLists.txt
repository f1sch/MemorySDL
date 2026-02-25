cmake_minimum_required(VERSION 3.20)

project(SpookyMemory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(SpookyMemory WIN32
	src/main.cpp
	src/Game.cpp
)

#target_precompile_headers(SpookyMemory PRIVATE include/pch.h)

target_include_directories(SpookyMemory PRIVATE include)

target_link_libraries(SpookyMemory PRIVATE SDL3::SDL3)