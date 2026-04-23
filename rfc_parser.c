#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rfc_parser.h"

// This function acts as a detective. It reads the raw text and extracts rules.
ApiRequest parse_rfc9213_header(int id, const char* header_string) {
    ApiRequest req;
    req.request_id = id;
    
    // RFC 9213 standard states that if urgency is not found, default is 3
    req.urgency = 3; 
    // Default incremental status is false
    req.incremental = false; 

    // 1. Copy the raw string safely to our struct's identity card
    strncpy(req.raw_header, header_string, sizeof(req.raw_header) - 1);
    req.raw_header[sizeof(req.raw_header) - 1] = '\0'; // Ensure string ends properly

    // 2. Search for "u=" (Urgency / Priority level)
    // strstr function searches for a specific word inside a text
    char* u_ptr = strstr(header_string, "u=");
    if (u_ptr != NULL) {
        // We found "u=". Now, convert the character right after it to an integer (atoi)
        int parsed_u = atoi(u_ptr + 2);
        
        // Security check: Urgency must be between 0 and 7
        if (parsed_u >= 0 && parsed_u <= 7) {
            req.urgency = parsed_u;
        }
    }

    // 3. Search for "i" (Incremental / Can be processed in parts)
    // To avoid matching the 'i' in the word "Priority", we only search after the colon ":"
    char* colon_ptr = strchr(header_string, ':');
    if (colon_ptr != NULL) {
        if (strstr(colon_ptr, "i") != NULL) {
            req.incremental = true;
        }
    }

    // Return the fully filled identity card
    return req;
}
