#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include "vertexclass.h"
#include "edgeclass.h"
#include <vector>
#include <iostream>
#include <memory>
#include "getdistance.h"

//#define MDtype double
using MDtype = double;

class GraphClass:private GetDistance
{
private:
    int *HEIGHT;
    int *WIDTH;
    int *VERTICES_GEN;
    int *EDGES_GEN;
    double *RADIUS;

public:
    std::shared_ptr <VertexClass> thisVertex;
    std::shared_ptr <EdgeClass> thisEdge;
    std::shared_ptr <std::vector< std::shared_ptr <VertexClass> > > vertices;
    std::shared_ptr <std::vector< std::shared_ptr <EdgeClass> > > edges;


    GraphClass(int *h, int *w, int *vg, int* eg, double *r);
    GraphClass(const GraphClass &g);
    void operator = (const GraphClass &g);

    std::shared_ptr<VertexClass> addNewVertex(double x = 0, double y = 0);
    std::shared_ptr<EdgeClass> addNewEdge(std::shared_ptr<VertexClass> a, std::shared_ptr<VertexClass> b);

    void deleteVertex(std::shared_ptr<VertexClass> vertex);
    void deleteEdge(std::shared_ptr<EdgeClass> edge);

    std::shared_ptr<VertexClass> findNearestVertex(double x, double y);
    std::shared_ptr<EdgeClass> findNearestEdge(double x, double y);

    void generateGraph();
    void printGraphData();
    friend std::ostream& operator << (std::ostream& out, const GraphClass &g);
};

#endif // GRAPH_H
