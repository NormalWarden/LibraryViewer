cmake_minimum_required (VERSION 3.25)

project(LibraryViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (LibraryViewer apps/LibraryViewer.cpp src/engine.cpp src/book.cpp)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(LibraryViewer PRIVATE cpr::cpr)

target_include_directories(LibraryViewer PRIVATE include)