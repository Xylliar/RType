
# **R-type**

Third year project part of CPP module at EPITECH
## **Table of Contents**
- [About](#about)
- [Documentation](#documentation)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [License](#license)
- [Authors](#authors)

---

## **About**

- **Purpose**: Create an ECS and a server to make a video game in cpp
- **Goal**: Recreate R-Type with more features such as multiplayer

This project is a cross-platform R-Type game engine designed to provide a classic arcade experience with modern enhancements.

---

## **Documentation**

- [Network protocol](docs/PROTOCOL.md): binary UDP protocol between the client and the server, detailed enough to write a new client.

---

## **Features**

- Multiplayer support
- Modular architecture
- Compatibility with Windows / Unix Systems
- Easy setup with cmake

---

## **Requirements**

List of dependencies and supported platforms:
- **Operating Systems**: Windows 10/11, Ubuntu 20.04+, macOS 11+
- **Build Tools**: CMake 3.22+, Git, G++/Clang or Visual Studio 17 2022
- **Languages**: C++17
- **Libraries**: SFML 2.6.2, FreeType 2.13.2 and Asio 1.18.1, downloaded and built automatically at configure time by the [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) package manager

---

## **Installation**

Step-by-step instructions for setting up the project locally.

### Prerequisites
1. Install [CMake](https://cmake.org/).
2. Clone the repository:
   ```bash
   git clone https://github.com/Xylliar/RType
   ```
3. Navigate to the repository folder
   ```
   cd RType/
   ```

### Setup
1. Create build folder and navigate to it:
   ```bash
   mkdir build && cd build/
   ```

2. Install all tools needed for the project:
   - **Linux (Debian/Ubuntu)**: SFML is built from source, but it still needs the system's windowing, OpenGL and audio libraries:
     ```bash
     sudo apt install build-essential cmake git pkg-config \
       libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
       libgl1-mesa-dev libegl1-mesa-dev libopenal-dev libflac-dev libvorbis-dev
     ```
   - **macOS**: `xcode-select --install` and `brew install cmake`
   - **Windows**: [Visual Studio 2022](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload, CMake and Git

##### Windows:
3. Generate build files:
   ```bash
   cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release
   ```
##### Linux / macOS:
3. Generate build files:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

The first configuration downloads the dependencies and takes a few minutes. Set the `CPM_SOURCE_CACHE` environment variable to a folder to reuse those downloads across build directories.

---

## **Build Instructions**

Detailed instructions for building the project.

### Windows
1. Install [Visual Studio](https://visualstudio.microsoft.com/).
2. Install OpenAL if necessary
3. Use MSBuild to compile:
   ```bash
   cmake --build . --config Release
   cd ../
   ```

### Linux/MacOS
1. Install GCC/Clang and required libraries.
2. Compile using:
   ```bash
   cmake --build . --config Release
   cd ../
   ```

The `r-type_server` and `r-type_client` binaries are written to the repository root.

---

## **Usage**

Instructions on how to run or use the project. Run both programs from the repository root (they load `assets/` and `config/` from the working directory).

**Quick local game**: start the server in one terminal, then one client per player (up to 4 per room) in other terminals:
```bash
./r-type_server -p 4242
./r-type_client -p 4242 -u Player1
```
In the client, create a room (or join one), pick a level and start the game.
Move with the **arrow keys** and shoot with **Space** (keys can be rebound in Settings).
Stop the server with **Ctrl+C** (or type `exit`). If a client crashes or loses its connection, the other players are notified within a few seconds.

### Linux / macOS:
#### Server:
```bash
./r-type_server -p <port>
```
##### Options:

- **`-p <port>`**
  Specify the port number to listen on.

#### Client:
```bash
./r-type_client -p <port> [-h <ip>] [-u <username>]
```
##### Options:
- **`-p <port>`**
  Specify the port number to connect to (required).

- **`-u <username>`**
  Specify the username, 1 to 12 characters (optional). Defaults to `RTyper` if not provided.

- **`-h <ip>`**
  Specify the server IP address (optional). Defaults to `127.0.0.1` if not provided.

### Windows:
#### Server:
```bash
.\r-type_server.exe -p <port>
```
##### Options:

- **`-p <port>`**
  Specify the port number to listen on.

#### Client:
```bash
.\r-type_client.exe -p <port> [-h <ip>] [-u <username>]
```
##### Options:
- **`-p <port>`**
  Specify the port number to connect to (required).

- **`-u <username>`**
  Specify the username, 1 to 12 characters (optional). Defaults to `RTyper` if not provided.

- **`-h <ip>`**
  Specify the server IP address (optional). Defaults to `127.0.0.1` if not provided.

---

## **License**

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for the full license text.


---

## **Authors**

- **Adrien JACQ** - *ECS / Cross platform build*
- **Alexandre MOLINA** - *Network*
- **Amitoj NAYAR** - *Scrum Master / Network*
- **Jules REYNAUD** - *Product Owner / Game design*
- **Tom FACON** - *ECS*
