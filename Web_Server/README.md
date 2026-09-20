# Native HTTP Web Server in C

A lightweight, high-performance, terminal-based **multi-threaded HTTP web server** written in pure C. The application handles raw socket connections concurrently, reads incoming streams, parses HTTP endpoints, and safely dispatches static disk assets (like images) or dynamic HTML structures to browser clients with optimized memory tracking.

---

## ✨ Key Features

*   **Zero Framework Dependencies:** Operates directly over standard Unix network boundaries (`sys/socket.h`, `netinet/in.h`) without third-party dependencies.
*   **Asynchronous Multi-Threading:** Leverages POSIX threads (`pthread`) to handle incoming client connections concurrently, ensuring file streams do not block the main loop listener.
*   **Static & Dynamic Asset Routing:** Native path dispatching for serving binary disk payloads (e.g., Image/PNG arrays) alongside standard `text/html` and `text/plain` content envelopes.
*   **Directory Traversal Protection:** Implements built-in path validation checkpoints to reject malicious parent directory queries (`..`), systematically blocking arbitrary file read exploits.
*   **Heap Optimization & Safety:** Drastically limits heap fragmentation by evaluating file properties upfront via system stat queries, cleaning temporary contexts systematically to eliminate leak build-ups.
*   **Rapid Re-binding Architecture:** Configured with `SO_REUSEADDR` parameters to bypass socket locking overhead, enabling immediate environment resets during active development cycles.

---

## 📊 Supported Endpoints

| HTTP Method | Resource Path | Expected Response Behavior |
| :---: | :--- | :--- |
| **GET** | `/app/webpage` | Returns an internal HTML layout referencing page image elements. |
| **GET** | `/img/<file.png>` | Pipes binary raw images from disk over a standard stream wrapper. |
| **GET** | *Any invalid route* | Emits a standard `404 Resource not found` status envelope. |

---

## 🛠️ Build and Local Deployment

To run this custom web utility locally, ensure your computer environment contains a standard C build toolchain.

### 1. Isolate the Project via Sparse-Checkout
To pull this specific tool out of your repository workspace without cluttering your system with your complete monorepo setup, open your terminal and run:

```bash
# Initialize an empty local directory
mkdir Web_Server && cd Web_Server
git init

# Link your multi-project workspace as the remote engine
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and pull the target server directory
git sparse-checkout set Web_Server
git pull origin main
```

### 2. Compilation
Build the production binary with thread mapping configurations and compiler optimization flags:
```bash
gcc -O2 webserver.c -o webserver -pthread
```

### 3. Running the Server
Execute the application binary. By default, the system binds securely to `0.0.0.0:8080`. You may explicitly override the port assignment by passing a numeric flag as a trailing runtime argument:

```bash
# Start server on default port 8080
./webserver

# Start server on custom port 8081
./webserver 8081
```

---

## 🔬 System Testing

Once your engine layer is actively running, you can trace stream operations using local terminal utilities:

### Direct Stream Check via Telnet
```bash
telnet localhost 8081
GET /app/webpage HTTP/1.0
```

### Traversal Attack Protection Validation
Verify that the security layer systematically responds with a `403 Forbidden Access` envelope when attempting unauthorized root breaks:
```bash
telnet localhost 8081
GET /img/../../../../etc/passwd HTTP/1.0
```

---

## 📂 Architecture Breakdown

The server loop manages browser connections by evaluating three foundational states sequentially:
*   `extract_http_fields()`: Extracts HTTP action markers (`GET`) and endpoint paths out of incoming text buffers into a stack-allocated tracking structure to remove heap allocation overhead.
*   `load_target_file()`: Queries the file system directly via `stat()` to determine resource sizes, provisioning a perfect memory footprint via a single `malloc()` call rather than looping allocations.
*   `dispatch_file_contents()`: Evaluates precise file bytes dynamically, piping data fragments over network descriptor blocks using standard network transmission buffers.




