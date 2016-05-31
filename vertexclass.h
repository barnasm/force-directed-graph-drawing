#ifndef VERTEXCLASS_H
#define VERTEXCLASS_H

#include "color.h"
#include <vector>
#include <memory>

//#define MDtype double
using MDtype = double;

class VertexClass: public Color
{
public:
    //char name[10];
    size_t id;
    double x;
    double y;

    MDtype xForce;
    MDtype yForce;

    std::shared_ptr <Color> color;

    std::vector< std::shared_ptr <VertexClass> > neighbors;

    VertexClass(double x = 0, double y = 0, size_t id = -1);
    VertexClass(const VertexClass &v);
    ~VertexClass();
};

#endif // VERTEXCLASS_H
