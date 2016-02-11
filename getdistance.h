#ifndef GETDISTANCE_H
#define GETDISTANCE_H

#include <cmath>
#include "vertexclass.h"
#include "edgeclass.h"

#define MDtype double

class GetDistance
{
private:
    MDtype whichSide(VertexClass *A, VertexClass *B, VertexClass *p);
    VertexClass* projectionPointOntoLine(EdgeClass *line, VertexClass *p);
    VertexClass *r;
public:
    GetDistance();
    MDtype getDx(VertexClass *v1, VertexClass *v2);
    MDtype getDy(VertexClass *v1, VertexClass *v2);
    MDtype getDistanceBetweenVertex(VertexClass *v1, VertexClass *v2);
    MDtype getDistanceBetweenEdge(VertexClass *p, EdgeClass *line);

    bool isBetween(EdgeClass *edge, VertexClass *v);
};

#endif // GETDISTANCE_H
