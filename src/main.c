#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "controller.h"
#include "graph.h"
#include "route.h"
#include "route_score.h"
#include "walk.h"

#include "logger.h"

#include "poi.h"

int main() {
    init_logger(&global_logger, "./sampo.log", INFO);
    info(&global_logger, "----------[START]----------");

    srand(time(NULL));

    init_nodes();
    init_edges();

    // FIXME: verifying code
    PoiPtr poi = init_poi(STORE, "お店のPOIサンプル");
    add_poi_to_edge(nodes[25]->edges[0], poi);

    const double distance = 4000.0;

    Controller *c = init_controller();

    // Run
    for (int i = 0; i < LOOP_COUNT; i++) {
        NodePtr start = nodes[0];

        RoutePtr route = init_route();
        route->start_node = start;

        walk(route, start, distance);

        calc_route_score(route);

        add_route(c, route);

        // show_route(route);

        reset_nodes();
        reset_edges();

        info(&global_logger, "---------------------------");
    }

    // chose best route.
    int route_idx = get_best_route(c);
    show_route(c->routes[route_idx]);

    free_controller(c);
    c = NULL;

    free_edges();
    free_nodes();

    info(&global_logger, "-----------[END]-----------");
    return 0;
}
