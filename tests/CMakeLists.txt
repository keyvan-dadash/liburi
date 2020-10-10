cmake_minimum_required(VERSION 3.8)
set(This UriTests)

set(Sources
    src/UriTests.cpp
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)


target_include_directories(${This} PRIVATE ..)

target_link_libraries(${This} PUBLIC
    gtest
    gtest_main
    Uri
)

add_test(
    NAME ${This}
    COMMAND ${This}
)