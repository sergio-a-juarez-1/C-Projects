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

// Callback function used by libcurl to write chunked API payloads safely
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "[ERROR] Out of memory (realloc failed)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Function to process and parse targeted Okta failure telemetry
void process_security_events(const char *raw_json) {
    cJSON *json_array = cJSON_Parse(raw_json);
    if (json_array == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "[ERROR] JSON Parsing failed before: %s\n", error_ptr);
        }
        return;
    }

    if (cJSON_IsArray(json_array)) {
        int array_size = cJSON_GetArraySize(json_array);
        if (array_size == 0) {
            printf("[INFO] No suspicious authentication failures discovered in this window.\n");
            cJSON_Delete(json_array);
            return;
        }

        printf("\n[ALERT] Found %d security warning alerts:\n", array_size);
        printf("%-25s %-30s %s\n", "Timestamp", "Actor Login", "Reason");
        printf("===========================================================================\n");

        for (int i = 0; i < array_size; i++) {
            cJSON *item = cJSON_GetArrayItem(json_array, i);
            
            cJSON *published = cJSON_GetObjectItemCaseSensitive(item, "published");
            
            // Navigate the nested 'actor' abstraction safely
            char *actor_login = "System/Unknown";
            cJSON *actor = cJSON_GetObjectItemCaseSensitive(item, "actor");
            if (cJSON_IsObject(actor)) {
                cJSON *alt_id = cJSON_GetObjectItemCaseSensitive(actor, "alternateId");
                if (cJSON_IsString(alt_id)) {
                    actor_login = alt_id->valuestring;
                }
            }

            // Navigate the nested 'outcome' abstraction safely
            char *reason = "Unknown Auth Block";
            cJSON *outcome = cJSON_GetObjectItemCaseSensitive(item, "outcome");
            if (cJSON_IsObject(outcome)) {
                cJSON *reason_obj = cJSON_GetObjectItemCaseSensitive(outcome, "reason");
                if (cJSON_IsString(reason_obj)) {
                    reason = reason_obj->valuestring;
                }
            }

            if (cJSON_IsString(published)) {
                // Truncate timestamp string visually for formatted stdout matching the Python layout
                char time_buff[26];
                snprintf(time_buff, sizeof(time_buff), "%s", published->valuestring);
                
                printf("%-25s %-30s %s\n", time_buff, actor_login, reason);
            }
        }
        printf("\n");
    } else {
        printf("[WARN] Response was not a valid log array. Payload: %s\n", raw_json);
    }

    cJSON_Delete(json_array);
}

int main(void) {
    // 1. Fetch secure configuration from OS Environment Variables
    const char *api_token = getenv("OKTA_API_TOKEN");
    const char *tenant_url = getenv("OKTA_TENANT_URL");

    if (!api_token || !tenant_url) {
        fprintf(stderr, "[ERROR] Missing configuration environment parameters.\n");
        fprintf(stderr, "Please export OKTA_API_TOKEN and OKTA_TENANT_URL before running.\n");
        return 1;
    }

    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1); 
    chunk.size = 0;    

    // URL Encode the Okta filter criteria to ensure safe character parsing over the wire
    // Filter matches Python: outcome.result eq "FAILURE" and eventType sw "user.authentication"
    const char *url_encoded_filter = "outcome.result%20eq%20%22FAILURE%22%20and%20eventType%20sw%20%22user.authentication%22";
    
    char url[1024];
    snprintf(url, sizeof(url), "%s/api/v1/logs?filter=%s&limit=100", tenant_url, url_encoded_filter);

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle) {
        struct curl_slist *headers = NULL;
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: SSWS %s", api_token);
        
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "C-Okta-SIEM-Forwarder-Pro/2.0");
        curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10L); // 10 second timeout matching Python

        printf("[*] Scanning Okta logs for high-severity authentication failures...\n");
        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "[Network/API Error] System Log querying failed: %s\n", curl_easy_strerror(res));
        } else {
            process_security_events(chunk.memory);
        }

        curl_easy_cleanup(curl_handle);
        curl_slist_free_all(headers);
    }

    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}
