cmake_minimum_required(VERSION 3.20)

project(MemorySDL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL)

set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
add_subdirectory(external/SDL_image)

add_executable(MemorySDL WIN32)

target_sources(MemorySDL PRIVATE
	src/main.cpp
	src/Game.cpp
	src/AssetManager.cpp
	src/GridLayout.cpp
	src/SoundSystem.cpp
)

#target_precompile_headers(SpookyMemory PRIVATE include/pch.h)

#target_include_directories(MemorySDL PRIVATE include)

target_link_libraries(MemorySDL PRIVATE 
	SDL3::SDL3
	SDL3_image::SDL3_image
)

add_custom_command(TARGET MemorySDL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:MemorySDL>
)

add_custom_command(TARGET MemorySDL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE_DIR:MemorySDL>
)

file(GLOB_RECURSE GAME_ASSETS "${CMAKE_SOURCE_DIR}/assets/*")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:MemorySDL>/assets
)

add_dependencies(MemorySDL copy_assets)
