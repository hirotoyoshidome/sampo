#ifndef SAMPO_GRAPH_H
#define SAMPO_GRAPH_H

#include "poi.h"

#define MAX_WIDTH 10
#define MAX_HEIGHT 8
#define NODE_COUNT (MAX_WIDTH * MAX_HEIGHT)
#define MAX_EDGES_PER_NODE 4 // EdgeDirection

extern int NODE_LAYOUT[MAX_HEIGHT][MAX_WIDTH];

typedef struct Node Node;

typedef Node *NodePtr;

typedef enum { UP, RIGHT, DOWN, LEFT } EdgeDirection;

typedef struct {
    NodePtr to_node;
    double length;           // distance
    EdgeDirection direction; // direction
    int is_used;             // 1: used, 0: unused

    PoiList poi_list; // pointer + realloc
    int poi_counter;
    int poi_capacity;
} Edge;

typedef Edge *EdgePtr;

typedef EdgePtr *EdgeList;

// NOTE: unused
typedef enum { NORMAL, WIDE, NARROW } NodeType;

struct Node {
    int id;
    int x, y;
    NodeType type;
    int edge_count;
    int is_used; // 1: used, 0: unused

    EdgePtr edges[]; // FAM
};

extern NodePtr nodes[NODE_COUNT];

void init_nodes();
void init_edges();

void add_poi_to_edge(EdgePtr edge, PoiPtr poi);

void reset_nodes();
void reset_edges();

void free_edges();
void free_nodes();

#endif // SAMPO_GRAPH_H
