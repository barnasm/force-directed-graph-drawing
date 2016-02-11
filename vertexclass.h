#ifndef VERTEXCLASS_H
#define VERTEXCLASS_H

#include "color.h"
#include <vector>

#define MDtype double

class VertexClass: public Color
{
public:
    //char name[10];
    int id;
    double x;
    double y;

    MDtype xForce;
    MDtype yForce;

    Color color;

    std::vector<VertexClass*> neighbors;

    VertexClass(double x = 0, double y = 0, int id = -1);
};

#endif // VERTEXCLASS_H
