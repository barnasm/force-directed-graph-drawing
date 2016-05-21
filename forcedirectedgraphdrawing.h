#ifndef FORCEDIRECTEDGRAPHDRAWING_H
#define FORCEDIRECTEDGRAPHDRAWING_H

#include <stdlib.h>
#include <algorithm>
#include "graphclass.h"
#include "getdistance.h"

//#define MDtype double
using MDtype = double;

class ForceDirectedGraphDrawing: public GetDistance
{
private:
    int *HEIGHT;
    int *WIDTH;
    MDtype *SPEED;
    MDtype *MAX_DISPLACEMENT;
    MDtype *GRAVITY;
    MDtype *REPULSION;
    MDtype *ATTRACTION;

    void clearForces(std::shared_ptr<GraphClass> g);
    void computeAttractiveForces(std::shared_ptr<GraphClass> g);
    void computeRepulsiveForces(std::shared_ptr<GraphClass> g);
    void computeGravityForces(std::shared_ptr<GraphClass> g);
    void moveVertices(std::shared_ptr<GraphClass> g, bool moveThisVertex);
public:
    ForceDirectedGraphDrawing(int *h, int *w, MDtype *gr, MDtype *rep, MDtype *att, MDtype *s, MDtype *mxd);
    void computeIteration(std::shared_ptr<GraphClass> g, bool moveThisVertex);
};

#endif // FORCEDIRECTEDGRAPHDRAWING_H
