cmake_minimum_required(VERSION 3.23)
project(parcel CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(parcel INTERFACE -Wno-unused-value)
endif()

