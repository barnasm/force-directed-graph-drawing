#include "edgeclass.h"

EdgeClass::EdgeClass(VertexClass *a, VertexClass *b, int w, int i)
{
    A = a;
    B = b;
    weight = w;
    id = i;
    //color->randColor();
}
