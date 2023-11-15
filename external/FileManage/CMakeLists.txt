cmake_minimum_required(VERSION 3.5)
project(FileManage VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED FALSE)

file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDES "include/*.h")

if(BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${INCLUDES})
    message(STATUS "Building Shared Libraries")
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES} ${INCLUDES})
    message(STATUS "Building Static Libraries")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(tests)