#ifndef SAMPO_CONTROLLER_H
#define SAMPO_CONTROLLER_H

#include "route.h"
#include <stddef.h>

#define LOOP_COUNT 5

typedef struct {
    RouteList routes;
    size_t route_counter;

    double best_route_score;
    double best_distance;
} Controller;

Controller *init_controller();
void add_route(Controller *c, RoutePtr route);
void free_controller(Controller *c);

int get_best_route(Controller *c);

#endif // SAMPO_CONTROLLER_H
