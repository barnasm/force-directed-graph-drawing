#ifndef EDGECLASS_H
#define EDGECLASS_H

#include "color.h"
#include "vertexclass.h"
#include <memory>

class EdgeClass: public Color
{
public:
    int weight;
    size_t id;
    std::shared_ptr<Color> color;

    std::shared_ptr <VertexClass> A;
    std::shared_ptr <VertexClass> B;


    EdgeClass(std::shared_ptr <VertexClass> a = nullptr, std::shared_ptr <VertexClass> b = nullptr, int w = 0, size_t i = -1);
    EdgeClass(const EdgeClass &e);
    ~EdgeClass();
};

#endif // EDGECLASS_H
