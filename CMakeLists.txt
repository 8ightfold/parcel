cmake_minimum_required(VERSION 3.23)
project(cmake_test CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(parcel)
add_executable(cmake_test main.cpp)
target_link_libraries(cmake_test PRIVATE parcel)
