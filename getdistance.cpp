#include "getdistance.h"

GetDistance::GetDistance()
{
}
MDtype GetDistance::getDx(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2 ){
    return v2->x - v1->x;
}


MDtype GetDistance::getDy(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2){
    return v2->y - v1->y;
}

MDtype GetDistance::getDistanceBetweenVertex(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2){
    MDtype dx = getDx(v1, v2);
    MDtype dy = getDy(v1, v2);
    return sqrt(dx*dx + dy*dy);
}

MDtype GetDistance::getDistanceBetweenEdge(std::shared_ptr<VertexClass> p, std::shared_ptr<EdgeClass> line){
    MDtype A = -(line->B->y - line->A->y);
    MDtype B =   line->B->x - line->A->x;
    MDtype C =  (line->B->y - line->A->y)*(line->A->x) - (line->A->y)*(line->B->x - line->A->x);

    MDtype d = abs(A*p->x + B*p->y + C) / sqrt(A*A + B*B);
    return d;
}

MDtype GetDistance::whichSide(std::shared_ptr<VertexClass>A, std::shared_ptr<VertexClass> B, std::shared_ptr<VertexClass>p){
    return A->y*B->x + B->y*p->x + p->y*A->x - p->y*B->x - A->y*p->x - B->y*A->x;
}

std::shared_ptr<VertexClass> GetDistance::projectionPointOntoLine(std::shared_ptr<EdgeClass> line, std::shared_ptr<VertexClass>p){
    MDtype u = ((p->x - line->A->x)*(line->B->x - line->A->x) + (p->y - line->A->y)*(line->B->y - line->A->y)) /
               ((line->A->x - line->B->x)*(line->A->x - line->B->x) + (line->A->y - line->B->y)*(line->A->y - line->B->y));

//    return std::shared_ptr<VertexClass> {
//        new VertexClass(((line->B->x - line->A->x) * u) + line->A->x,
//                       ((line->B->y - line->A->y) * u) + line->A->y)
//    };
    return std::make_shared<VertexClass>(((line->B->x - line->A->x) * u) + line->A->x,
                                         ((line->B->y - line->A->y) * u) + line->A->y);
}

bool GetDistance::isBetween(std::shared_ptr<EdgeClass> edge, std::shared_ptr<VertexClass>v){
    std::shared_ptr<VertexClass> r = projectionPointOntoLine(edge, v);

    if(whichSide(v, r, edge->A) * whichSide(v, r, edge->B) <= 0){
        return true;
    }
    else{
        return false;
    }
}
