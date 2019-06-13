cmake_minimum_required (VERSION 3.2)
project (GameBoyCore)

set (CMAKE_CXX_STANDARD 11)

file(GLOB GameBoyCore_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(gbcore STATIC ${GameBoyCore_SRC})

if(NOT WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Werror")
endif ()