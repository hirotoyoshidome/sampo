#include "graph.h"

#include <stdlib.h>

#include "logger.h"

// Node Layout(1: exists、0: none)
int NODE_LAYOUT[MAX_HEIGHT][MAX_WIDTH] = {
    {1,1,1,1,1,1,1,1,0,1},
    {1,1,1,1,1,1,1,1,0,1},
    {1,1,1,0,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0,1,1},
    {1,1,1,1,1,1,1,0,1,1},
    {1,1,0,0,1,1,1,1,1,1}
};

NodePtr nodes[NODE_COUNT];

static NodePtr init_node() {
    // FIXME: realloc pattern.
    NodePtr p = (NodePtr)malloc(sizeof(Node) + sizeof(EdgePtr) * MAX_EDGES_PER_NODE);
    if (p == NULL) {
        fetal(&global_logger, "Node Initialization Failed");
        exit(EXIT_FAILURE);
    }
    return p;
}

static EdgePtr init_edge() {
    EdgePtr p = (EdgePtr)malloc(sizeof(Edge));
    if (p == NULL) {
        fetal(&global_logger, "Edge Initialization Failed");
        exit(EXIT_FAILURE);
    }
    return p;
}

void init_nodes() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            if (NODE_LAYOUT[h][w] == 0) {
                nodes[id] = NULL;
                continue;
            }

            NodePtr np = init_node();
            np->id = id;
            np->x = w;
            np->y = h;
            np->type = NORMAL;
            np->edge_count = 0;
            np->is_used = 0;
            nodes[id] = np;
        }
    }
}

static void add_edge(NodePtr from, NodePtr to, double length, const EdgeDirection direction) {
    if (from == NULL || to == NULL) {
        fetal(&global_logger, "add_edge: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    if (from->edge_count >= MAX_EDGES_PER_NODE) {
        return;
    }

    EdgePtr ep = init_edge();

    ep->to_node = to;
    ep->length = length;
    ep->direction = direction;
    ep->is_used = 0;

    ep->poi_list = NULL;
    ep->poi_counter = 0;
    ep->poi_capacity = 0;

    from->edges[from->edge_count++] = ep;
}

void init_edges() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            NodePtr current = nodes[id];
            if (current == NULL) {
                continue;
            }

            // TODO Node id is invalid. Caused by memoery address is changing by realloc ??
            // int edge_num = 0;
            // edge_num += (h > 0) ? 1 : 0;
            // edge_num += (w < MAX_WIDTH-1) ? 1 : 0;
            // edge_num += (h < MAX_HEIGHT-1) ? 1 : 0;
            // edge_num += (w > 0) ? 1 : 0;
            // NodePtr newcurrent = realloc(current, sizeof(Node) + sizeof(EdgePtr) * edge_num);
            // if (newcurrent == NULL) {
            //     printf("Node Initialization Failed\n");
            //     exit(EXIT_FAILURE);
            // }
            // current = NULL;
            // nodes[id] = newcurrent;
            // current = nodes[id];

            if (h > 0) {
                // UP
                NodePtr up = nodes[id - MAX_WIDTH];
                if (up != NULL) {
                    add_edge(current, up, 100, UP);
                }
            }
            if (w < MAX_WIDTH - 1) {
                // RIGHT
                NodePtr right = nodes[id + 1];
                if (right != NULL) {
                    add_edge(current, right, 100, RIGHT);
                }
            }
            if (h < MAX_HEIGHT - 1) {
                // DOWN
                NodePtr down = nodes[id + MAX_WIDTH];
                if (down != NULL) {
                    add_edge(current, down, 200, DOWN);
                }
            }
            if (w > 0) {
                // LEFT
                NodePtr left = nodes[id - 1];
                if (left != NULL) {
                    add_edge(current, left, 100, LEFT);
                }
            }
        }
    }
}

void add_poi_to_edge(EdgePtr edge, PoiPtr poi) {
    if (edge == NULL || poi == NULL) {
        fetal(&global_logger, "add_poi_to_edge: Invalid Pointer");
        exit(EXIT_FAILURE);
    }

    if (edge->poi_counter >= edge->poi_capacity) {
        int newcap = 0;
        if (edge->poi_capacity == 0) {
            newcap = 4;
        } else {
            newcap = edge->poi_capacity * 2;
        }
        PoiList newlist = realloc(edge->poi_list, newcap * sizeof(PoiPtr));
        if (newlist == NULL) {
            fetal(&global_logger, "add_poi_to_edge: POI LIST Expansion is Falied");
            exit(EXIT_FAILURE);
        }
        edge->poi_list = newlist;
        edge->poi_capacity = newcap;
    }

    edge->poi_list[edge->poi_counter++] = poi;
}

void reset_nodes() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            if (nodes[id] != NULL) {
                nodes[id]->is_used = 0;
            }
        }
    }
}

void reset_edges() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            NodePtr current = nodes[id];
            if (current != NULL) {
                for (int i = 0; i < current->edge_count; i++) {
                    current->edges[i]->is_used = 0;
                }
            }
        }
    }
}

void free_edges() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            NodePtr current = nodes[id];
            if (current != NULL) {
                for (int i = 0; i < current->edge_count; i++) {
                    if (current->edges[i]->poi_list != NULL) {
                        free_poi_list(current->edges[i]->poi_list, current->edges[i]->poi_counter);
                        current->edges[i]->poi_list = NULL;
                    }
                    free(current->edges[i]);
                    current->edges[i] = NULL;
                }
            }
        }
    }
}

void free_nodes() {
    for (int h = 0; h < MAX_HEIGHT; h++) {
        for (int w = 0; w < MAX_WIDTH; w++) {
            int id = h * MAX_WIDTH + w;
            if (nodes[id] != NULL) {
                free(nodes[id]);
                nodes[id] = NULL;
            }
        }
    }
}
