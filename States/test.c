#include "graph.h"
#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

bool testQueueMinProperty(void)
{
    PriorityQueue* pq = queueCreate(10);
    addInQueue(pq, 1, 100, 0);
    addInQueue(pq, 2, 10, 1);
    addInQueue(pq, 3, 50, 2);

    Node n1 = deleteElement(pq);
    Node n2 = deleteElement(pq);
    Node n3 = deleteElement(pq);

    bool res = (n1.dist == 10 && n1.city == 2) && (n2.dist == 50 && n2.city == 3) && (n3.dist == 100 && n3.city == 1);

    queueFree(pq);
    return res;
}

bool testQueueOwner(void)
{
    PriorityQueue* pq = queueCreate(5);
    addInQueue(pq, 5, 10, 42);
    Node n = deleteElement(pq);
    bool res = (n.owner == 42);
    queueFree(pq);
    return res;
}

bool testDijkstraSingleCapital(void)
{
    FILE* f = fopen("test_single.txt", "w");
    fprintf(f, "3 2\n1 2 5\n2 3 5\n1\n1\n");
    fclose(f);

    Graph* g = graphRead("test_single.txt");
    dijkstra(g);

    bool res = (g->cityOwners[1] == 0 && g->cityOwners[2] == 0 && g->cityOwners[3] == 0);

    graphFree(&g);
    remove("test_single.txt");
    return res;
}

bool testDijkstraNearestCapital(void)
{
    FILE* f = fopen("test_near.txt", "w");
    fprintf(f, "3 2\n1 2 10\n3 2 1\n2\n1 3\n");
    fclose(f);

    Graph* g = graphRead("test_near.txt");
    dijkstra(g);

    bool res = (g->cityOwners[2] == 1);

    graphFree(&g);
    remove("test_near.txt");
    return res;
}

bool testDijkstraTieBreaker(void)
{
    FILE* f = fopen("test_tie.txt", "w");
    fprintf(f, "3 2\n1 2 10\n3 2 10\n2\n1 3\n");
    fclose(f);

    Graph* g = graphRead("test_tie.txt");
    dijkstra(g);

    bool res = (g->cityOwners[2] == 0);

    graphFree(&g);
    remove("test_tie.txt");
    return res;
}

bool testGraphReadValidation(void)
{
    FILE* f = fopen("test_read.txt", "w");
    fprintf(f, "2 1\n1 2 100\n1\n1\n");
    fclose(f);

    Graph* g = graphRead("test_read.txt");
    bool res = (g != NULL && g->numCities == 2 && g->numRoads == 1 && g->numCapitals == 1);

    graphFree(&g);
    remove("test_read.txt");
    return res;
}

int main(int argc, char** argv)
{
    bool testMode = false;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--test") == 0) {
            testMode = true;
            break;
        }
    }

    if (testMode) {
        const int TEST_NUM = 6;
        bool good = true;
        bool (*tests[])(void) = {
            &testQueueMinProperty,
            &testQueueOwner,
            &testGraphReadValidation,
            &testDijkstraSingleCapital,
            &testDijkstraNearestCapital,
            &testDijkstraTieBreaker
        };

        for (int testNum = 0; testNum < TEST_NUM; ++testNum) {
            if (tests[testNum]()) {
                printf(GREEN("Test %d passed!\n"), testNum + 1);
            } else {
                printf(RED("Test %d failed!\n"), testNum + 1);
                good = false;
            }
        }
        return good ? 0 : 1;
    }
}
