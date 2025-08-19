#include "edge_score.h"

#include <stdlib.h>

#include "logger.h"

static double calc_unused_node_score(NodePtr node) {
    if (node == NULL) {
        fetal(&global_logger, "calc_unused_node_score: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    return node->is_used == 0 ? 10 : -10;
}

static double calc_edge_direction_score(const WalkState *state, EdgePtr edge) {
    if (state == NULL || edge == NULL) {
        fetal(&global_logger, "calc_edge_direction_score: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    return state->prev_direction == edge->direction ? 10 : 0;
}

void calc_edge_score(EdgeScore *scores, EdgeList edges, const int *candidates, const int c_count,
                     const WalkState *state) {
    if (scores == NULL || edges == NULL || candidates == NULL || state == NULL) {
        fetal(&global_logger, "calc_edge_score: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < c_count; i++) {
        int idx = candidates[i];
        double s = 0.0;

        s += calc_unused_node_score(edges[idx]->to_node);
        s += calc_edge_direction_score(state, edges[idx]);

        // NOTE: scores idx is the same as edges idx.
        scores[idx].score = s;
    }
}

int get_idx(EdgeScore *scores, const int *candidates, const int c_count) {
    if (scores == NULL || candidates == NULL) {
        fetal(&global_logger, "get_idx: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    double tmpmax = 0.0;
    double tmpmin = 0.0;
    for (int i = 0; i < c_count; i++) {
        int idx = candidates[i];
        if (scores[idx].score > tmpmax) {
            tmpmax = scores[idx].score;
        }
        // NOTE: min is unused.
        if (scores[idx].score < tmpmin) {
            tmpmin = scores[idx].score;
        }
    }

    // Get same score nodes.
    int scoreCandidates[MAX_EDGES_PER_NODE];
    int score_c_count = 0;
    for (int i = 0; i < c_count; i++) {
        int idx = candidates[i];
        if (scores[idx].score == tmpmax) {
            scoreCandidates[score_c_count++] = idx;
        }
    }

    if (score_c_count == 0) {
        return -1;
    }

    // NOTE: If same score nodes exists, get random one.

    // printf("Max: %f, Min: %f, SameScoreCount: %d\n", tmpmax, tmpmin, score_c_count);
    int ni = rand() % score_c_count;
    // printf("Next Score Candidate Index: %d\n", ni);
    int selectedIdx = scoreCandidates[ni];

    return selectedIdx;
}
