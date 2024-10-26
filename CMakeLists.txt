cmake_minimum_required(VERSION 3.29)
project(UTP_P1)

set(CMAKE_CXX_STANDARD 20)
set(JAVA_HOME "/Users/dsharnevich/Library/Java/JavaVirtualMachines/openjdk-23.0.1/Contents/Home")

add_library(UTP_P1 SHARED Main.cpp
        Main.cpp
        Main.h
)

target_include_directories(UTP_P1 PRIVATE "${JAVA_HOME}/include")
target_include_directories(UTP_P1 PRIVATE "${JAVA_HOME}/include/darwin")


