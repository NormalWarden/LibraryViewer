## System requirements

To build the project, you will need:
* **C++ Standard:** C++17
* **Build System:** CMake 3.25 or higher
* **Package Manager:** vcpkg (commit: e861f04798ca54fa70ff906b5e43e5fc99b7f406)
* **Compiler:** MSVC in Microsoft Visual Studio Community 2022 (version 17.14.11)
* **OS:** Windows 10 (64-bit)

Note: The use of cross-platform libraries (cpr and nlohmann/json) and CMake theoretically allows the project to be built on other operating systems (Linux, macOS) and with other compilers (GCC, Clang). However, these configurations have not been tested by the author.

## Building
```bash
git clone "https://github.com/NormalWarden/LibraryViewer"
cd LibraryViewer
cmake --preset x64-release
cmake --build --preset x64-release
```

## Running
* To run the main application, execute: `out/build/x64-release/Release/LibraryViewer.exe`
* To run the tests, execute: `out/build/x64-release/Release/tests.exe`