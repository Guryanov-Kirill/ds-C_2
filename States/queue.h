#include <stdlib.h>

typedef struct {
    unsigned city;
    unsigned dist;
    int owner;
} Node;

typedef struct {
    Node* nodes;
    unsigned size;
    unsigned capacity;
} PriorityQueue;

PriorityQueue* queueCreate(unsigned capacity);
void addInQueue(PriorityQueue* pq, unsigned city, unsigned dist, int owner);
Node deleteElement(PriorityQueue* pq);
void queueFree(PriorityQueue* pq);