#include "forcedirectedgraphdrawing.h"

ForceDirectedGraphDrawing::ForceDirectedGraphDrawing(int *h, int *w, MDtype *gr, MDtype *rep, MDtype *att, MDtype *s, MDtype *mxd)
{
    HEIGHT = h;
    WIDTH = w;
    GRAVITY = gr;
    REPULSION = rep;
    ATTRACTION = att;
    SPEED = s;
    MAX_DISPLACEMENT = mxd;
}
void ForceDirectedGraphDrawing::clearForces(std::shared_ptr<GraphClass>g)
{
    for(unsigned int i = 0; i < g->vertices->size(); ++i){
        g->vertices->at(i)->xForce = 0;
        g->vertices->at(i)->yForce = 0;
    }
}

void ForceDirectedGraphDrawing::computeAttractiveForces(std::shared_ptr<GraphClass>g)
{
    MDtype k = 0.1 * sqrt((*HEIGHT * *WIDTH) / (1.0 + g->vertices->size()));
    for(unsigned int i = 0; i < g->edges->size(); ++i){
        std::shared_ptr<VertexClass> v1 = g->edges->at(i)->A;
        std::shared_ptr<VertexClass> v2 = g->edges->at(i)->B;
        MDtype distance = getDistanceBetweenVertex(v1, v2);
        MDtype attractiveF = distance*distance / k * *ATTRACTION;
        if (distance > 0) {
            v1->xForce += (getDx(v1, v2) / distance) * attractiveF;
            v1->yForce += (getDy(v1, v2) / distance) * attractiveF;
            v2->xForce -= (getDx(v1, v2) / distance) * attractiveF;
            v2->yForce -= (getDy(v1, v2) / distance) * attractiveF;
        }
    }
}

void ForceDirectedGraphDrawing::computeRepulsiveForces(std::shared_ptr<GraphClass> g)
{
    MDtype k = sqrt((*WIDTH * *HEIGHT) / (1.0 + g->vertices->size()));
    for(unsigned int i = 0; i < g->vertices->size(); ++i){
        for(unsigned int j = 0; j < g->vertices->size(); ++j){
            if(i != j){
                std::shared_ptr<VertexClass> v1 = g->vertices->at(i);
                std::shared_ptr<VertexClass> v2 = g->vertices->at(j);
                MDtype distance = getDistanceBetweenVertex(v1, v2);
                if (distance > 0) {
                    MDtype repulsiveF = k*k / distance * *REPULSION;
                    v1->xForce -= (( getDx(v1,v2) / distance) * repulsiveF);
                    v1->yForce -= (( getDy(v1,v2) / distance) * repulsiveF);
                }
            }
        }
    }
}


void ForceDirectedGraphDrawing::computeGravityForces(std::shared_ptr<GraphClass> g)
{
    MDtype k = sqrt((*WIDTH * *HEIGHT) / (1.0 + g->vertices->size()));
    for(unsigned int i = 0; i < g->vertices->size(); ++i){
        std::shared_ptr<VertexClass>  v = g->vertices->at(i);
        MDtype x = (MDtype)v->x - *WIDTH/2.0;
        MDtype y = (MDtype)v->y - *HEIGHT/2.0;
        MDtype distance = sqrt(x*x + y*y);
        MDtype gravityForce = k * *GRAVITY * distance;
        v->xForce -= gravityForce * x/distance;
        v->yForce -= gravityForce * y/distance;
    }

}

void ForceDirectedGraphDrawing::moveVertices(std::shared_ptr<GraphClass> g, bool moveThisVertex){
    for (unsigned int i = 0; i<g->vertices->size(); ++i) {
        if(!moveThisVertex && g->thisVertex == g->vertices->at(i)) continue;
        std::shared_ptr<VertexClass>  v = g->vertices->at(i);

        v->xForce *= *SPEED;
        v->yForce *= *SPEED;
        MDtype distance = sqrt(v->xForce * v->xForce + v->yForce * v->yForce);

        if (distance > 0) {
            MDtype limitedDist = std::min(*MAX_DISPLACEMENT * *SPEED, distance);
            v->x +=(v->xForce / distance * limitedDist);
            v->y +=(v->yForce / distance * limitedDist);
        }
    }
}

void ForceDirectedGraphDrawing::computeIteration(std::shared_ptr<GraphClass>g, bool moveThisVertex)
{
    if(g == nullptr) return;

    clearForces(g);
    computeAttractiveForces(g);
    computeRepulsiveForces(g);
    computeGravityForces(g);
    moveVertices(g, moveThisVertex);

}
