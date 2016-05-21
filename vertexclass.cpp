#include "vertexclass.h"

VertexClass::VertexClass(double x, double y, size_t id)
{
    this->x = x;
    this->y = y;
    this->id = id;
    color = std::make_shared<Color>();
}
VertexClass::~VertexClass()
{
    //delete color;
    //for(VertexClass* v: neighbors)
    //    delete v;
}
