#include "route.h"
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

RoutePtr init_route() {
    RoutePtr route = (RoutePtr)malloc(sizeof(Route));
    if (route == NULL) {
        fetal(&global_logger, "calc_route_score: Route Initialization Failed");
        exit(EXIT_FAILURE);
    }

    Routing routing = (Routing)malloc(sizeof(unsigned int) * DEFAULT_ROUTING_SIZE);
    if (routing == NULL) {
        fetal(&global_logger, "calc_route_score: Routing Initialization Failed");
        exit(EXIT_FAILURE);
    }

    route->routing = routing;

    route->total_score = 0.0;
    route->total_distance = 0.0;
    route->turn_count = 0;
    route->route_step = 0;

    return route;
}

void free_route(RoutePtr route) {
    if (route == NULL) {
        fetal(&global_logger, "free_route: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    free(route->routing);
    route->routing = NULL;
    free(route);
    route = NULL;
}

// TODO: Fix it to output Edges.
void show_route(const RoutePtr route) {
    if (route == NULL) {
        fetal(&global_logger, "show_route: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    // Generate whole array.(0 padding)
    int node_map[MAX_HEIGHT][MAX_WIDTH];
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            node_map[i][j] = -1;
        }
    }

    // mapping using used nodes.
    int counter = 0;
    for (size_t x = 0; x < route->route_step; x++) {
        NodePtr node = nodes[route->routing[x]];
        node_map[node->y][node->x] = counter++;
    }

    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            char mark[16] = "  *";
            // NOTE: Not Exists node pattern.
            if (NODE_LAYOUT[i][j] == 0) {
                snprintf(mark, sizeof(mark), "   ");
            }
            if (node_map[i][j] != -1) {
                snprintf(mark, sizeof(mark), "%3d", node_map[i][j]);
            }
            printf("%s ", mark);
        }

        printf("\n");
    }

    NodePtr start_node = route->start_node;
    printf("Start Node(x,y) : (%d, %d)\n", start_node->x, start_node->y);
    printf("Total Score : %.2f\n", route->total_score);
    printf("Total Distance : %.2f\n", route->total_distance);
    printf("Turn Count : %d\n", route->turn_count);
}
