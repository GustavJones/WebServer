cmake_minimum_required(VERSION 3.5)
project(WebServer)

file(GLOB Server "src/Server/*.cpp")
file(GLOB Client "src/Client/*.cpp")
file(GLOB SHARED "src/Shared/*.cpp")

add_subdirectory(external/GNetworking)

add_library(${PROJECT_NAME}_Shared STATIC ${SHARED})
add_executable(${PROJECT_NAME}_s ${Server})
add_executable(${PROJECT_NAME}_c ${Client})
add_executable(test_app "tests/main.cpp")

target_include_directories(${PROJECT_NAME}_Shared PRIVATE include external/GNetworking/include)
target_include_directories(${PROJECT_NAME}_s PRIVATE include external/GNetworking/include)
target_include_directories(${PROJECT_NAME}_c PRIVATE include external/GNetworking/include)
target_include_directories(test_app PRIVATE include external/GNetworking/include)

target_link_libraries(${PROJECT_NAME}_Shared PRIVATE Sockets)
target_link_libraries(${PROJECT_NAME}_s PRIVATE ${PROJECT_NAME}_Shared)
target_link_libraries(${PROJECT_NAME}_c PRIVATE ${PROJECT_NAME}_Shared)
target_link_libraries(test_app PRIVATE ${PROJECT_NAME}_Shared)

if(UNIX)
    target_link_libraries(${PROJECT_NAME}_s PRIVATE pthread)
    target_link_libraries(${PROJECT_NAME}_c PRIVATE pthread)
endif(UNIX)