#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "rfc_parser.h"

// The structure that represents our "Smart Waiting Room"
typedef struct {
    ApiRequest* heap_array; // Dynamic array to store requests
    int capacity;           // Maximum number of requests allowed
    int size;               // Current number of requests in the heap
} PriorityQueue;

// Function prototypes for Heap operations
PriorityQueue* create_queue(int initial_capacity);
void insert_request(PriorityQueue* pq, ApiRequest req);
ApiRequest extract_min(PriorityQueue* pq);
void free_queue(PriorityQueue* pq);

#endif // MIN_HEAP_H
