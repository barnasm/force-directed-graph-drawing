#include "graphclass.h"

GraphClass::GraphClass(int *h, int *w, int *vg, int *eg, double *r)
{
    HEIGHT = h;
    WIDTH = w;
    VERTICES_GEN = vg;
    EDGES_GEN = eg;
    RADIUS = r;
    //vertices = new std::vector<VertexClass*>;
    //edges    = new std::vector<EdgeClass*>;
}

VertexClass* GraphClass::newVertex(double x, double y, int i){
    //printf("NewVertex\n");
    VertexClass *v = new VertexClass(x, y, i);
    v->color.randColor();

    return v;
}

EdgeClass* GraphClass::newEdge(VertexClass *a, VertexClass *b, int w, int i){
    //printf("NewEdge\n");
    EdgeClass *e = new EdgeClass(a, b, w, i);
    e->color.randColor();
    return e;
}

void GraphClass::deleteEdge(EdgeClass *edge){
    for(unsigned int i = 0; i < edges.size(); i++){
        if(edges[i] == edge){
            free(edges[i]);
            edges.erase(edges.begin()+i);
            return;
        }
    }
}

void GraphClass::addNewVertex(double x, double y){
    vertices.push_back(newVertex(x, y, vertices.size()));
}

EdgeClass* GraphClass::addNewEdge(VertexClass *a, VertexClass *b){
    if(a == b || a == NULL || b == NULL) return NULL;
    edges.push_back(newEdge( a, b, 0, edges.size()));
    return edges[edges.size()-1];
}

void GraphClass::deleteVertex(VertexClass *vertex){
    for(unsigned int i = 0; i < edges.size(); i++){
        if(edges[i]->A == vertex || edges[i]->B == vertex){
            free(edges[i]);
            edges.erase(edges.begin()+i);
            i--;
        }
    }
    for(unsigned int i = 0; i < vertices.size(); i++){
        if(vertices[i] == vertex){
            vertices[i]->neighbors.clear();
            free(vertices[i]);
            vertices.erase(vertices.begin()+i);
            return;
        }
    }
}

VertexClass* GraphClass::findNearestVertex(double x, double y){
    if(vertices.size() == 0) return NULL;

    VertexClass *v = new VertexClass(x, y);
    VertexClass *vRet = vertices[0];
    MDtype minDist = getDistanceBetweenVertex(v, vRet);
    for(unsigned int i = 0; i < vertices.size(); i++){
        if(getDistanceBetweenVertex(v, vertices[i]) < minDist){
            minDist = getDistanceBetweenVertex(v, vertices[i]);
            vRet = vertices[i];
        }
    }
    free(v);
    if (minDist > (*RADIUS-*RADIUS/1.5) + 10) return NULL;
    return vRet;
}

EdgeClass* GraphClass::findNearestEdge(double x, double y){
    if(edges.size() == 0) return NULL;

    VertexClass *v = new VertexClass(x, y);
    EdgeClass *eRet = edges[0];
    MDtype minDist = getDistanceBetweenEdge(v, eRet);
    for(unsigned int i = 0; i < edges.size(); i++){
        if(getDistanceBetweenEdge(v, edges[i]) < minDist && isBetween(edges[i], v)){
            minDist = getDistanceBetweenEdge(v, edges[i]);
            eRet = edges[i];
        }
    }

    if(!isBetween(eRet, v) || minDist > 10){
        free(v);
        return NULL;
    }
    free(v);
    return eRet;
}

void GraphClass::generateGraph(){
    for(int i = 0; i < *VERTICES_GEN; i++){
        addNewVertex((double)(rand() % *WIDTH), (double)(rand() % *HEIGHT));
    }

    for(int i = 0; i < *EDGES_GEN; i++){
        int a = rand() % *VERTICES_GEN;
        int b = rand() % *VERTICES_GEN;
        if(a == b){
            i--;
            continue;
        }
        vertices[a]->neighbors.push_back(vertices[b]);
        vertices[b]->neighbors.push_back(vertices[a]);

        //edges.push_back(newEdge( vertices[a], vertices[b], w, i));
        addNewEdge(vertices[a], vertices[b]);
    }
    thisVertex = vertices[0];
    thisEdge = edges[0];
}

void GraphClass::printGraphData(){
    int s = 3;
    printf("Vertices\n");
    for(unsigned int i = 0; i < vertices.size(); i++){
        printf("%*u) Neighbors: %lu  id: %i\n", 3, i, vertices[i]->neighbors.size(), vertices[i]->id);
        for(unsigned int j = 0; j < vertices[i]->neighbors.size(); j++){
            printf("%*i) Neighbors: %lu id: %d\n", 8, j, vertices[i]->neighbors[j]->neighbors.size(), vertices[i]->neighbors[j]->id);
        }printf("\n");
    }

    printf("\n\nEdges\n");
    for(unsigned int i = 0; i < edges.size(); i++){
        printf("%*i) %*i--->%*i\n", s, i, s, edges[i]->A->id, s, edges[i]->B->id);
    }
}

void GraphClass::freeGraph(){
    for(unsigned int i = 0; i < vertices.size(); i++){
        vertices[i]->neighbors.clear();
        free(vertices[i]);
    }
    for(unsigned int i = 0; i < edges.size(); i++){
        free(edges[i]);
    }
    vertices.clear();
    edges.clear();
}
