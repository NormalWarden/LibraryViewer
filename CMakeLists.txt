cmake_minimum_required (VERSION 3.25)

project(LibraryViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (LibraryViewer apps/LibraryViewer.cpp)