#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// Struct to handle memory allocations for dynamic API responses
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function used by libcurl to write chunked API payloads
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Error: Not enough memory (realloc failed)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Function to process and parse Okta JSON logs
void process_okta_logs(const char *raw_json) {
    cJSON *json_array = cJSON_Parse(raw_json);
    if (json_array == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error parsing JSON before: %s\n", error_ptr);
        }
        return;
    }

    // Ensure Okta returned an array of events
    if (cJSON_IsArray(json_array)) {
        int array_size = cJSON_GetArraySize(json_array);
        printf("[INFO] Successfully fetched %d security events.\n", array_size);

        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(json_array, i);
            
            // Extract key cybersecurity datapoints from Okta Schema
            cJSON *published = cJSON_GetObjectItemCaseSensitive(item, "published");
            cJSON *event_type = cJSON_GetObjectItemCaseSensitive(item, "eventType");
            cJSON *display_text = cJSON_GetObjectItemCaseSensitive(item, "displayMessage");
            cJSON *outcome = cJSON_GetObjectItemCaseSensitive(item, "outcome");

            char *result = "UNKNOWN";
            if (cJSON_IsObject(outcome)) {
                cJSON *result_obj = cJSON_GetObjectItemCaseSensitive(outcome, "result");
                if (cJSON_IsString(result_obj)) {
                    result = result_obj->valuestring;
                }
            }

            // Simple SIEM/Syslog formatting simulation
            if (cJSON_IsString(published) && cJSON_IsString(event_type)) {
                printf("[%s] ALERT - Event: %s | Status: %s | Msg: %s\n",
                       published->valuestring,
                       event_type->valuestring,
                       result,
                       cJSON_IsString(display_text) ? display_text->valuestring : "N/A");
            }
        }
    } else {
        // Okta occasionally returns an error object instead of an array if the token is invalid
        printf("[WARN] Response was not a valid log array. Payload: %s\n", raw_json);
    }

    cJSON_Delete(json_array);
}

int main(void) {
    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1); 
    chunk.size = 0;    

    // Configuration - Replace placeholders with your sandbox information
    const char *okta_domain = "https://okta.com"; 
    const char *api_token = "YOUR_OKTA_API_TOKEN_HERE";
    
    // FIX: Allocated 512 bytes on the stack to prevent memory corruption
    char url[512];
    snprintf(url, sizeof(url), "%s/api/v1/logs?limit=5", okta_domain);

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle) {
        // Setup HTTP Headers required for Okta API Authorization
        struct curl_slist *headers = NULL;
        
        // FIX: Allocated 512 bytes on the stack for the header authorization string
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: SSWS %s", api_token);
        
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Set Target URL and Headers
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

        // Bind standard callback parameters to handle response streams safely
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "C-Okta-SIEM-Forwarder/1.0");

        printf("[*] Polling security events from Okta API...\n");
        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "API Request Failed: %s\n", curl_easy_strerror(res));
        } else {
            // Process the collected dynamic buffer
            process_okta_logs(chunk.memory);
        }

        // Cleanup Allocations
        curl_easy_cleanup(curl_handle);
        curl_slist_free_all(headers);
    }

    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}
