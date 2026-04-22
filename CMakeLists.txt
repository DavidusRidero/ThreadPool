cmake_minimum_required(VERSION 4.0)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ThreadPool main.cpp
        Chapters/Ch1/Ch1.cpp
        Chapters/Ch2/Ch2a.cpp
        Chapters/Ch2/Ch2d.cpp)
