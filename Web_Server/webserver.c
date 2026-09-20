#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BIND_INTERFACE "0.0.0.0"
#define BUFFER_SIZE 4096 // Bumped to 4KB for better network and disk I/O alignment

typedef struct {
    char action[8];
    char path[128];
} HttpRequestCtx;

typedef struct {
    char name[64];
    char *payload;
    off_t length;
} FilePayload;

// Thread context package
typedef struct {
    int client_fd;
} ThreadArgs;

int init_server_socket(int port) 
{
    struct sockaddr_in host_addr;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    host_addr.sin_addr.s_addr = inet_addr(BIND_INTERFACE);
    host_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) < 0) {
        perror("Binding operation failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 128) < 0) { // Increased backlog queue size for concurrency
        perror("Listen sequence failed");
        close(server_fd);
        return -1;
    }
    return server_fd;
}

// Stack allocated fields replace the heap-churn model
int extract_http_fields(char *raw_request, HttpRequestCtx *req) 
{
    char *first_space = strchr(raw_request, ' ');
    if (!first_space) return 0;
    
    *first_space = '\0';
    strncpy(req->action, raw_request, sizeof(req->action) - 1);
    req->action[sizeof(req->action) - 1] = '\0';

    char *url_start = first_space + 1;
    char *second_space = strchr(url_start, ' ');
    if (!second_space) return 0;

    *second_space = '\0';
    strncpy(req->path, url_start, sizeof(req->path) - 1);
    req->path[sizeof(req->path) - 1] = '\0';

    return 1;
}

void transmit_http_headers(int client_fd, int status_code) 
{
    char header_buffer[512];
    int len = snprintf(header_buffer, sizeof(header_buffer),
             "HTTP/1.0 %d OK\r\n"
             "Server: NativeCustomServer/3.0-Threaded\r\n"
             "Cache-Control: no-store, no-cache, must-revalidate, private\r\n"
             "Content-Language: en\r\n"
             "X-Frame-Options: SAMEORIGIN\r\n",
             status_code);
    write(client_fd, header_buffer, len);
}

void transmit_http_envelope(int client_fd, const char *mime_type, const char *body) 
{
    char meta_buffer[BUFFER_SIZE];
    int body_len = strlen(body);
    int total_len = snprintf(meta_buffer, sizeof(meta_buffer),
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "\r\n%s\r\n",
             mime_type, body_len, body);

    write(client_fd, meta_buffer, total_len);
}

// HIGH EFFICIENCY: Uses 'stat' to read the file in one solid layout step
FilePayload *load_target_file(const char *target_path) 
{
    struct stat st;
    if (stat(target_path, &st) < 0) return NULL;

    int local_fd = open(target_path, O_RDONLY);
    if (local_fd < 0) return NULL;

    FilePayload *file_ctx = malloc(sizeof(FilePayload));
    if (!file_ctx) {
        close(local_fd);
        return NULL;
    }

    strncpy(file_ctx->name, target_path, sizeof(file_ctx->name) - 1);
    file_ctx->name[sizeof(file_ctx->name) - 1] = '\0';
    file_ctx->length = st.st_size;
    
    file_ctx->payload = malloc(file_ctx->length);
    if (!file_ctx->payload) {
        close(local_fd);
        free(file_ctx);
        return NULL;
    }

    // Read the complete asset directly into the exact buffer allocation size
    ssize_t bytes_read = read(local_fd, file_ctx->payload, file_ctx->length);
    close(local_fd);

    if (bytes_read != file_ctx->length) {
        free(file_ctx->payload);
        free(file_ctx);
        return NULL;
    }
    return file_ctx;
}

int dispatch_file_contents(int client_fd, const char *mime_type, FilePayload *file_ctx) 
{
    char meta_buffer[256];
    int meta_len = snprintf(meta_buffer, sizeof(meta_buffer),
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n\r\n",
             mime_type, file_ctx->length);

    if (write(client_fd, meta_buffer, meta_len) < 0) return 0;

    off_t bytes_remaining = file_ctx->length;
    char *cursor = file_ctx->payload;

    while (bytes_remaining > 0) {
        size_t chunk_size = (bytes_remaining < BUFFER_SIZE) ? bytes_remaining : BUFFER_SIZE;
        ssize_t written = write(client_fd, cursor, chunk_size);
        if (written < 1) return 0;

        bytes_remaining -= written;
        cursor += written;
    }
    return 1;
}

void *handle_client_session(void *arg) 
{
    ThreadArgs *t_args = (ThreadArgs *)arg;
    int client_fd = t_args->client_fd;
    free(t_args); // Free arguments wrap block immediately

    pthread_detach(pthread_self()); // Automatically reclaim system thread resources upon exit

    char client_buffer[BUFFER_SIZE];
    memset(client_buffer, 0, BUFFER_SIZE);

    if (recv(client_fd, client_buffer, sizeof(client_buffer) - 1, 0) < 0) {
        close(client_fd);
        return NULL;
    }

    HttpRequestCtx http_req;
    if (!extract_http_fields(client_buffer, &http_req)) {
        close(client_fd);
        return NULL;
    }

    if (strcmp(http_req.action, "GET") == 0 && strncmp(http_req.path, "/img/", 5) == 0) {
        // Robust boundary checking across the complete payload path space
        if (strstr(http_req.path, "..")) {
            transmit_http_headers(client_fd, 403); 
            transmit_http_envelope(client_fd, "text/plain", "Forbidden Access");
        } else {
            char safe_path[256]; 
            snprintf(safe_path, sizeof(safe_path), ".%s", http_req.path);
            
            FilePayload *disk_file = load_target_file(safe_path);
            if (!disk_file) {
                transmit_http_headers(client_fd, 404);
                transmit_http_envelope(client_fd, "text/plain", "Resource not found");
            } else {
                transmit_http_headers(client_fd, 200);
                dispatch_file_contents(client_fd, "image/png", disk_file);
                
                // Safe sequential teardown block avoids leak conditions on dropouts
                free(disk_file->payload);
                free(disk_file);
            }
        }
    } 
    else if (strcmp(http_req.action, "GET") == 0 && strcmp(http_req.path, "/app/webpage") == 0) {
        transmit_http_headers(client_fd, 200);
        transmit_http_envelope(client_fd, "text/html", "<html><body><img src='/img/test.png' alt='Dynamic Element' /></body></html>");
    } 
    else {
        transmit_http_headers(client_fd, 404);
        transmit_http_envelope(client_fd, "text/plain", "Endpoint path does not match");
    }

    close(client_fd);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    int port = 8080;
    if (argc > 1) port = atoi(argv[1]);

    int server_fd = init_server_socket(port);
    if (server_fd < 0) return 1;

    printf("[INFO] Multi-Threaded Server Operational on port: %d\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_fd >= 0) {
            ThreadArgs *args = malloc(sizeof(ThreadArgs));
            if (args) {
                args->client_fd = client_fd;
                pthread_t thread_id;
                // Dispatch work instance out to asynchronous engine thread workers
                if (pthread_create(&thread_id, NULL, handle_client_session, args) != 0) {
                    perror("Failed to create worker thread");
                    close(client_fd);
                    free(args);
                }
            } else {
                close(client_fd);
            }
        }
    }

    close(server_fd);
    return 0;
}

