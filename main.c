#include <stdio.h>
#include <stdlib.h>
#include "rfc_parser.h"
#include "min_heap.h"

int main() {
    printf("--- API Gateway Priority Scheduler Started ---\n\n");

    // 1. Initialize the Priority Queue (Waiting Room)
    // We start with a capacity of 2 to test our dynamic realloc (capacity doubling) feature
    PriorityQueue* pq = create_queue(2); 

    // 2. Simulate incoming HTTP requests (Patients arriving at the hospital)
    const char* req1_str = "Priority: u=5, i"; // Moderate priority, incremental
    const char* req2_str = "Priority: u=0";    // Highest priority (CRITICAL)
    const char* req3_str = "Priority: u=7, i"; // Lowest priority
    const char* req4_str = "Priority: u=2";    // High priority
    const char* req5_str = "Priority: i";      // Missing urgency, should default to 3

    printf("[System] Incoming Requests to API Gateway:\n");
    
    // Parse and Insert Request 1
    ApiRequest req1 = parse_rfc9213_header(101, req1_str);
    printf(" -> Req 101: %s (Parsed Urgency: %d)\n", req1.raw_header, req1.urgency);
    insert_request(pq, req1);

    // Parse and Insert Request 2
    ApiRequest req2 = parse_rfc9213_header(102, req2_str);
    printf(" -> Req 102: %s (Parsed Urgency: %d) [CRITICAL]\n", req2.raw_header, req2.urgency);
    insert_request(pq, req2);

    // Parse and Insert Request 3
    ApiRequest req3 = parse_rfc9213_header(103, req3_str);
    printf(" -> Req 103: %s (Parsed Urgency: %d)\n", req3.raw_header, req3.urgency);
    insert_request(pq, req3);

    // Parse and Insert Request 4
    ApiRequest req4 = parse_rfc9213_header(104, req4_str);
    printf(" -> Req 104: %s (Parsed Urgency: %d)\n", req4.raw_header, req4.urgency);
    insert_request(pq, req4);

    // Parse and Insert Request 5
    ApiRequest req5 = parse_rfc9213_header(105, req5_str);
    printf(" -> Req 105: %s (Parsed Urgency: %d) [DEFAULTED]\n", req5.raw_header, req5.urgency);
    insert_request(pq, req5);

    printf("\n[System] All requests inserted into the Min-Heap.\n");
    printf("[System] Current Queue Size: %d\n", pq->size);
    printf("----------------------------------------------\n");
    printf("[System] Processing Requests (Extracting Min)...\n\n");

    // 3. Process requests (Doctor calling patients from the top of the pyramid)
    // They MUST come out in order of urgency: 0, 2, 3, 5, 7
    while (pq->size > 0) {
        ApiRequest processed_req = extract_min(pq);
        printf(" [PROCESSED] Request ID: %d | Urgency: %d | Incremental: %s\n", 
               processed_req.request_id, 
               processed_req.urgency, 
               processed_req.incremental ? "Yes" : "No");
    }

    // 4. Memory Management (Clean up the hospital to prevent memory leaks)
    free_queue(pq);
    printf("\n[System] Memory successfully freed. Shutting down...\n");

    return 0;
}
