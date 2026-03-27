#include "graph.h"
#include "queue.c"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

Graph* graphCreate(void)
{
    return calloc(1, sizeof(Graph));
}

static void addRoad(VertexList* list, unsigned j, unsigned len);
static VertexList* reallocList(VertexList* list, unsigned numCities);

Graph* graphRead(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    Graph* graph = graphCreate();
    if (graph == NULL) {
        fclose(file);
        return NULL;
    }
    if ((fscanf(file, "%u", &graph->numCities) != 1) || (fscanf(file, "%u", &graph->numRoads) != 1)) {
        graphFree(&graph);
        fclose(file);
        return NULL;
    }

    graph->cityOwners = malloc((graph->numCities + 1) * sizeof(int));
    for (unsigned i = 0; i <= graph->numCities; i++) {
        graph->cityOwners[i] = -1;
    }

    VertexList* newVertices = reallocList(graph->vertices, graph->numCities + 1);
    if (newVertices == NULL) {
        graphFree(&graph);
        fclose(file);
        return NULL;
    }
    memset(newVertices, 0, (graph->numCities + 1) * sizeof(VertexList));
    graph->vertices = newVertices;

    for (unsigned index = 0; index < graph->numRoads; index++) {
        unsigned i, j, len;
        if (fscanf(file, "%u %u %u", &i, &j, &len) == 3) {
            addRoad(&graph->vertices[i], j, len);
            addRoad(&graph->vertices[j], i, len);
        }
    }

    if (fscanf(file, "%u", &graph->numCapitals) != 1) {
        graphFree(&graph);
        fclose(file);
        return NULL;
    }

    graph->capitals = malloc(graph->numCapitals * sizeof(unsigned));

    for (unsigned index = 0; index < graph->numCapitals; index++) {
        fscanf(file, "%u", &graph->capitals[index]);
        graph->cityOwners[graph->capitals[index]] = index;
    }

    fclose(file);
    return graph;
}

static void addRoad(VertexList* list, unsigned j, unsigned len)
{
    Road* temp = realloc(list->roads, (list->count + 1) * sizeof(Road));
    if (temp != NULL) {
        list->roads = temp;
        list->roads[list->count].number = j;
        list->roads[list->count].length = len;
        list->count++;
    }
}

static VertexList* reallocList(VertexList* list, unsigned numCities)
{
    return realloc(list, numCities * sizeof(list[0]));
}

void RoadFree(VertexList* list)
{
    if (list == NULL) {
        return;
    }
    if (list->roads != NULL) {
        free(list->roads);
        list->roads = NULL;
        list->count = 0;
    }
}

static void listFree(Graph* graph)
{
    if (graph == NULL) {
        return;
    }
    if (graph->vertices != NULL) {
        for (unsigned i = 0; i <= graph->numCities; i++) {
            RoadFree(&graph->vertices[i]);
        }
        free(graph->vertices);
        graph->vertices = NULL;
    }
    if (graph->capitals != NULL) {
        free(graph->capitals);
        graph->capitals = NULL;
    }
    if (graph->cityOwners != NULL) {
        free(graph->cityOwners);
        graph->cityOwners = NULL;
    }
}

void graphFree(Graph** graph)
{
    if (graph == NULL || *graph == NULL) {
        return;
    }
    listFree(*graph);
    free(*graph);
    *graph = NULL;
}

void tooManyRoads(Graph* g, VertexList* list, unsigned* bestCity, unsigned* minLen)
{
    for (unsigned j = 0; j < list->count; j++) {
        if (g->cityOwners[list->roads[j].number] == -1) {
            if (list->roads[j].length < *minLen) {
                *minLen = list->roads[j].length;
                *bestCity = list->roads[j].number;
            }
        }
    }
}

void distributionCities(Graph* g)
{
    unsigned distributedCities = g->numCapitals;
    while (distributedCities != g->numCities) {
        for (unsigned index = 0; index < g->numCapitals; index++) {
            unsigned bestCity = 0;
            unsigned minLen = 0xFFFFFFFF;
            for (unsigned i = 1; i <= g->numCities; i++) {
                if (g->cityOwners[i] == index) {
                    tooManyRoads(g, &g->vertices[i], &bestCity, &minLen);
                }
            }
            if (bestCity != 0) {
                g->cityOwners[bestCity] = index;
                distributedCities++;
            }
        }
    }
}

void dijkstra(Graph* g)
{
    PriorityQueue* pq = queueCreate(g->numRoads * 2 + g->numCapitals);
    unsigned* distation = malloc((g->numCities + 1) * sizeof(unsigned));
    for (unsigned i = 0; i <= g->numCities; i++) {
        distation[i] = UINT_MAX;
    }
    for (unsigned i = 0; i < g->numCapitals; i++) {
        distation[g->capitals[i]] = 0;
        g->cityOwners[g->capitals[i]] = i;
        addInQueue(pq, g->capitals[i], 0, i);
    }

    while (pq->size > 0) {
        Node current = deleteElement(pq);
        if (current.dist > distation[current.city]) {
            continue;
        }
        VertexList* list = &g->vertices[current.city];
        for (unsigned i = 0; i < list->count; i++) {
            unsigned to = list->roads[i].number;
            unsigned len = list->roads[i].length;
            if (distation[current.city] + len < distation[to]) {
                distation[to] = distation[current.city] + len;
                g->cityOwners[to] = current.owner;
                addInQueue(pq, to, distation[to], current.owner);
            } else if (distation[current.city] + len == distation[to]) {
                if (current.owner < g->cityOwners[to]) {
                    g->cityOwners[to] = current.owner;
                    addInQueue(pq, to, distation[to], current.owner);
                }
            }
        }
    }
    free(distation);
    queueFree(pq);
}

void printResult(Graph* g)
{
    for (unsigned index = 0; index < g->numCapitals; index++) {
        printf("Города принадлежащие %u государству: ", index + 1);
        for (unsigned i = 1; i <= g->numCities; i++) {
            if (g->cityOwners[i] == index) {
                printf("%u ", i);
            }
        }
        printf("\n");
    }
}