#include "getdistance.h"

GetDistance::GetDistance()
{
    r = new VertexClass;
}
MDtype GetDistance::getDx(VertexClass* v1, VertexClass* v2){
    return v2->x - v1->x;
}


MDtype GetDistance::getDy(VertexClass* v1, VertexClass* v2){
    return v2->y - v1->y;
}

MDtype GetDistance::getDistanceBetweenVertex(VertexClass* v1, VertexClass* v2){
    MDtype dx = getDx(v1, v2);
    MDtype dy = getDy(v1, v2);
    return sqrt(dx*dx + dy*dy);
}

MDtype GetDistance::getDistanceBetweenEdge(VertexClass* p, EdgeClass* line){
    MDtype A = -(line->B->y - line->A->y);
    MDtype B =   line->B->x - line->A->x;
    MDtype C =  (line->B->y - line->A->y)*(line->A->x) - (line->A->y)*(line->B->x - line->A->x);

    MDtype d = abs(A*p->x + B*p->y + C) / sqrt(A*A + B*B);
    return d;
}

MDtype GetDistance::whichSide(VertexClass *A, VertexClass *B, VertexClass *p){
    return A->y*B->x + B->y*p->x + p->y*A->x - p->y*B->x - A->y*p->x - B->y*A->x;
}

VertexClass* GetDistance::projectionPointOntoLine(EdgeClass *line, VertexClass *p){
    MDtype u = ((p->x - line->A->x)*(line->B->x - line->A->x) + (p->y - line->A->y)*(line->B->y - line->A->y)) /
               ((line->A->x - line->B->x)*(line->A->x - line->B->x) + (line->A->y - line->B->y)*(line->A->y - line->B->y));

    r->x = ((line->B->x - line->A->x) * u) + line->A->x;
    r->y = ((line->B->y - line->A->y) * u) + line->A->y;
    return r;
}

bool GetDistance::isBetween(EdgeClass *edge, VertexClass *v){
    projectionPointOntoLine(edge, v);

    if(whichSide(v, r, edge->A) * whichSide(v, r, edge->B) <= 0){
        return true;
    }
    else{
        return false;
    }
}
