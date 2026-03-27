#include "queue.h"
#include <limits.h>

PriorityQueue* queueCreate(unsigned capacity)
{
    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    pq->nodes = malloc(sizeof(Node) * (capacity + 1));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void addInQueue(PriorityQueue* pq, unsigned city, unsigned dist, int owner)
{
    unsigned i = ++pq->size;
    while (i > 1 && pq->nodes[i / 2].dist > dist) {
        pq->nodes[i] = pq->nodes[i / 2];
        i = i / 2;
    }
    pq->nodes[i].city = city;
    pq->nodes[i].dist = dist;
    pq->nodes[i].owner = owner;
}

Node deleteElement(PriorityQueue* pq)
{
    Node min = pq->nodes[1];
    Node last = pq->nodes[pq->size--];
    unsigned i = 1;
    unsigned child = 0;
    while (i * 2 <= pq->size) {
        child = i * 2;
        if (child != pq->size && pq->nodes[child + 1].dist < pq->nodes[child].dist) {
            child++;
        }
        if (last.dist > pq->nodes[child].dist) {
            pq->nodes[i] = pq->nodes[child];
        } else {
            break;
        }
        i = child;
    }
    pq->nodes[i] = last;
    return min;
}

void queueFree(PriorityQueue* pq)
{
    free(pq->nodes);
    free(pq);
}