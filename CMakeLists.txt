cmake_minimum_required(VERSION 3.29)
project(UTP_P1)

set(CMAKE_CXX_STANDARD 20)
#set(JAVA_HOME "/Library/Java/JavaVirtualMachines/jdk-23.jdk/Contents/Home")

add_library(UTP_P1 SHARED Main.cpp
        Main.cpp
)

target_include_directories(UTP_P1 PRIVATE "$ENV{JAVA_HOME}/include")
target_include_directories(UTP_P1 PRIVATE "$ENV{JAVA_HOME}/include/darwin")


