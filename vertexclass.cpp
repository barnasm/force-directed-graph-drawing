#include "vertexclass.h"

VertexClass::VertexClass(double x, double y, size_t id)
{
    this->x = x;
    this->y = y;
    this->id = id;
    color = std::make_shared<Color>();
}
VertexClass::VertexClass(const VertexClass &v):
    id(v.id), x(v.x), y(v.y), xForce(0), yForce(0)
    //,color(v.color), neighbors(v.neighbors)//the same addresses
{}

VertexClass::~VertexClass()
{
    //delete color;
    //for(VertexClass* v: neighbors)
    //    delete v;
}
