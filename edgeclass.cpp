#include "edgeclass.h"

EdgeClass::EdgeClass(std::shared_ptr<VertexClass> a, std::shared_ptr<VertexClass> b, int w, size_t i)
{
    A = a;
    B = b;
    weight = w;
    id = i;
    color = std::make_shared<Color>();
    //color = std::shared_ptr<Color> {new Color()};
}
EdgeClass::EdgeClass(const EdgeClass &e):
    weight(e.weight), id(e.id),
    color(e.color), A(e.A), B(e.B)//the same addresses, copy address
{
    //color = std::make_shared<Color>(*e.color);//different addresses, copy value
    //A = std::make_shared<VertexClass>(*e.A);
    //B = std::make_shared<VertexClass>(*e.B);
}

EdgeClass::~EdgeClass(){
    //delete color;
    //delete A;
    //delete B;
}
