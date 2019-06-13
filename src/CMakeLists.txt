cmake_minimum_required (VERSION 3.0)
project (GameBoy)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(core)
include_directories(core)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

add_subdirectory(glfw)
include_directories(glfw)

file(GLOB GameBoy_SRC
    "${CMAKE_SOURCE_DIR}/*.h"
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

find_package(OpenGL)

if (APPLE)
add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

add_executable(gbrun ${GameBoy_SRC})
target_link_libraries(gbrun gbcore ${OPENGL_gl_LIBRARY})

if (UNIX)
target_link_libraries(gbrun glfw)
elseif (WIN32)
target_link_libraries(gbrun glfw3)
elseif (APPLE)
target_link_libraries(gbrun glfw)
endif ()

if(NOT WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Werror")
endif()