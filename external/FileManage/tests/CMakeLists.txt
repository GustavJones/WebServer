file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(Source ${TEST_SOURCES})
    string(REGEX REPLACE ".cpp" "" PathAndName ${Source})
    string(REGEX REPLACE ${PROJECT_SOURCE_DIR}/tests/ "" Name ${PathAndName})

    add_executable(${Name} ${Source})
    target_include_directories(${Name} PRIVATE ../include)
    target_link_libraries(${Name} ${PROJECT_NAME})
endforeach()