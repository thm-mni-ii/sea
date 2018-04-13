cmake_minimum_required(VERSION 3.9)
project(sea)
#set(PROJECT_SOURCE_DIR .)
include(gtest.cmake)

SET(COVERAGE OFF CACHE BOOL "Coverage")

# Project Structure
set(LIBRARY_OUTPUT_PATH lib)                                    # generated library files
set(EXECUTABLE_OUTPUT_PATH bin)                                 # generated binary files

set(MAIN_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)                 # full name of the main file
set(CMAKE_INCLUDE_PATH include/)                                # all headers
file(GLOB SOURCES "src/*.cpp")                                  # all sources inside src/ except for the main file
list(REMOVE_ITEM SOURCES ${MAIN_FILE})
file(GLOB TESTS "test/*.cpp")                                   # all test files

include_directories(${CMAKE_INCLUDE_PATH})

add_library(sealib SHARED ${SOURCES})

add_executable(sealib_main ${MAIN_FILE} ${SOURCES})

add_executable(tests ${SOURCES} ${TESTS})
find_package (Threads)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(sealib_main Qt5::Core)
target_link_libraries(sealib_main Qt5::Gui)
target_link_libraries(sealib_main Qt5::Widgets)



if (MSVC)
    target_compile_options(tests PRIVATE /std:c++17 /W4 /WX)
    target_compile_options(sealib_main PRIVATE /std:c++17 /W4 /WX)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")

    # VS 2017 removed tr1
    add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
else()
    target_compile_options(tests PRIVATE -std=c++14 -g -Wall -Werror -Wextra -Wpedantic -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef)
    target_compile_options(sealib_main PRIVATE -std=c++14 -g -Wall -Werror -Wextra -Wpedantic -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef)

    if (COVERAGE)
        MESSAGE( STATUS "Link Coverage")
        target_compile_options(tests PRIVATE --coverage)
        target_link_libraries(tests --coverage)
    endif()
endif()