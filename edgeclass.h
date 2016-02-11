#ifndef EDGECLASS_H
#define EDGECLASS_H

#include "color.h"
#include "vertexclass.h"

class EdgeClass: public Color
{
public:
    int weight;
    //char *name;
    int id;
    Color color;

    VertexClass *A;
    VertexClass *B;


    EdgeClass(VertexClass *a = NULL, VertexClass *b = NULL, int w = 0, int i = -1);

};

#endif // EDGECLASS_H
