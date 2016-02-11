#ifndef FORCEDIRECTEDGRAPHDRAWING_H
#define FORCEDIRECTEDGRAPHDRAWING_H

#include <stdlib.h>
#include <algorithm>
#include "graphclass.h"
#include "getdistance.h"

#define MDtype double

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

    void clearForces(GraphClass *g);
    void computeAttractiveForces(GraphClass *g);
    void computeRepulsiveForces(GraphClass *g);
    void computeGravityForces(GraphClass *g);
    void moveVertices(GraphClass *g, bool moveThisVertex);
public:
    ForceDirectedGraphDrawing(int *h, int *w, MDtype *gr, MDtype *rep, MDtype *att, MDtype *s, MDtype *mxd);
    void computeIteration(GraphClass *g, bool moveThisVertex);
};

#endif // FORCEDIRECTEDGRAPHDRAWING_H
