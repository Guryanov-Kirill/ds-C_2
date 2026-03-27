#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct Road {
    unsigned number;
    unsigned length;
} Road;

typedef struct VertexList {
    Road* roads;
    unsigned count;
} VertexList;

typedef struct Graph {
    VertexList* vertices;
    unsigned numRoads;
    unsigned numCities;
    unsigned numCapitals;
    unsigned* capitals;
    int* cityOwners;
} Graph;

Graph* graphCreate(void);

Graph* graphRead(const char* filename);

void graphFree(Graph** graph);

void tooManyRoads(Graph* g, VertexList* list, unsigned* bestCity, unsigned* minLen);

void distributionCities(Graph* g);

void printResult(Graph* g);

void dijkstra(Graph* g);