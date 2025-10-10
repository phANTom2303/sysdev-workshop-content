# Building Your Own CLI Tool in C
## A Beginner's Guide to Executables, Build Systems, and Deployment

---

## Table of Contents
1. [Understanding Executables](#understanding-executables)
2. [How Commands Work in Your Terminal](#how-commands-work)
3. [Building Your First CLI Tool](#building-your-first-cli-tool)
4. [Multi-File Projects](#multi-file-projects)
5. [Automating Builds with Makefiles](#automating-builds-with-makefiles)
6. [Working with External Libraries](#working-with-external-libraries)

---

## Understanding Executables

### What is an Executable?
An **executable** is a file or program that your computer can run directly. It contains machine code that your processor understands and can execute.

**Examples:**
- On Linux: `ls`, `cat`, `grep`, `firefox`
- On Windows: `notepad.exe`, `cmd.exe`
- Your compiled C programs!

---

## How Commands Work in Your Terminal

### What Happens When You Type a Command?

When you open an application or type a command in the terminal, your operating system follows these steps:

#### 1. **Parsing the Command**
The terminal (shell) parses your input and separates:
- The command name
- Arguments and options

```bash
ls -la /home/user
# 'ls' = command
# '-la' = options
# '/home/user' = argument
```

#### 2. **Searching the PATH Environment Variable**
For non-built-in commands, the shell searches for an executable file using the **PATH** variable.

**What is PATH?**
- A list of directories where the system looks for executables
- Directories are separated by colons (`:`) on Linux/Mac
- Searched in order from left to right

**View your PATH:**
```bash
echo $PATH
```

**Typical output:**
```
/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
```

#### 3. **Executing the Program**
Once found:
- The OS loads the program into memory
- Creates a new process
- Begins executing instructions
- Passes any arguments to the program

**Example: Running `ls`**
```bash
ls
```
1. Shell checks if `ls` is built-in (it's not, usually)
2. Searches PATH directories
3. Finds `/usr/bin/ls`
4. Executes `/usr/bin/ls` and displays directory contents

---

## Building Your First CLI Tool

### Step 1: Write a Simple Program

Let's create a program that greets the currently logged-in user.

**`hello.c`**
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* user = getenv("USER");
    printf("Hello %s!\n", user);
    return 0;
}
```

### Step 2: Compile the Program

```bash
gcc hello.c -o sayhello
```

**What this does:**
- `gcc` = GNU C Compiler
- `hello.c` = source file
- `-o sayhello` = output file name
- Creates an executable named `sayhello`

### Step 3: Test Your Program

```bash
./sayhello
# Output: Hello anish-goenka!
```

### Step 4: "Install" Your Program

To use `sayhello` from anywhere (without `./`), add it to a PATH directory.

**Option 1: Move to existing PATH directory**
```bash
# Check which directories are in PATH
echo $PATH

# Move your executable (requires sudo for system directories)
sudo mv sayhello /usr/local/bin/

# Now you can run it from anywhere!
sayhello
```

**Option 2: Add a custom directory to PATH**
```bash
# Create a personal bin directory
mkdir -p ~/bin
mv sayhello ~/bin/

# Add to PATH (add this line to ~/.bashrc or ~/.zshrc)
export PATH="$HOME/bin:$PATH"

# Reload your shell configuration
source ~/.bashrc

# Now you can run it from anywhere!
sayhello
```

---

## Multi-File Projects

As projects grow, organizing code across multiple files becomes essential.

### Example: Separating Functions into Modules

Let's refactor our hello program to use separate files.

#### **`hello-with-dependency.c`** (Main Program)
```c
#include <stdio.h>
#include "getuser.h"

int main() {
    char* user = getUser();
    printf("Hello %s!\n", user);
    return 0;
}
```

#### **`getuser.h`** (Header File)
```c
#ifndef GETUSER_H
#define GETUSER_H

char* getUser();

#endif
```

#### **`getuser.c`** (Implementation)
```c
#include <stdlib.h>
#include "getuser.h"

char* getUser() {
    char* user = getenv("USER");
    return user;
}
```

### Understanding Header Files

**Header files (`.h`) serve as interfaces:**
- Declare function prototypes
- Define constants and macros
- Allow other files to know what functions are available

**Header guards (`#ifndef`, `#define`, `#endif`):**
- Prevent multiple inclusions of the same header
- Avoid duplicate definitions

**Include syntax:**
- `#include <stdio.h>` → System headers (in standard locations)
- `#include "getuser.h"` → User headers (in current directory or custom paths)

### Compiling Multi-File Projects

```bash
gcc hello-with-dependency.c getuser.c -o sayhello
```

**What happens:**
1. **Compilation**: Each `.c` file is compiled into object code
2. **Linking**: Object files are combined into a single executable
3. **Static Linking**: All code is bundled together in the executable

**Result:** A standalone executable that can run anywhere (doesn't need separate files)

---

## Automating Builds with Makefiles

As projects grow, typing long compilation commands becomes tedious. **Makefiles** automate this!

### Basic Makefile Syntax

**`Makefile`**
```makefile
all: sayhello

sayhello: hello-with-dependency.c getuser.c getuser.h
	gcc hello-with-dependency.c getuser.c -o sayhello

clean:
	rm -f sayhello
```

**Anatomy of a Makefile Rule:**
```makefile
target: dependencies
	command
```

- **Target**: What you want to build (e.g., `sayhello`, `clean`)
- **Dependencies**: Files needed to build the target
- **Command**: Shell command to execute (MUST be indented with a TAB)

### Using the Makefile

```bash
# Build the project (runs 'all' target by default)
make

# Clean up compiled files
make clean

# Build a specific target
make sayhello
```

### The Magic of Make

**Make is intelligent:**
- Only recompiles files that have changed
- Checks timestamps of dependencies
- Skips unnecessary work

**Try this:**
```bash
make              # Compiles everything
make              # Output: "sayhello is up to date"
# Edit getuser.c
make              # Only recompiles what's needed!
```

### Benefits of Makefiles

✅ **Save time**: No more typing long commands  
✅ **Efficient**: Only rebuilds changed files  
✅ **Organized**: Clear structure for complex projects  
✅ **Portable**: Document your build process  

⚠️ **Note**: Makefiles can be OS-specific (different commands for Linux/Mac/Windows)

---

## Working with External Libraries

Real-world programs often need external libraries for additional functionality.

### Example: Using libcurl

**libcurl** is a library for making HTTP requests.

#### Installation

```bash
# Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# Fedora/RHEL
sudo dnf install libcurl-devel

# macOS
brew install curl
```

#### Simple libcurl Example

**`curl-example.c`**
```c
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    CURLcode res;
    CURL* curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com/");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    return 0;
}
```

#### Compilation with External Libraries

```bash
gcc curl-example.c -o curlExample -lcurl
```

**Understanding the `-lcurl` flag:**
- `-l` = Link with a library
- `curl` = Library name (libcurl)
- This is **dynamic linking**

### Static vs Dynamic Linking

**Static Linking:**
```bash
gcc hello-with-dependency.c getuser.c -o sayhello
```
- Library code is copied into your executable
- Larger executable, but standalone
- Works anywhere (no dependencies needed)

**Dynamic Linking:**
```bash
gcc curl-example.c -o curlExample -lcurl
```
- Executable references external library
- Smaller executable
- **Requires library to be installed** on the system where it runs
- Library is loaded at runtime

### Custom Header File Directories

Use the `-I` flag to specify where to find custom header files:

```bash
gcc -I./includes main.c -o program
```

This allows:
```c
#include "myheader.h"  // Instead of #include "./includes/myheader.h"
```

### Advanced Makefile with Wildcards

For larger projects, use wildcards to avoid listing every file:

```makefile
SRC=src
INC=includes

# Define wildcards to match all files
SOURCES=$(wildcard $(SRC)/*.c)
HEADERS=$(wildcard $(INC)/*.h)

all: myprogram

myprogram: main.c $(SOURCES) $(HEADERS)
	gcc -I$(INC) main.c $(SOURCES) -o myprogram -lcurl

clean:
	rm -f myprogram
```

**What this does:**
- `$(wildcard $(SRC)/*.c)` → Finds all `.c` files in `src/` directory
- `$(wildcard $(INC)/*.h)` → Finds all `.h` files in `includes/` directory
- `-I$(INC)` → Tells compiler to look in `includes/` for headers
- No need to manually list each source file!

---

## Summary

### What You've Learned

✅ **Executables**: Programs your computer can run  
✅ **PATH Variable**: How your OS finds commands  
✅ **Compilation**: Turning source code into executables  
✅ **Multi-File Projects**: Organizing code with headers  
✅ **Makefiles**: Automating build processes  
✅ **Static vs Dynamic Linking**: Different ways to use libraries  
✅ **External Libraries**: Using libcurl and cJSON  
✅ **APIs**: Communicating with web services  

### Next Steps

🎯 **Explore more libraries:**
- SQLite for databases
- GTK for GUI applications
- OpenSSL for encryption

🎯 **Learn more about build systems:**
- CMake (more powerful than Make)
- Autotools
- Meson

### Resources

- [GCC Documentation](https://gcc.gnu.org/onlinedocs/)
- [Make Manual](https://www.gnu.org/software/make/manual/)
- [libcurl Tutorial](https://curl.se/libcurl/c/libcurl-tutorial.html)
- [cJSON GitHub](https://github.com/DaveGamble/cJSON)
- [Gemini API Docs](https://ai.google.dev/docs)

---

**Happy Coding! 🚀**
