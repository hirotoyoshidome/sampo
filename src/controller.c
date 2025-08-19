#include "controller.h"

#include <stdlib.h>

#include "logger.h"

Controller *init_controller() {
    Controller *c = (Controller *)malloc(sizeof(Controller));
    if (c == NULL) {
        fetal(&global_logger, "Controller Initialization Failed");
        exit(EXIT_FAILURE);
    }
    RouteList rl = (RouteList)malloc(sizeof(RoutePtr) * LOOP_COUNT);
    if (rl == NULL) {
        fetal(&global_logger, "RouteList Initialization Failed");
        exit(EXIT_FAILURE);
    }
    c->routes = rl;
    c->route_counter = 0;

    return c;
}

void add_route(Controller *c, RoutePtr route) {
    if (c == NULL || route == NULL) {
        fetal(&global_logger, "add_route: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    c->routes[c->route_counter++] = route;
}

void free_controller(Controller *c) {
    if (c == NULL) {
        fetal(&global_logger, "free_controller: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < c->route_counter; i++) {
        free_route(c->routes[i]);
    }
    free(c->routes);
    c->routes = NULL;
    free(c);
    c = NULL;
}

int get_best_route(Controller *c) {
    if (c == NULL) {
        fetal(&global_logger, "get_best_route: Invalid Pointer");
    }

    double max_route_score = 0.0;
    int idx = 0;
    for (size_t i = 0; i < c->route_counter; i++) {
        RoutePtr r = c->routes[i];
        if (r->total_score > max_route_score) {
            max_route_score = r->total_score;
            idx = i;
        }
    }

    return idx;
}
