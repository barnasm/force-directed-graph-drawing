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

void GraphClass::deleteEdge(std::shared_ptr<EdgeClass> edge){
    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i) == edge){

            edges->erase(edges->begin()+i);
            return;
        }
    }
}

void GraphClass::addNewVertex(double x, double y){
    vertices->push_back(std::make_shared<VertexClass> (x, y, vertices->size()));
}

std::shared_ptr<EdgeClass> GraphClass::addNewEdge(std::shared_ptr<VertexClass> a, std::shared_ptr<VertexClass> b){
    if(a == b || a == NULL || b == NULL) return NULL;
    edges->push_back(std::make_shared <EdgeClass>( a, b, 0, edges->size()));
    return edges->at(edges->size()-1);
}

void GraphClass::deleteVertex(std::shared_ptr<VertexClass> vertex){
    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i)->A == vertex || edges->at(i)->B == vertex){
            edges->erase(edges->begin()+i);
            i--;
        }
    }
    for(unsigned int i = 0; i < vertices->size(); i++){
        if(vertices->at(i) == vertex){
            vertices->at(i)->neighbors.clear();
            vertices->erase(vertices->begin()+i);
            return;
        }
    }
}

std::shared_ptr<VertexClass> GraphClass::findNearestVertex(double x, double y){
    if(vertices->size() == 0) return NULL;

    auto v = std::make_shared<VertexClass> (x, y);
    auto vRet = vertices->at(0);
    MDtype minDist = getDistanceBetweenVertex(v, vRet);
    for(unsigned int i = 0; i < vertices->size(); i++){
        if(getDistanceBetweenVertex(v, vertices->at(i)) < minDist){
            minDist = getDistanceBetweenVertex(v, vertices->at(i));
            vRet = vertices->at(i);
        }
    }
    if (minDist > (*RADIUS-*RADIUS/1.5) + 10) return NULL;
    return vRet;
}

std::shared_ptr<EdgeClass> GraphClass::findNearestEdge(double x, double y){
    if(edges->size() == 0) return NULL;

    auto v = std::make_shared<VertexClass> (x, y);
    auto eRet = edges->at(0);
    MDtype minDist = getDistanceBetweenEdge(v, eRet);
    for(unsigned int i = 0; i < edges->size(); i++){
        if(getDistanceBetweenEdge(v, edges->at(i)) < minDist && isBetween(edges->at(i), v)){
            minDist = getDistanceBetweenEdge(v, edges->at(i));
            eRet = edges->at(i);
        }
    }

    if(!isBetween(eRet, v) || minDist > 10){
        return NULL;
    }
    return eRet;
}

void GraphClass::generateGraph(){
    vertices = std::make_shared <std::vector< std::shared_ptr <VertexClass> >>();
    edges    = std::make_shared <std::vector< std::shared_ptr <EdgeClass> >>();

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
        vertices->at(a)->neighbors.push_back(vertices->at(b));
        vertices->at(b)->neighbors.push_back(vertices->at(a));

        //edges->push_back(newEdge( vertices->at(a), vertices->at(b), w, i));
        addNewEdge(vertices->at(a), vertices->at(b));
    }
    thisVertex = vertices->at(0);
    thisEdge = edges->at(0);
}

std::ostream& operator << (std::ostream& out, const GraphClass &g){
    int s = 3;
    out << "Vertices\n";
    for(unsigned int i = 0; i < g.vertices->size(); i++){
        out << i << ") " <<
               " Neighbors: " << g.vertices->at(i)->neighbors.size() <<
               " id: " << g.vertices->at(i)->id << std::endl;
        for(unsigned int j = 0; j < g.vertices->at(i)->neighbors.size(); j++){
            out << "" << j << ") " <<
                   "Neighbors: " << g.vertices->at(i)->neighbors[j]->neighbors.size() <<
                   " id: " <<  g.vertices->at(i)->neighbors[j]->id << std::endl;
        }
        out << "\n";
    }


    out << "\n\nEdges\n";
    for(unsigned int i = 0; i < g.edges->size(); i++){
        //printf("%*i) %*lu--->%*lu\n", s, i, s, edges->at(i)->A->id, s, edges->at(i)->B->id);
    }
    return out;
}
