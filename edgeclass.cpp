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
EdgeClass::~EdgeClass(){
    //delete color;
    //delete A;
    //delete B;
}
