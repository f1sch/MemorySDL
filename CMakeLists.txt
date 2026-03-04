cmake_minimum_required(VERSION 3.20)

project(MemorySDL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(external/SDL)

set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF support" FORCE)
#option(SDLIMAGE_AVIF "Enable AVIF support" OFF)
add_subdirectory(external/SDL_image)

add_executable(MemorySDL)

file(GLOB_RECURSE GAME_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

target_sources(MemorySDL PRIVATE 
    ${GAME_SOURCES}
)

target_link_libraries(MemorySDL PRIVATE
	SDL3::SDL3
	SDL3_image::SDL3_image
)

if (WIN32)
    set_target_properties(MemorySDL PROPERTIES WIN32_EXECUTABLE TRUE)

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
endif()

#target_include_directories(MemorySDL PRIVATE
#    ${CMAKE_SOURCE_DIR}/include
#)

#target_precompile_headers(SpookyMemory PRIVATE include/pch.h)


# Copy assets after build
if (NOT EMSCRIPTEN)
    add_custom_command(TARGET MemorySDL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:MemorySDL>/assets
    )
endif()

if (EMSCRIPTEN)
    message(STATUS "Building for Web (Emscripten)")

    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(MemorySDL PROPERTIES OUTPUT_NAME "index")

    target_link_options(MemorySDL PRIVATE
        "-O3"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sINITIAL_MEMORY=64MB"
        "-sUSE_SDL=3"
        "--preload-file" "${CMAKE_SOURCE_DIR}/assets@/assets"
        "--shell-file" "${CMAKE_SOURCE_DIR}/web/index.html"
    )
endif()