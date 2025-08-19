#ifndef SAMPO_EDGE_SCORE_H
#define SAMPO_EDGE_SCORE_H

#include "graph.h"
#include "walk.h"

typedef struct {
    double score;
} EdgeScore;

void calc_edge_score(EdgeScore *scores, EdgeList edges, const int *candidates, const int c_count,
                     const WalkState *state);

// get index best score.(if same score index exists, chose index as random.)
int get_idx(EdgeScore *scores, const int *candidates, const int c_count);

#endif // SAMPO_EDGE_SCORE_H
