<h1 align="center">Multi thread logger</h1>

<br />
Dedicated logging thread + SPSC queue (best performance)
Worker threads push log messages into a queue

One logger thread pops and writes to file

Benefits:

No blocking in worker threads

No mutex contention

Clean logs

Helgrind sees no races

This is the pattern used in high‑performance servers.

<br />

# Library dependency(s) 

# 🚀 Available Scripts

In the project directory, you can build the Application with CMakeLists

<br />

Use current folder as: ~/mtlogr (Project root folder)
```
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_INSTALL_PREFIX=/usr/local -G "Unix Makefiles" . -B ./build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local -G "Unix Makefiles" . -B ./build
cmake --build build --target Logtest

```

## 🧪 test

```
cmake --build clientcs-build-release --target test

```
Launches the test runner.

<br />

# 🧬 Project structure

This is the structure of the files in the project:

```sh
    │
    ├── mtlog                 # mtlog package
    │   ├── CMakeLists.txt          # CMake fetchs Boost Asio, CSTokenModel and Json
    │   ├── include
    │   │   └── mtlog         # code
    │   │       └── mt_log.h  # header (extern logger)
    │   └── src           
    │       └── mt_log.cpp    # code
    ├── tests                 # Tests
    │   └── *.cpp
    ├── .gitignore
    ├── CMakeLists.txt        # CMake MTLogger project
    ├── INSTALL.txt
    ├── LICENCE.txt
    └── README.md
```
