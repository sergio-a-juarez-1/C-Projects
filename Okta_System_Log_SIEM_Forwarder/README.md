# Okta Security Log SIEM Forwarder (C-Based)

A lightweight, ultra-high-performance security log forwarder written in pure C. This tool interfaces directly with the cloud-native **Okta System Log API**, pulling real-time authentication and administration telemetry down into memory, parsing it natively, and standardizing it into an actionable alert stream for SIEM ingestion.

Unlike traditional Python or Java-based log collectors, this implementation provides a near-zero memory footprint and minimal CPU overhead, making it ideal for containerized sidecars, embedded thin clients, or high-throughput enterprise monitoring systems.

## ✨ Features
* **Zero-Dependency Runtime Execution:** Uses highly optimized, close-to-metal processing.
* **Heap-Safe Stream Reallocation:** Employs dynamic callback buffers (`libcurl`) engineered specifically to protect against classic heap vulnerabilities.
* **Native JSON Compilation:** Utilizes `cJSON` to recursively parse structured cloud events directly in memory.
* **SIEM-Ready Output:** Normalizes raw multi-nested payloads into single-line alerts compatible with Splunk, ElasticSearch, Logstash, or standard Linux `syslog`.

## 🏗️ Architecture & Data Flow
1. **Request Authorization:** The application creates custom memory-safe HTTP headers containing your Okta API Token (`SSWS`).
2. **Dynamic Data Fetch:** `libcurl` initiates a secure TLS connection to the Okta log endpoint (`/api/v1/logs`) and streams down payloads sequentially.
3. **In-Memory Parsing:** The raw string is converted into a native JSON object array where security attributes (`published`, `eventType`, `outcome.result`, `displayMessage`) are isolated.
4. **Output Generation:** The extracted attributes are cleanly structured to `stdout` or redirected to system pipes.

## 🛠️ Prerequisites & Installation

### 1. Install System Development Libraries
Before compiling, you must install the development headers for both `curl` and `cJSON` via your Linux package manager.

**Debian/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libcjson-dev build-essential
```

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install libcurl-devel cJSON-devel gcc
```

### 2. Configure Your Environment Variables
Open the source code file (`okta_forwarder.c`) and update the primary setup variables in the `main` block with your testing sandbox parameters:

```c
const char *okta_domain = "https://okta.com"; 
const char *api_token   = "YOUR_OKTA_API_TOKEN_HERE";
```

## 🚀 Compilation & Usage

Compile the source tree using `gcc`. You must explicitly pass the `-lcurl` and `-lcjson` linker flags to pair the shared system objects:

```bash
gcc okta_forwarder.c -o okta_forwarder -lcurl -lcjson
```

### Running the Application

Execute the compiled binary directly:
```bash
./okta_forwarder
```

### Piping Logs to a SIEM / Local File
To simulate a continuous SIEM collector on a local system, you can use standard bash pipes to route the application output directly into a dedicated infrastructure file:

```bash
./okta_forwarder >> /var/log/okta_siem_forwarder.log
```

## 🔍 Sample Output

```text
[*] Polling security events from Okta API...
[INFO] Successfully fetched 2 security events.
[2026-09-23T14:02:11.000Z] ALERT - Event: user.authentication.auth_via_mfa.fail | Status: FAILURE | Msg: User failed Okta Verify Push MFA challenge
[2026-09-23T14:05:43.000Z] ALERT - Event: user.account.privilege.grant | Status: SUCCESS | Msg: Granted 'Super Admin' role to user security_audit@company.com
```

## 🔒 Security Best Practices
* **Secret Management:** Never commit your production Okta API Token directly to this repository. It is highly recommended to refactor this script to ingest the token securely via environment variables (`getenv("OKTA_API_TOKEN")`) rather than static string literals.
* **Least Privilege:** The API token generated inside your Okta Administrative console should be configured strictly with a read-only role (`Report Administrator` or similar logging-restricted permission).
