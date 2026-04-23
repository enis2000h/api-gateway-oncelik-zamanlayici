#ifndef RFC_PARSER_H
#define RFC_PARSER_H

#include <stdbool.h>

// Represents a single API request parsed from HTTP Headers
// urgency: 0 (Highest) to 7 (Lowest)
// incremental: true (1) or false (0)
typedef struct {
    int request_id;
    int urgency;
    bool incremental;
    char raw_header[256]; 
} ApiRequest;

// Function prototype to parse the raw header string
// Example raw_header: "Priority: u=1, i"
ApiRequest parse_rfc9213_header(int id, const char* header_string);

#endif // RFC_PARSER_H
