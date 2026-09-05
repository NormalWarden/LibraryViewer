# LibraryViewer

**LibraryViewer** is a lightweight C++ desktop application designed to search for books and manage local user accounts directly on your PC. The app integrates with the Open Library REST API to fetch real-time book metadata.

## Features
* **Account Management:** Create, select, and delete local user profiles.
* **Book Search:** Query books by title, author, year, and language using advanced filters.
* **Favorites & History:** Add books to a personal favorites list, remove them, and keep track of recently viewed books.

### Tech Stack
* **Language:** C++17
* **Networking:** [CPR](https://github.com/libcpr/cpr) (C++ Requests) for elegant REST API interaction.
* **Data Parsing:** [nlohmann/json](https://github.com/nlohmann/json) for local database storage and API response processing.
* **Package Management & Build System:** vcpkg and CMake (with Presets).
* **Testing:** Google Test (GTest) for integration and unit testing.

## Documentation
* [Building Guide](docs/BUILD.md) — System requirements, compilation steps, and how to run the app/tests.
* [Architecture Overview](docs/ARCHITECTURE.md) — Detailed breakdown of code structure, namespaces, and `.inl` files.
* [Search API Dump](docs/API_DUMP.md) — Technical details about Open Library requests and sample JSON payloads.