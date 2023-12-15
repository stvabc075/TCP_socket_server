
### Project Overview
This project comprises a network-based quiz application utilizing a client-server architecture. The server (`server.c`) and client (`client.c`) communicate over a network to facilitate a quiz session based on some basic unix and networking prinicples. The project includes a header file (`header.h`) which appears to contain quiz questions and possibly shared definitions or functions.


### Building and Running
#### Prerequisites
- A C compiler, like GCC.
- A Unix-like environment (Linux, MacOS, etc.) for network and thread handling.

#### Compilation
To compile the server and client:
```bash
gcc -o server server.c
gcc -o client client.c
```

#### Running the Application
1. Start the server:
   ```bash
   ./server <IP address of server> <port number>
   ```
2. In a separate terminal, start the client:
   ```bash
   ./client <IP address of server> <port number>
   ```

### Features
- Network-based quiz communication.
- Multiple choice or Y/N questions.
- Color-coded text output for enhanced user experience.

### Notes
- Ensure the server is running before starting the client.
- Network settings (like port numbers) must be consistent on both server and client.
- The application may need modifications to run on non-Unix-like systems.

### Potential Enhancements
- Adding a database for dynamic question management.
- Implementing a GUI for a better user experience.
- Expanding to support multiple clients simultaneously.

