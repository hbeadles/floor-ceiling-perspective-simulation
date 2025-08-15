cmake_minimum_required(VERSION 3.20)
project(floor_ceil_perspective)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0167 NEW)
include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2_GFX REQUIRED SDL2_gfx)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/system/*.cpp"
    "src/game/*.cpp"
    "includes/base/*.hpp"
    "includes/system/*.hpp"
    "src/main.cpp"
)
add_executable (${CMAKE_PROJECT_NAME} ${SOURCES})
target_include_directories (
    ${CMAKE_PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
    ${PROJECT_SOURCE_DIR}/includes/base
    ${PROJECT_SOURCE_DIR}/includes/system
    ${PROJECT_SOURCE_DIR}/src/game
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
)
target_link_directories(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
    ${SDL2_GFX_LIBRARY_DIRS}
)
target_link_libraries (
 	${CMAKE_PROJECT_NAME}
    PRIVATE
 	${SDL2_LIBRARIES}
 	${SDL2_MIXER_LIBRARIES}
 	${SDL2_IMAGE_LIBRARIES}
 	${SDL2_TTF_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
)