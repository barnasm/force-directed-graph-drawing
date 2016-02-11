#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include "vertexclass.h"
#include "edgeclass.h"
#include <vector>
#include "getdistance.h"

#define MDtype double

class GraphClass:private GetDistance
{
private:
    int *HEIGHT;
    int *WIDTH;
    int *VERTICES_GEN;
    int *EDGES_GEN;
    double *RADIUS;

    VertexClass *newVertex(double x, double y, int i);
    EdgeClass *newEdge(VertexClass *a, VertexClass *b, int w, int i);

public:
    VertexClass *thisVertex;
    EdgeClass *thisEdge;
    std::vector<VertexClass*> vertices;
    std::vector<EdgeClass*> edges;


    GraphClass(int *h, int *w, int *vg, int* eg, double *r);
    void addNewVertex(double x = 0, double y = 0);
    EdgeClass *addNewEdge(VertexClass *a, VertexClass *b);

    void deleteVertex(VertexClass *vertex);
    void deleteEdge(EdgeClass *edge);

    VertexClass* findNearestVertex(double x, double y);
    EdgeClass* findNearestEdge(double x, double y);

    void generateGraph();
    void printGraphData();
    void freeGraph();
};

#endif // GRAPH_H
