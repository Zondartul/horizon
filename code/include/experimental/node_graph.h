#ifndef NODE_GRAPH_GUARD
#define NODE_GRAPH_GUARD
#include "vec.h"
#include "globals.h"
class entity;

void generateNodegraph();
struct nav_node;
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

extern pathTaskKind g_pathTask;

typedef vector<nav_node*> nav_path;
typedef nav_path empty_path;

void AStarInit(pathTaskKind *task, nav_node *src, nav_node *dest);
void AStarIterative(pathTaskKind *task);
nav_path AStar(vec3 src, vec3 dest);

#endif // NODE_GRAPH_GUARD
