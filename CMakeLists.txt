cmake_minimum_required(VERSION 3.5)
project(Sandbox)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Set up output directories.
set(BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(OBJECT_DIR ${PROJECT_SOURCE_DIR}/obj)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

# CMake modules path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Project common include directories.
include_directories(src)

# Add subdirectories.
add_subdirectory(src/api)
add_subdirectory(src/common)
add_subdirectory(src/configuration)
add_subdirectory(src/application)
add_subdirectory(src/modules/example)
add_subdirectory(src/modules/tracker)

# Add tests.
add_subdirectory(src/tests)

if(MSVC)
	include_directories(AFTER "C:/Program Files (x86)/Visual Leak Detector/include")
	link_libraries("C:/Program Files (x86)/Visual Leak Detector/lib/Win64/vld.lib")
endif()