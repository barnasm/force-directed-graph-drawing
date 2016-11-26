#include "graphclass.h"

GraphClass::GraphClass(int *h, int *w, int *vg, int *eg, double *r)
{
    HEIGHT = h;
    WIDTH = w;
    VERTICES_GEN = vg;
    EDGES_GEN = eg;
    RADIUS = r;
    vertices = std::make_shared <std::vector< std::shared_ptr <VertexClass> >>();
    edges    = std::make_shared <std::vector< std::shared_ptr <EdgeClass> >>();
}
GraphClass::GraphClass(const GraphClass &g):
    HEIGHT(g.HEIGHT), WIDTH(g.WIDTH), VERTICES_GEN(g.VERTICES_GEN),
    EDGES_GEN(g.EDGES_GEN), RADIUS(g.RADIUS)
    //,edges(g.edges)
    //,vertices(g.vertices)
{
    std::cout << "constructor" << std::endl;
    thisEdge = std::make_shared<EdgeClass>(*g.thisEdge);
    thisVertex = std::make_shared<VertexClass>(*g.thisVertex);

    std::cout << (thisEdge->color) << std::endl << (g.thisEdge->color) << std::endl;

    vertices = std::make_shared <std::vector< std::shared_ptr <VertexClass> >>();
    edges    = std::make_shared <std::vector< std::shared_ptr <EdgeClass> >>();

    for(std::shared_ptr<VertexClass> a: *g.vertices)
        vertices->push_back(std::make_shared<VertexClass>(*a));
    for(std::shared_ptr<EdgeClass> a: *g.edges)
        edges->push_back(std::make_shared<EdgeClass>(*a));
}
//void GraphClass::operator = (const GraphClass &g){
//    std::cout << "assigment" << std::endl;
//    *this = GraphClass(g);
//}

void GraphClass::deleteEdge(std::shared_ptr<EdgeClass> edge){
    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i) == edge){

            edges->erase(edges->begin()+i);
            return;
        }
    }
}

std::shared_ptr<VertexClass> GraphClass::addNewVertex(double x, double y){
    vertices->push_back(std::make_shared<VertexClass> (x, y, vertices->size()));
    return vertices->at(vertices->size()-1);
}

std::shared_ptr<EdgeClass> GraphClass::
addNewEdge(std::shared_ptr<VertexClass> a, std::shared_ptr<VertexClass> b){
    if(a == b || a == nullptr || b == nullptr) return nullptr;
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
            //vertices->at(i)->neighbors.clear();
            vertices->erase(vertices->begin()+i);
            return;
        }
    }
}

std::shared_ptr<VertexClass> GraphClass::findNearestVertex(double x, double y){
    if(vertices->size() == 0) return nullptr;

    auto v = std::make_shared<VertexClass> (x, y);
    auto vRet = vertices->at(0);
    MDtype minDist = getDistanceBetweenVertex(v, vRet);
    for(unsigned int i = 0; i < vertices->size(); i++){
        if(getDistanceBetweenVertex(v, vertices->at(i)) < minDist){
            minDist = getDistanceBetweenVertex(v, vertices->at(i));
            vRet = vertices->at(i);
        }
    }
    if (minDist > (*RADIUS-*RADIUS/1.5) + 10) return nullptr;
    return vRet;
}

std::shared_ptr<EdgeClass> GraphClass::findNearestEdge(double x, double y){
    if(edges->size() == 0) return nullptr;

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
        return nullptr;
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
        //vertices->at(a)->neighbors.push_back(vertices->at(b));
        //vertices->at(b)->neighbors.push_back(vertices->at(a));

        //edges->push_back(newEdge( vertices->at(a), vertices->at(b), w, i));
        addNewEdge(vertices->at(a), vertices->at(b));
    }
    thisVertex = vertices->at(0);
    thisEdge = edges->at(0);
    //std::cout << *this << std::endl;
}

std::ostream& operator << (std::ostream& out, const GraphClass &g){
    int s = 4;
    out << "Vertices: " << g.vertices->size() << std::endl;
    for(unsigned int i = 0; i < g.vertices->size(); i++){
        out << "id: ";
        out.width(s);
#if 0
        out << g.vertices->at(i)->id <<
               " Connections: " << g.vertices->at(i)->neighbors.size() << std::endl;

        for(unsigned int j = 0; j < g.vertices->at(i)->neighbors.size(); j++){
            out.width(3*s);
            out <<  g.vertices->at(i)->neighbors[j]->id << std::endl;

        }
#endif
    }


//    out << "\n\nEdges\n";
//    for(unsigned int i = 0; i < g.edges->size(); i++){
//        printf("%*i) %*lu--->%*lu\n", s, i, s, edges->at(i)->A->id, s, edges->at(i)->B->id);
//    }
    return out;
}
