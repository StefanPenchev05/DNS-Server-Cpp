# 🌐 DNS-Server-Cpp

A lightweight, educational DNS (Domain Name System) server implementation in C++. This project is designed to help you understand the core concepts of DNS by building a basic server that can handle DNS queries and generate appropriate responses.

## 📋 Table of Contents

- [🚀 Getting Started](#-getting-started)
- [🛠️ Building the Project](#️-building-the-project)
- [▶️ Running the Server](#️-running-the-server)
- [🐞 Debugging](#-debugging)
- [📁 Project Structure](#-project-structure)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

## 🚀 Getting Started

To get started with this project, ensure you have the following tools installed:

- [CMake](https://cmake.org/download/) for build management.
- A C++ compiler that supports C++23.
- [vcpkg](https://github.com/microsoft/vcpkg) for managing dependencies.

## 🛠️ Building the Project

Follow these steps to build the project locally:

1. **Clone the repository:**

    ```bash
    git clone https://github.com/yourusername/DNS-Server-Cpp.git
    cd DNS-Server-Cpp
    ```

2. **Install dependencies using vcpkg:**

    ```bash
    ./vcpkg/bootstrap-vcpkg.sh
    ./vcpkg/vcpkg install
    ```

3. **Compile the project:**

    ```bash
    ./your_program.sh
    ```

## ▶️ Running the Server

To run the DNS server locally, execute:

```bash
./build/server
```

Upon successful execution, you should see output indicating that the server is running:
    Logs from your program will appear here!

You can test the DNS server using tools like dig or nslookup. For example, to query the server for an A record (IPv4 address) for a domain, use:
```bash
dig @127.0.0.1 -p 2053 example.com A
```
## 🐞 Debugging

To debug the DNS server, you can use debugging tools such as `gdb` or any IDE with C++ debugging support. To ensure that the executable contains debug symbols, you should configure your CMake project for debugging:

1. **Configure the build for debugging**:

    ```bash
    cmake -DCMAKE_BUILD_TYPE=Debug .
    make
    ```

2. **Run the server with gdb**:

    ```bash
    gdb ./build/server
    ```

3. **Set breakpoints and run**:

    In `gdb`, you can set breakpoints in your code and then run the server to analyze its behavior:

    ```gdb
    break main
    run
    ```

This allows you to inspect variables, step through code, and identify where things might be going wrong if the server is not behaving as expected.

## 📁 Project Structure

```plaintext
DNS-Server-Cpp/
├── .codecrafters/
│   ├── compile.sh
│   ├── run.sh
├── .gitattributes
├── .gitignore
├── build/
│   ├── cmake_install.cmake
│   ├── CMakeCache.txt
│   ├── CMakeFiles/
│   ├── Makefile
│   ├── server
│   └── vcpkg_installed/
├── CMakeLists.txt
├── codecrafters.yml
├── README.md
├── src/
│   ├── dns_message.cpp
│   ├── include/
│   ├── server.cpp
│   └── udp_server.cpp
├── vcpkg-configuration.json
├── vcpkg.json
└── your_program.sh
```
## Source Code
- **src/**: Contains the source code for the DNS server.
  - **dns_message.cpp**: Handles the parsing and serialization of DNS messages.
  - **udp_server.cpp**: Implements the UDP server logic that handles incoming DNS queries.
  - **server.cpp**: The main entry point for the DNS server.

## Header Files
- **include/**: Directory for header files.

## Build and Scripts
- **.codecrafters/**: Scripts used by Codecrafters to compile and run the program.
  - **compile.sh**: Script to compile the program.
  - **run.sh**: Script to run the program.
- **build/**: Directory where the build artifacts are generated.
  - **CMakeCache.txt**: Stores CMake configuration cache.
  - **Makefile**: Makefile generated by CMake for building the project.
  - **server**: The compiled DNS server binary.

## Configuration Files
- **CMakeLists.txt**: CMake configuration file that defines how the project should be built.
- **codecrafters.yml**: Configuration file for Codecrafters platform.
- **your_program.sh**: Script to compile and run the program locally.

## 🤝 Contributing

Contributions are welcome! If you'd like to improve the project, please fork the repository, make your changes, and submit a pull request. Here’s how you can get started:

1. **Fork the repository**.
2. **Create a new branch**:

    ```bash
    git checkout -b feature-branch
    ```

3. **Make your changes**.
4. **Commit your changes**:

    ```bash
    git commit -m 'Add new feature'
    ```

5. **Push to the branch**:

    ```bash
    git push origin feature-branch
    ```

6. **Open a pull request** on GitHub.

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## 🔧 Prerequisites

Before you begin, ensure you have met the following requirements:

- You have installed [CMake](https://cmake.org/download/).
- You have a C++ compiler that supports C++23.
- You have installed [vcpkg](https://github.com/microsoft/vcpkg) for managing dependencies.

## 📦 Installation

Follow these steps to set up the project locally:

1. **Clone the repository**:

    ```bash
    git clone https://github.com/yourusername/DNS-Server-Cpp.git
    cd DNS-Server-Cpp
    ```

2. **Install dependencies using vcpkg**:

    ```bash
    ./vcpkg/bootstrap-vcpkg.sh
    ./vcpkg/vcpkg install
    ```

3. **Build the project**:

    ```bash
    ./your_program.sh
    ```