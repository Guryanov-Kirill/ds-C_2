#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    Graph* g = graphRead(argv[1]);
    dijkstra(g);
    printResult(g);
    graphFree(&g);
    return 0;
}
