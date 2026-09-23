# ==========================================
# STAGE 1: Build the binary application
# ==========================================
FROM ubuntu:24.04 AS builder

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and development headers
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    libcurl4-openssl-dev \
    libcjson-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the compilation workspace
WORKDIR /app

# Copy the source code file into the builder image
COPY okta_forwarder_pro.c .

# Compile the binary with static linking considerations where possible
RUN gcc -O2 okta_forwarder_pro.c -o okta_forwarder_pro -lcurl -lcjson

# ==========================================
# STAGE 2: Create the minimal runtime image
# ==========================================
FROM ubuntu:24.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install ONLY runtime shared libraries (no compilers or dev headers)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libcurl4 \
    libcjson1 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create a non-privileged system user for process isolation
RUN useradd -u 10001 -m security_user
USER security_user

WORKDIR /home/security_user

# Copy the compiled binary from the builder stage
COPY --from=builder --chown=security_user:security_user /app/okta_forwarder_pro .

# Ensure the container expects the critical Okta environment variables
ENV OKTA_API_TOKEN=""
ENV OKTA_TENANT_URL=""

# Set the default binary execution command
ENTRYPOINT ["./okta_forwarder_pro"]
