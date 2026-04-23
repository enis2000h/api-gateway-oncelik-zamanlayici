#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"

// Helper function: Swaps two requests in the array
void swap(ApiRequest* a, ApiRequest* b) {
    ApiRequest temp = *a;
    *a = *b;
    *b = temp;
}

// Memory allocation for our priority queue
PriorityQueue* create_queue(int initial_capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = initial_capacity;
    pq->size = 0;
    pq->heap_array = (ApiRequest*)malloc(sizeof(ApiRequest) * initial_capacity);
    return pq;
}

// HEAPIFY UP: When a new patient arrives, move them up if they are more urgent
void heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // In Min-Heap, lower urgency value means HIGHER priority (RFC 9213: u=0 is top)
        if (pq->heap_array[index].urgency < pq->heap_array[parent].urgency) {
            swap(&pq->heap_array[index], &pq->heap_array[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// INSERT: Adds a new request and organizes the pyramid
void insert_request(PriorityQueue* pq, ApiRequest req) {
    if (pq->size == pq->capacity) {
        // Dynamic memory: If full, double the size
        pq->capacity *= 2;
        pq->heap_array = (ApiRequest*)realloc(pq->heap_array, sizeof(ApiRequest) * pq->capacity);
    }
    pq->heap_array[pq->size] = req;
    heapify_up(pq, pq->size);
    pq->size++;
}

// HEAPIFY DOWN: After the top patient leaves, re-organize the pyramid
void heapify_down(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->heap_array[left].urgency < pq->heap_array[smallest].urgency)
        smallest = left;
    
    if (right < pq->size && pq->heap_array[right].urgency < pq->heap_array[smallest].urgency)
        smallest = right;

    if (smallest != index) {
        swap(&pq->heap_array[index], &pq->heap_array[smallest]);
        heapify_down(pq, smallest);
    }
}

// EXTRACT: Pulls the most urgent request from the top (root)
ApiRequest extract_min(PriorityQueue* pq) {
    if (pq->size <= 0) {
        ApiRequest empty = {-1, 9, false, "EMPTY"};
        return empty;
    }
    
    ApiRequest root = pq->heap_array[0];
    pq->heap_array[0] = pq->heap_array[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    
    return root;
}

void free_queue(PriorityQueue* pq) {
    free(pq->heap_array);
    free(pq);
}
