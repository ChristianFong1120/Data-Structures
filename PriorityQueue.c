//Structure for a priority queue, utilizes the node structure
//NOTE: The node structure in this repository will not work with this queue
//as this implementation requires a node that stores integer values

#include "pq.h"
#include "defines.h"
#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    Node **queue;
    uint32_t capacity;
    uint32_t head;
    uint32_t tail;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->head = 0;
        q->capacity = capacity;
        q->tail = 0;
        q->queue = (Node **) calloc(capacity, sizeof(Node));
        if (!q->queue) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

//Modified swap and compare, original structure from Assignment 3 "stats.h"
int cmp(PriorityQueue *q, int index_i, int index_j) {
    if (q->queue[index_i]->frequency < q->queue[index_j]->frequency) {
        return -1;
    } else if (q->queue[index_i]->frequency > q->queue[index_j]->frequency) {
        return 1;
    } else {
        return 0;
    }
}

void swap(PriorityQueue *q, int index_i, int index_j) {
    Node *temp = q->queue[index_i];
    q->queue[index_i] = q->queue[index_j];
    q->queue[index_j] = temp;
}

//Code built from pseudocode provided by Introduction To Algorithms
//3rd Edition (Cormen, Leiserson, Rivest, Stein)
//Chapter 6.2
void min_heapify(PriorityQueue *q, uint32_t index) {
    // +2 since 0 based indexing
    uint32_t left = 2 * index + 1;
    uint32_t right = left + 1;
    uint32_t min_val = index;
    if (left < q->tail && cmp(q, min_val, left) == 1) {
        min_val = left;
    } else if (right < q->tail && cmp(q, min_val, right) == 1) {
        min_val = right;
    }
    if (min_val != index) {
        swap(q, index, min_val);
        min_heapify(q, min_val);
    }
}

//check if child's frequency is less than its parent's, if so then swap
//start from bottom, and work upwards on heap
//Code built from pseudocode provided by Introduction To Algorithms
//3rd Edition (Cormen, Leiserson, Rivest, Stein)
//Chapter 6.5
void decrease_key(PriorityQueue *q, int curr_index) {
    while (curr_index > 0 && (cmp(q, curr_index, (curr_index - 1) / 2)) == -1) {
        swap(q, curr_index, (curr_index - 1) / 2);
        curr_index = (curr_index - 1) / 2;
    }
}

void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->queue) {
        free((*q)->queue);
        free(*q);
        *q = NULL;
    }
}

bool pq_empty(PriorityQueue *q) {
    if (q->tail == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    if (q->tail == q->capacity) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->tail;
}

//Code built from pseudocode provided by Introduction To Algorithms
//3rd Edition (Cormen, Leiserson, Rivest, Stein)
//Chapter 6.5
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q) || !(q)) {
        return false;
    }
    q->tail++;
    int i = q->tail - 1;
    q->queue[i] = n;
    decrease_key(q, i);
    return true;
}
//Code built from pseudocode provided by Introduction To Algorithms
//3rd Edition (Cormen, Leiserson, Rivest, Stein)
//Chapter 6.5
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q) || !(q)) {
        return false;
    }
    *n = q->queue[0];
    q->tail--;
    q->queue[0] = q->queue[q->tail];
    min_heapify(q, 0);
    return true;
}
