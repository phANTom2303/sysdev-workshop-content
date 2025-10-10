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
7. [Building the AskAI CLI Chatbot](#building-the-askai-cli-chatbot)

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

## Building the AskAI CLI Chatbot

Now let's put it all together by building a terminal-based AI chatbot using the Gemini API!

### Project Overview

**What we'll build:**
- A CLI tool that lets you chat with Google's Gemini AI
- Uses libcurl for API requests
- Uses cJSON for JSON parsing
- Stores conversation history
- Manages API keys via config files

### Prerequisites

#### 1. Get a Gemini API Key

1. Visit [Google AI Studio](https://makersuite.google.com/app/apikey)
2. Sign in with your Google account
3. Click "Create API Key"
4. Copy your API key

#### 2. Test the API with curl

```bash
# Replace YOUR_API_KEY with your actual key
curl -X POST \
  "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=YOUR_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{
    "contents": [{
      "parts": [{
        "text": "Explain what an API is in simple terms"
      }]
    }]
  }'
```

### Understanding APIs

**API (Application Programming Interface):**
- A way for programs to talk to each other
- Set of rules for requesting and exchanging data

**REST API:**
- Uses HTTP requests (like web browsers)
- Common operations: GET, POST, PUT, DELETE
- Data usually in JSON format

**Gemini API:**
- Send text prompts via HTTP POST
- Receive AI-generated responses in JSON
- Requires API key for authentication

### Development Roadmap

We'll build the chatbot incrementally:

1. ✅ Make a basic API call with hardcoded query
2. ✅ Introduce cJSON library for JSON handling
3. ✅ Get user input and create JSON request
4. ✅ Parse API response to extract text
5. ✅ Add context for better terminal formatting
6. ✅ Implement conversation history
7. ✅ Create config file system for API keys
8. ✅ Write installation script

### Installing cJSON

**Option 1: Manual installation**
```bash
# Clone the repository
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
make
sudo make install
```

**Option 2: Package manager**
```bash
# Ubuntu/Debian
sudo apt-get install libcjson-dev

# macOS
brew install cjson
```

### Project Structure

```
askai-project/
├── src/
│   └── cJSON.c          # JSON library implementation
├── includes/
│   └── cJSON.h          # JSON library header
├── askai.c              # Main program
├── Makefile             # Build automation
└── install.sh           # Installation script
```

### Basic API Call Example

**`askai.c`** (Simplified version)
```c
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "cJSON.h"

// Callback to capture API response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    strncat((char*)userp, (char*)contents, realsize);
    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;
    char response[10000] = {0};
    
    // Initialize curl
    curl = curl_easy_init();
    if(curl) {
        // Create JSON request
        cJSON *root = cJSON_CreateObject();
        cJSON *contents = cJSON_CreateArray();
        cJSON *content = cJSON_CreateObject();
        cJSON *parts = cJSON_CreateArray();
        cJSON *part = cJSON_CreateObject();
        
        cJSON_AddStringToObject(part, "text", "Hello! Tell me a joke.");
        cJSON_AddItemToArray(parts, part);
        cJSON_AddItemToObject(content, "parts", parts);
        cJSON_AddItemToArray(contents, content);
        cJSON_AddItemToObject(root, "contents", contents);
        
        char *json_string = cJSON_Print(root);
        
        // Set up curl request
        char url[512];
        snprintf(url, sizeof(url), 
            "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=%s",
            "YOUR_API_KEY");
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        
        // Perform request
        res = curl_easy_perform(curl);
        
        if(res == CURLE_OK) {
            printf("Response: %s\n", response);
        }
        
        // Cleanup
        free(json_string);
        cJSON_Delete(root);
        curl_easy_cleanup(curl);
    }
    
    return 0;
}
```

### Makefile for AskAI Project

```makefile
SRC=src
INC=includes

# Define wildcards to avoid typing all files separately
SOURCES=$(wildcard $(SRC)/*.c)
HEADERS=$(wildcard $(INC)/*.h)

# The 'all' target is the default one
# It tells 'make' that the main goal is to build 'askai'
all: askai

# This rule builds the 'askai' executable from the source files
# and links it with the curl library
# It runs if 'askai' doesn't exist, or if source files have changed
askai: askai.c $(SOURCES) $(HEADERS)
	gcc -I$(INC) askai.c $(SOURCES) -o askai -lcurl

# This is a 'clean' rule to remove the compiled program
# You can run it with the command: make clean
clean:
	rm -f askai
```

### Key Features to Implement

#### 1. **User Input**
```c
char user_input[1000];
printf("You: ");
fgets(user_input, sizeof(user_input), stdin);
```

#### 2. **JSON Response Parsing**
```c
cJSON *json_response = cJSON_Parse(response);
cJSON *candidates = cJSON_GetObjectItem(json_response, "candidates");
cJSON *candidate = cJSON_GetArrayItem(candidates, 0);
cJSON *content = cJSON_GetObjectItem(candidate, "content");
cJSON *parts = cJSON_GetObjectItem(content, "parts");
cJSON *part = cJSON_GetArrayItem(parts, 0);
cJSON *text = cJSON_GetObjectItem(part, "text");

printf("AI: %s\n", text->valuestring);
```

#### 3. **Conversation History**
- Store previous messages in an array
- Include history in each API request
- Allows multi-turn conversations

#### 4. **Config File for API Key**
```c
// Save API key to ~/.askai_config
FILE *config = fopen(config_path, "w");
fprintf(config, "API_KEY=%s\n", api_key);
fclose(config);

// Read API key from config
FILE *config = fopen(config_path, "r");
fscanf(config, "API_KEY=%s", api_key);
fclose(config);
```

### Installation Script

**`install.sh`**
```bash
#!/bin/bash

echo "🚀 Installing AskAI CLI Chatbot..."

# Check for dependencies
echo "📦 Checking dependencies..."

if ! command -v gcc &> /dev/null; then
    echo "❌ gcc not found. Please install gcc first."
    exit 1
fi

if ! dpkg -l | grep -q libcurl; then
    echo "📥 Installing libcurl..."
    sudo apt-get install -y libcurl4-openssl-dev
fi

# Build the project
echo "🔨 Building AskAI..."
make clean
make

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

# Install the executable
echo "📦 Installing to /usr/local/bin..."
sudo mv askai /usr/local/bin/

echo "✅ Installation complete!"
echo "Run 'askai' from anywhere to start chatting!"
```

**Make it executable and run:**
```bash
chmod +x install.sh
./install.sh
```

### Usage

```bash
# First time setup
askai
# Prompts for API key, saves to config

# Chat with AI
You: What is the capital of France?
AI: The capital of France is Paris.

You: Tell me more about it.
AI: Paris is known as the "City of Light"...

You: exit
Goodbye!
```

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
✅ **Real-World Project**: Building a complete CLI application  

### Next Steps

🎯 **Extend your AskAI chatbot:**
- Add color output with ANSI codes
- Implement conversation saving/loading
- Add command-line arguments
- Support different AI models

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
