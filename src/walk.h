#ifndef SAMPO_WALK_H
#define SAMPO_WALK_H

#include "graph.h"
#include "route.h"

typedef struct {
    EdgeDirection prev_direction;
} WalkState;

void walk(RoutePtr route, NodePtr start, const double distance);

#endif // SAMPO_WALK_H
