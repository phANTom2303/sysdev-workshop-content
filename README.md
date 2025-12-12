# System Development Workshop Content

## Prerequisites

### Update and Upgrade Ubuntu
Before starting, ensure your system is up to date:
```bash
sudo apt update && sudo apt upgrade
```

### Install Compilers
Install the GCC and G++ compilers along with other essential build tools:
```bash
sudo apt install build-essential
```
Or install them individually:
```bash
sudo apt install gcc g++
```

## Project Structure Summary
This repository contains a basic C project setup:

- **`includes/`**: Contains header files (e.g., `utils.h`).
- **`src/`**: Contains source code files (e.g., `main.c`, `utils.c`).
- **`Makefile`**: Configuration file to compile and link the project.

## Resources
For a quick reference on C and C++ development commands and concepts, please refer to the [C & CPP Dev CheatSheet](C%20&%20CPP%20Dev%20CheatSheet.md).
