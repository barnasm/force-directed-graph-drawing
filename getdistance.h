#ifndef GETDISTANCE_H
#define GETDISTANCE_H

#include <cmath>
#include <memory>
#include "vertexclass.h"
#include "edgeclass.h"

//#define MDtype double
using MDtype = double;

class GetDistance
{
private:
    MDtype whichSide(std::shared_ptr<VertexClass> A, std::shared_ptr<VertexClass> B, std::shared_ptr<VertexClass> p);
    std::shared_ptr<VertexClass> projectionPointOntoLine(std::shared_ptr<EdgeClass> line, std::shared_ptr<VertexClass> p);
public:
    GetDistance();
    MDtype getDx(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2);
    MDtype getDy(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2);
    MDtype getDistanceBetweenVertex(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2);
    MDtype getDistanceBetweenEdge(std::shared_ptr<VertexClass> p, std::shared_ptr<EdgeClass> line);

    bool isBetween(std::shared_ptr<EdgeClass> edge, std::shared_ptr<VertexClass> v);
};

#endif // GETDISTANCE_H
