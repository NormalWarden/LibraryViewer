cmake_minimum_required (VERSION 3.25)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	find_program(VCPKG_EXECUTABLE vcpkg)
	if(VCPKG_EXECUTABLE)
		get_filename_component(VCPKG_DIR "${VCPKG_EXECUTABLE}" DIRECTORY)
		set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
	else()
		set(CMAKE_TOOLCHAIN_FILE "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg\scripts\buildsystems\vcpkg.cmake")
	endif()
endif()

project(LibraryViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (LibraryViewer apps/LibraryViewer.cpp src/engine.cpp  src/ui.cpp)
target_include_directories(LibraryViewer PRIVATE include)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(LibraryViewer PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/1b6f64d659944658a4c685b7bd9f04c1c3b8a39b.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/LibraryViewerTests.cpp  src/engine.cpp tests/EngineTest.cpp )
target_link_libraries(tests PRIVATE gtest_main cpr::cpr nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(tests)

target_include_directories(tests PRIVATE include)