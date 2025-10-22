cmake_policy(SET CMP0135 NEW)

cmake_minimum_required(VERSION 3.14) # FetchContent is new in version 3.11.

project(RayOn_Tests C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}-COS-LUT src/Tests/CosLUT.cpp
        src/RayOn/CosLUT.h
)