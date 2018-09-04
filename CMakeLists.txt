cmake_minimum_required(VERSION 3.0)
project(sea)
set(PROJECT_SOURCE_DIR .)

SET(COVERAGE OFF CACHE BOOL "Coverage")

# Project Structure
set(LIBRARY_OUTPUT_PATH lib)                                    # generated library files
set(EXECUTABLE_OUTPUT_PATH bin)                                 # generated binary files
set(CMAKE_INCLUDE_PATH include/)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# --------------------------------------------------- #
#                   Dependencies
# --------------------------------------------------- #
include(ExternalProject)
find_program(MAKE NAMES cmake make)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# --------------------------------------------------- #
#                   gtest library
# --------------------------------------------------- #
ExternalProject_Add(GTEST_PROJECT
        GIT_REPOSITORY https://github.com/google/googletest.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/gtest
        CMAKE_ARGS "-DBUILD_GTEST=ON -DBUILD_GMOCK=OFF -DINSTALL_GTEST=OFF -DINSTALL_GMOCK=OFF"
        INSTALL_COMMAND ""
        BUILD_COMMAND "${MAKE}" --build .)

ExternalProject_Get_Property(GTEST_PROJECT source_dir)
ExternalProject_Get_Property(GTEST_PROJECT binary_dir)
set(GOOGLETEST_ROOT ${source_dir})
set(GOOGLETEST_BIN ${binary_dir})
MESSAGE(STATUS "GTest source_dir: ${GOOGLETEST_ROOT}")
MESSAGE(STATUS "GTest build_dir: ${GOOGLETEST_BIN}")

include_directories(SYSTEM ${GOOGLETEST_ROOT}/googletest/)
include_directories(SYSTEM ${GOOGLETEST_ROOT}/googletest/include)
add_library(gtest STATIC IMPORTED)
add_dependencies(gtest GTEST_PROJECT)
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${GOOGLETEST_BIN}/googlemock/gtest/libgtest.a")

# --------------------------------------------------- #
#                   build library
# --------------------------------------------------- #
include_directories(${CMAKE_INCLUDE_PATH})
file(GLOB LIB_SOURCES "src/*.cpp")

add_library(sealib SHARED ${LIB_SOURCES})
target_compile_options(sealib PRIVATE -std=c++11 -g -O3 -Wall -Werror -Wextra -Wpedantic -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef)

# --------------------------------------------------- #
#                   build tests
# --------------------------------------------------- #
file(GLOB TESTS "test/*.cpp")                                   # all test files
add_executable(tests ${LIB_SOURCES} ${TESTS} test/dycktable_test.cpp)
add_dependencies(tests gtest)
find_package(Threads)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(tests PRIVATE -std=c++11 -g -O3 -Wall -Wextra -Wpedantic -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef)

if (COVERAGE)
        MESSAGE(STATUS "Link Coverage")
        target_compile_options(tests PRIVATE --coverage)
        target_link_libraries(tests --coverage)
endif ()

# --------------------------------------------------- #
#                   build main
# --------------------------------------------------- #
file(GLOB MAIN "src-view/*.cpp")                                   # all test files
add_executable(main ${LIB_SOURCES} ${MAIN})
find_package(Threads)
target_compile_options(main PRIVATE -std=c++11 -O3)
