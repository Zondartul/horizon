#ifndef NODE_GRAPH_GUARD
#define NODE_GRAPH_GUARD
#include "vec.h"
#include "globals.h"
#include "global_vars.h"
class entity;
class nav_node;
class renderLayer;
class rmodel;
class timer;

void generateNodegraph();
struct nav_edge{
    nav_node *node;
    bool accessible;
    nav_edge() = default;
    nav_edge(nav_node *N, bool a);
};

class nav_node{
    public:
    bool valid = true;
    nav_edge neighbors[8];
    entity *E = 0;
    void drawLines();
};

void pathfinding_test(vec3 pos);


struct pathData{
    float cost;
    nav_node *parent;
};

struct pathTaskKind{
    nav_node *source;
    nav_node *dest;
    map<nav_node*,pathData> node_data;
    list<nav_node*> openSet;
    set<nav_node*> closedSet;
    vector<nav_node*> path;
    bool done;
};

//extern pathTaskKind g_pathTask;

typedef vector<nav_node*> nav_path;
typedef nav_path empty_path;

void AStarInit(pathTaskKind *task, nav_node *src, nav_node *dest);
void AStarIterative(pathTaskKind *task);
nav_path AStar(vec3 src, vec3 dest);


struct nodegraphKind {
    int numX;
    int numY;
    vector<nav_node*> nodes;
    renderLayer* layer;
    rmodel* rm;
};

struct gs_node_graphKind {
    timer* g_pathfinding_test_timer = 0;
    pathTaskKind g_pathTask;
    nodegraphKind g_nodegraph;
};

#endif // NODE_GRAPH_GUARD
