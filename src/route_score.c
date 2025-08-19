#include "route_score.h"
#include "logger.h"

#include <stdlib.h>

// NOTE: tmp.
static double calc_distance(const double total_distance) {
    if (total_distance == 2000.0) {
        return 10.0;
    } else {
        return 0.0;
    }
}

void calc_route_score(RoutePtr route) {
    if (route == NULL) {
        fetal(&global_logger, "calc_route_score: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    double score = 0.0;
    score += calc_distance(route->total_distance);

    route->total_score = score;
}
