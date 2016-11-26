#include "getdistance.h"

GetDistance::GetDistance()
{
}
MDtype GetDistance::getDx(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2 ){
  if(fabs(v2->x - v1->x) < 0.0001)
    printf("dx: \n%lf \n%lf \n%lf\n\n", v2->x, v1->x, fabs(v2->x - v1->x));
  return v2->x - v1->x;
}


MDtype GetDistance::getDy(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2){
  if(fabs(v2->x - v1->x) < 0.0001)
    printf("dy: \n%lf \n%lf \n%lf\n\n", v2->x, v1->x, fabs(v2->x - v1->x));
  return v2->y - v1->y;
}

MDtype GetDistance::
getDistanceBetweenVertex(std::shared_ptr<VertexClass> v1, std::shared_ptr<VertexClass> v2){
    MDtype dx = getDx(v1, v2);
    MDtype dy = getDy(v1, v2);
    return sqrt(dx*dx + dy*dy);
}

MDtype GetDistance::getDistanceBetweenEdge(std::shared_ptr<VertexClass> p, std::shared_ptr<EdgeClass> line){
  if(fabs(line->B->y - line->A->y) < 0.0001)
    printf("Y:\n%lf\n %lf\n\n", line->B->y,  line->A->y);
  if(fabs(line->B->x - line->A->x) < 0.0001)
    printf("X:\n%lf\n %lf\n\n", line->B->x,  line->A->x);
  if(fabs((line->B->y - line->A->y)*(line->A->x) - (line->A->y)*(line->B->x - line->A->x)) < 0.0001)
    printf("C");
  MDtype A = -(line->B->y - line->A->y);
  MDtype B =   line->B->x - line->A->x;
  MDtype C =  (line->B->y - line->A->y)*(line->A->x) - (line->A->y)*(line->B->x - line->A->x);

  MDtype d = fabs(A*p->x + B*p->y + C) / sqrt(A*A + B*B);
  if(fabs(A*p->x + B*p->y) < 0.0001 or fabs((A*p->x + B*p->y) + C)< 0.0001)
    printf("d\n");
  
  return d;
}

MDtype GetDistance::
whichSide(std::shared_ptr<VertexClass>A, std::shared_ptr<VertexClass> B, std::shared_ptr<VertexClass>p){
    return A->y*B->x + B->y*p->x + p->y*A->x - p->y*B->x - A->y*p->x - B->y*A->x;
}

std::shared_ptr<VertexClass> GetDistance::
projectionPointOntoLine(std::shared_ptr<EdgeClass> line, std::shared_ptr<VertexClass>p){
    MDtype u =
      ((p->x - line->A->x)*(line->B->x - line->A->x) +
       (p->y - line->A->y)*(line->B->y - line->A->y)) /
      ((line->A->x - line->B->x)*(line->A->x - line->B->x) +
       (line->A->y - line->B->y)*(line->A->y - line->B->y));

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
