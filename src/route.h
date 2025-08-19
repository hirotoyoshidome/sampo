#ifndef SAMPO_ROUTE_H
#define SAMPO_ROUTE_H

#define DEFAULT_ROUTING_SIZE 255

#include "graph.h"
#include <stddef.h>

typedef unsigned int *Routing;

typedef struct {
    double total_score;
    double total_distance;
    int turn_count;
    NodePtr start_node;
    Routing routing;
    size_t route_step;
} Route;

typedef Route *RoutePtr;
typedef RoutePtr *RouteList;

RoutePtr init_route();
void free_route(RoutePtr route);

void show_route(const RoutePtr route);

#endif // SAMPO_ROUTE_H
