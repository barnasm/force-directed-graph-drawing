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
void ForceDirectedGraphDrawing::clearForces(GraphClass *g)
{
    for(unsigned int i = 0; i < g->vertices.size(); ++i){
        g->vertices[i]->xForce = 0;
        g->vertices[i]->yForce = 0;
    }
}

void ForceDirectedGraphDrawing::computeAttractiveForces(GraphClass *g)
{
    MDtype k = 0.1 * sqrt((*HEIGHT * *WIDTH) / (1.0 + g->vertices.size()));
    for(unsigned int i = 0; i < g->edges.size(); ++i){
        VertexClass* v1 = g->edges[i]->A;
        VertexClass* v2 = g->edges[i]->B;
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

void ForceDirectedGraphDrawing::computeRepulsiveForces(GraphClass *g)
{
    MDtype k = sqrt((*WIDTH * *HEIGHT) / (1.0 + g->vertices.size()));
    for(unsigned int i = 0; i < g->vertices.size(); ++i){
        for(unsigned int j = 0; j < g->vertices.size(); ++j){
            if(i != j){
                VertexClass* v1 = g->vertices[i];
                VertexClass* v2 = g->vertices[j];
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


void ForceDirectedGraphDrawing::computeGravityForces(GraphClass *g)
{
    MDtype k = sqrt((*WIDTH * *HEIGHT) / (1.0 + g->vertices.size()));
    for(unsigned int i = 0; i < g->vertices.size(); ++i){
        VertexClass* v = g->vertices[i];
        MDtype x = (MDtype)v->x - *WIDTH/2.0;
        MDtype y = (MDtype)v->y - *HEIGHT/2.0;
        MDtype distance = sqrt(x*x + y*y);
        MDtype gravityForce = k * *GRAVITY * distance;
        v->xForce -= gravityForce * x/distance;
        v->yForce -= gravityForce * y/distance;
    }

}

void ForceDirectedGraphDrawing::moveVertices(GraphClass *g, bool moveThisVertex){
    for (unsigned int i = 0; i<g->vertices.size(); ++i) {
        if(!moveThisVertex && g->thisVertex == g->vertices[i]) continue;
        VertexClass* v = g->vertices[i];

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

void ForceDirectedGraphDrawing::computeIteration(GraphClass *g, bool moveThisVertex)
{
    if(g == NULL) return;

    clearForces(g);
    computeAttractiveForces(g);
    computeRepulsiveForces(g);
    computeGravityForces(g);
    moveVertices(g, moveThisVertex);
}
