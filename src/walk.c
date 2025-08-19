#include "walk.h"
#include "edge_score.h"

#include "logger.h"

#include <stdio.h>
#include <stdlib.h>

// NOTE: For Debugging.
__attribute__((unused)) static void show_unused_edges(const NodePtr current, const int *candidates,
                                                      const int c_count) {
    printf("Unused EdgeCount: %d\n", c_count);
    printf("Can Move Direction > ");
    for (int i = 0; i < c_count; i++) {
        switch (current->edges[candidates[i]]->direction) {
        case UP:
            printf("UP ");
            break;
        case RIGHT:
            printf("RIGHT ");
            break;
        case DOWN:
            printf("DOWN ");
            break;
        case LEFT:
            printf("LEFT ");
            break;
        }
    }
    printf("\n");
}

// NOTE: For Debugging.
__attribute__((unused)) static void show_edge_score(const NodePtr current, const int *candidates,
                                                    const int c_count,
                                                    const EdgeScore *edge_scores) {
    printf("Direction EdgeScore > ");
    for (int i = 0; i < c_count; i++) {
        switch (current->edges[candidates[i]]->direction) {
        case UP:
            printf("UP:%f ", edge_scores[0].score);
            break;
        case RIGHT:
            printf("RIGHT:%f ", edge_scores[1].score);
            break;
        case DOWN:
            printf("DOWN:%f ", edge_scores[2].score);
            break;
        case LEFT:
            printf("LEFT:%f ", edge_scores[3].score);
            break;
        }
    }
    printf("\n");
}

void walk(RoutePtr route, NodePtr start, const double distance) {
    if (route == NULL || start == NULL) {
        fetal(&global_logger, "walk: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    WalkState state;

    NodePtr current = start;
    double total_distance = 0.0;
    int turn_count = 0;
    int nodeOrder[255];
    size_t currentNodeOrderIdx = 0;

    // printf("Start Node id: %d (%d, %d)\n\n", current->id, current->x, current->y);
    current->is_used = 1;
    nodeOrder[currentNodeOrderIdx++] = current->id;

    while (total_distance < distance) {
        // Get Unused Edges.
        int candidates[MAX_EDGES_PER_NODE];
        int c_count = 0;
        for (int i = 0; i < current->edge_count; i++) {
            if (current->edges[i]->is_used == 0) {
                candidates[c_count++] = i;
            }
        }

        if (c_count == 0) {
            info(&global_logger, "walk: Not Exists unused Edges.");
            break;
        }
        // show_unused_edges(current, candidates, c_count);

        EdgeScore edge_scores[MAX_EDGES_PER_NODE];
        calc_edge_score(edge_scores, current->edges, candidates, c_count, &state);
        // show_edge_score(current, candidates, c_count, edge_scores);

        int idx = get_idx(edge_scores, candidates, c_count);
        if (idx == -1) {
            break;
        }
        // printf("Next Edge Index: %d\n", idx);
        current->edges[idx]->is_used = 1;
        Node *next = current->edges[idx]->to_node;

        for (int i = 0; i < next->edge_count; i++) {
            if (next->edges[i]->to_node == current) {
                next->edges[i]->is_used = 1;
                break;
            }
        }

        next->is_used = 1;

        if (state.prev_direction && state.prev_direction != current->edges[idx]->direction) {
            turn_count++;
        }

        // Update State.
        state.prev_direction = current->edges[idx]->direction;

        total_distance += current->edges[idx]->length;
        current = next;
        nodeOrder[currentNodeOrderIdx++] = current->id;

        // printf("Move Node id: %d (%d, %d), CurrentDistance: %.2f\n", current->id, current->x,
        // current->y, total_distance);
    }

    route->total_distance = total_distance;
    route->turn_count = turn_count;
    for (size_t i = 0; i < currentNodeOrderIdx; i++) {
        route->routing[i] = nodeOrder[i];
    }
    route->route_step = currentNodeOrderIdx;

    char buff[255];
    snprintf(buff, sizeof(buff), "Finished! total distance: %.2f\n", total_distance);
    info(&global_logger, buff);
}
