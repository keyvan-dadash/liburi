cmake_minimum_required(VERSION 3.8)
set(This Uri)

set(Headers
    include/Uri/Uri.hpp
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Sources
    src/Uri.cpp
)

add_subdirectory(googletest)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

link_directories(
    /usr/lib
)

target_include_directories(
    ${This} PUBLIC 
    include
)

add_subdirectory(tests)