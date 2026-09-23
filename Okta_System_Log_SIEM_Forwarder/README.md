# Okta Security Log SIEM Forwarder (Pro Edition)

A lightweight, ultra-high-performance security log forwarder written in pure C. This utility interfaces directly with the cloud-native **Okta System Log API**, pulling real-time, high-severity authentication telemetry down into memory, parsing it natively, and standardizing it into an actionable alert stream for SIEM ingestion.

Unlike traditional Python or Java-based log collectors, this implementation provides a near-zero memory footprint and minimal CPU overhead, making it ideal for containerized sidecars, embedded thin clients, or high-throughput enterprise security monitoring systems.

## ✨ Features
* **Zero-Hardcoded Secrets (`getenv`):** Ingests sensitive API infrastructure parameters directly from process memory workspace to safeguard against source-control exposure.
* **Server-Side Log Filtering:** Optimizes bandwidth and host CPU cycles by requesting *only* targeted authentication failures directly over the wire (`outcome.result eq "FAILURE"`).
* **Heap-Safe Stream Reallocation:** Employs dynamic callback buffers (`libcurl`) engineered explicitly to protect against classic heap vulnerabilities.
* **Native JSON Processing:** Utilizes `cJSON` to recursively parse structured cloud events directly in memory without relying on bulky execution virtual machines.
* **SIEM-Ready Output:** Normalizes raw multi-nested payloads into single-line alerts compatible with Splunk, ElasticSearch, Logstash, or standard Linux `syslog`.

## 🏗️ Architecture & Data Flow
1. **Runtime Context Gathering:** The application reads `OKTA_API_TOKEN` and `OKTA_TENANT_URL` dynamically from the host environment variables.
2. **Targeted Data Fetch:** `libcurl` initiates a secure TLS connection to the Okta log endpoint, passing url-encoded queries for specific identity telemetry (`/api/v1/logs?filter=...`).
3. **In-Memory Parsing:** The raw string is parsed into a native JSON object array where nested identity structures (`published`, `actor.alternateId`, `outcome.reason`) are extracted.
4. **Output Generation:** The program displays a structured column matrix to `stdout` or pipes it straight to dedicated logging daemons.

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
Do not hardcode your API keys inside the code. Instead, export them to your shell workspace or include them inline during application startup:

```bash
export OKTA_API_TOKEN="your-secret-ssws-token"
export OKTA_TENANT_URL="https://your-company-domain.okta.com"
```

## 🚀 Compilation & Usage

Compile the source tree using `gcc`. You must explicitly pass the `-lcurl` and `-lcjson` linker flags to pair the shared system objects:

```bash
gcc okta_forwarder_pro.c -o okta_forwarder_pro -lcurl -lcjson
```

### Running the Application

Execute the compiled binary directly (inline environment declaration pattern):
```bash
OKTA_API_TOKEN="your-secret-token" OKTA_TENANT_URL="https://company.okta.com" ./okta_forwarder_pro
```

### Piping Logs to a SIEM / Local File
To simulate a continuous SIEM collector on a production layout, route the structured standard output directly into an infrastructure file:

```bash
./okta_forwarder_pro >> /var/log/okta_security_alerts.log
```

## 🔍 Sample Output

```text
[*] Scanning Okta logs for high-severity authentication failures...

[ALERT] Found 2 security warning alerts:
Timestamp                 Actor Login                    Reason
===========================================================================
2026-09-23T14:02:11.000Z  malicious_user@attacker.com    User failed Okta Verify Push MFA challenge
2026-09-23T14:05:43.000Z  compromised_admin@company.com  Invalid password match entry attempt

```

## 🔒 Security Best Practices
* **Least Privilege Access:** The API token generated inside your Okta Administrative console should be configured strictly with a read-only role (`Report Administrator` or similar log-viewing restricted profile).
* **Environment Sandboxing:** In microservices or Kubernetes deployments, inject these variables using protected **Kubernetes Secrets** mapped directly as environment entries to the container runtime landscape.
