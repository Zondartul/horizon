#include "node_graph.h"
#include "game.h"
#include "timer.h"
nav_node *putNode(vec3 pos, string name = "nav_node");

struct nodegraphKind{
    int numX;
    int numY;
    vector<nav_node*> nodes;
    renderLayer *layer;
    rmodel *rm;
} nodegraph;

void generateNodegraphFinalize();


void generateNodegraph(){
    printf("generating node graph\n");
    collisioninfo *col = 0;
    col = raytrace(g_camera.pos,vec3(0,0,-1));
    if(!col){printf("no ground!\n"); return;}
    vec3 origin = col->c_to_c.pos+vec3(0,0,0.2f);


    float sidelen = 1.5f;
    nodegraph.numX = 12;
    nodegraph.numY = 12;
    nodegraph.layer = new renderLayer("layerNodegraph", true, false);
    addLayer(g_layer3D, nodegraph.layer); //i cba set up the layers properly
    vec3 corner = origin+vec3(-(nodegraph.numX-1)/2.f,-(nodegraph.numY-1)/2.f,0)*sidelen;

    nodegraph.rm = new rmodel();
    for(int Ix = 0; Ix < nodegraph.numX; Ix++){
        for(int Iy = 0; Iy < nodegraph.numY; Iy++){
            string name = fstring("nav_node_%d_%d",Ix,Iy);
            nav_node *N = putNode(corner+vec3(Ix,Iy,0)*sidelen,name);
            nodegraph.nodes.push_back(N);
        }
    }
    printf("done putting nodes\n");

    simpletimer([=](timer *T){generateNodegraphFinalize();}, 2); //call this function in 2 ticks (after collisions are done)
}

nav_edge::nav_edge(nav_node *N, bool a){node = N; accessible = a;}

enum LOSresult{ LOS_OBSTACLE = 0,
                LOS_DIRECT = 1,
                LOS_NOTHING = 2,
                LOS_YES = LOS_DIRECT,
                LOS_NO = LOS_NOTHING | LOS_OBSTACLE,
                LOS_NO_OBSTACLE = LOS_NOTHING | LOS_DIRECT};

                // maybe keep this as yes/no and change ray-point to epsilon-based
                // or change nodes to small cubes
LOSresult hasLineOfSight(entity *E1, entity *E2){
    if(!(E1->body) || !(E2->body)){return LOS_DIRECT;}
    vec3 p1 = E1->body->pos;
    vec3 p2 = E2->body->pos;
    collisioninfo *col = raytrace(p1,p2-p1,{E1});
    if(!col){
        return LOS_NOTHING;
    }
    if(col->body2->E == E2){
        return LOS_DIRECT;
    }else{
        vec3 p3 = col->c_to_c.pos;
        if(length(p2-p1) < length(p3-p1)){
            return LOS_NOTHING;
        }
        //else we hit something, but it wasn't what we're looking for
    }
    return LOS_OBSTACLE;
}

void generateNodegraphFinalize(){
    for(int Ix = 0; Ix < nodegraph.numX; Ix++){
        for(int Iy = 0; Iy < nodegraph.numY; Iy++){
            nav_node *O = nodegraph.nodes[Ix+Iy*nodegraph.numX];
            bool hasUp = Iy > 0;
            bool hasDn = (Iy+1)<nodegraph.numY;
            bool hasLt = Ix > 0;
            bool hasRt = (Ix+1)<nodegraph.numX;
            nav_node *N = (hasUp)? nodegraph.nodes[Ix+(Iy-1)*nodegraph.numX] : 0;
            nav_node *NW = (hasUp && hasLt)? nodegraph.nodes[(Ix-1)+(Iy-1)*nodegraph.numX] : 0;
            nav_node *W = (hasLt)? nodegraph.nodes[(Ix-1)+Iy*nodegraph.numX] : 0;
            nav_node *SW = (hasDn && hasLt)? nodegraph.nodes[(Ix-1)+(Iy+1)*nodegraph.numX] : 0;
            nav_node *S = (hasDn)? nodegraph.nodes[Ix+(Iy+1)*nodegraph.numX] : 0;
            nav_node *SE = (hasDn && hasRt)? nodegraph.nodes[(Ix+1)+(Iy+1)*nodegraph.numX] : 0;
            nav_node *E = (hasRt)? nodegraph.nodes[(Ix+1)+Iy*nodegraph.numX] : 0;
            nav_node *NE = (hasUp && hasRt)? nodegraph.nodes[(Ix+1)+(Iy-1)*nodegraph.numX] : 0;

            O->neighbors[0] = nav_edge(N,  (N  && N->valid  && (hasLineOfSight(O->E, N->E ) & LOS_NO_OBSTACLE)));
            O->neighbors[1] = nav_edge(NW, (NW && NW->valid && (hasLineOfSight(O->E, NW->E) & LOS_NO_OBSTACLE)));
            O->neighbors[2] = nav_edge(W,  (W  && W->valid  && (hasLineOfSight(O->E, W->E ) & LOS_NO_OBSTACLE)));
            O->neighbors[3] = nav_edge(SW, (SW && SW->valid && (hasLineOfSight(O->E, SW->E) & LOS_NO_OBSTACLE)));
            O->neighbors[4] = nav_edge(S,  (S  && S->valid  && (hasLineOfSight(O->E, S->E ) & LOS_NO_OBSTACLE)));
            O->neighbors[5] = nav_edge(SE, (SE && SE->valid && (hasLineOfSight(O->E, SE->E) & LOS_NO_OBSTACLE)));
            O->neighbors[6] = nav_edge(E,  (E  && E->valid  && (hasLineOfSight(O->E, E->E ) & LOS_NO_OBSTACLE)));
            O->neighbors[7] = nav_edge(NE, (NE && NE->valid && (hasLineOfSight(O->E, NE->E) & LOS_NO_OBSTACLE)));
            O->drawLines();
        }
    }
    //now disable collisions (this will also disable raytraces)
        for(int Ix = 0; Ix < nodegraph.numX; Ix++){
        for(int Iy = 0; Iy < nodegraph.numY; Iy++){
            nav_node *O = nodegraph.nodes[Ix+Iy*nodegraph.numX];
            O->E->body->type = BODY_NOCOLLIDE;
        }
    }
    //setLayer(loadLayer);
    nodegraph.rm->finalize();
    setLayer(g_loadLayer);
    uploadRmodel(nodegraph.rm);
    setLayer(nodegraph.layer);
    setRenderMode(2);
    setPosition(vec3(0,0,0));
    setScale(vec3(1,1,1));
    setRotation(vec3(0,0,0));
    setTexturing(false);
    setLighting(false);
    setAlpha(255);
    setColor(vec3(0,255,0));
    drawRmodel(nodegraph.rm);

    printf("nodes finalized\n");
}

void nav_node::drawLines(){
    if(!valid){return;}
    for(int i = 0; i < 8; i++){
        nav_edge e = neighbors[i];
        if(e.accessible){

            vec3 p1 = E->body->pos;
            vec3 p2 = e.node->E->body->pos;
            nodegraph.rm->vertices->push_back(p1);
            nodegraph.rm->vertices->push_back(p2);
        }
    }
}

nav_node *putNode(vec3 pos, string name){
    entity *E = new entity();
    E->name = name;

    nav_node *N = new nav_node;
    N->E = E;

    float r = 0.1f;
    collisionbody *body = new collisionbodyAABB(AABB(-vec3(r,r,r),vec3(r,r,r)));

    body->type = BODY_TRIGGER;
    body->E = E;
    body->onCollision = [=](collisioninfo *col)
    {
        if(col->body1->type == BODY_STATIC){
            N->valid = false;
            N->E->r->color = vec3(255,0,0);
        }else{
        }
    };
    E->body = body;


    renderableSprite *ren = new renderableSprite();
    ren->t = getTexture("gui/footprint_right");
    vec2 spriteSize = ren->t->size();
    ren->R = rect(-spriteSize/2.f, spriteSize/2.f).moveBy(vec2(0,-spriteSize.y/2.f));
    E->r = ren;

    E->setPosition(pos);
    addEntity(E);
    return N;
}

float nodeDist(nav_node *n, vec3 pos){return length(n->E->body->pos - pos);}
float nodeDist(nav_node *n1, nav_node *n2){return length(n1->E->body->pos-n2->E->body->pos);}

nav_node *findClosestNode(vec3 pos){
    if(!nodegraph.nodes.size()){return 0;}
    auto &n = nodegraph.nodes;
    nav_node *bestNode = n[0];
    float bestDist = nodeDist(n[0],pos);
    for(int I = 0; I < n.size(); I++){
        float dist = nodeDist(n[I],pos);
        if(dist < bestDist){bestDist = dist; bestNode = n[I];}
    }
    return bestNode;
}

extern entity *g_ent_flag;
timer *g_pathfinding_test_timer = 0;



void pathfinding_test(vec3 pos){
    if(!nodegraph.nodes.size()){printf("no node graph\n"); return;}
    if(!g_ent_flag){printf("no flag\n"); return;}
    //reset color
    for(int I = 0; I < nodegraph.nodes.size(); I++){
        nodegraph.nodes[I]->E->r->color = vec3(255,255,255);
    }

    //begin

    vec3 flagPos = g_ent_flag->body->pos;
    nav_node *flagNode = findClosestNode(flagPos);
    if(!flagNode){printf("no dest node\n"); return;}
    flagNode->E->r->color = vec3(0,255,255);

    nav_node *srcNode = findClosestNode(pos);
    if(!srcNode){printf("no src node\n"); return;}
    srcNode->E->r->color = vec3(0,0,255);

    AStarInit(&g_pathTask, srcNode, flagNode);


    if(g_pathfinding_test_timer){delete g_pathfinding_test_timer; g_pathfinding_test_timer = 0;}
    timer *T = new timer(0,40,1,0,0);
    g_pathfinding_test_timer = T;
    T->F = [&](timer *T){
        AStarIterative(&g_pathTask);
        if(g_pathTask.done){
            printf("A* DONE\n");
            delete T;
            g_pathfinding_test_timer = 0;
        }
    };
    T->run = true;


}

void AStarInit(pathTaskKind *task, nav_node *srcNode, nav_node *flagNode){
    printf("A* init:\ntask = %p\n",task);
    task->source = srcNode;
    task->dest = flagNode;
    task->node_data.clear();
    task->openSet.clear();
    task->closedSet.clear();
    task->path.clear();
    task->done = false;

    task->openSet.push_back(srcNode);
    printf("srcNode = %p\n",srcNode);
    printf("node_data contents before:\n");
    for(auto I = task->node_data.begin(); I != task->node_data.end(); I++){printf("%p:{%d,%p}\n",I->first, I->second.cost, I->second.parent);}
    task->node_data[srcNode] = {0, 0};
    printf("node_data contents after:\n");
    for(auto I = task->node_data.begin(); I != task->node_data.end(); I++){printf("%p:{%d,%p}\n",I->first, I->second.cost, I->second.parent);}
}

void AStarIterative(pathTaskKind *task){
    if(task->openSet.size() == 0){task->done = true; return;}
    //for every node in the open set
    int loopCount = task->openSet.size();
    for(auto I = task->openSet.begin(); I != task->openSet.end();){
        if(!loopCount--){return;}
        nav_node *n_open = *I;
        if(!task->node_data.count(n_open)){error("node_data doesn't contain the open node\n");}
        float cost = task->node_data[n_open].cost;

        if(n_open != task->dest){

            //check all connected nodes
            for(int J = 0; J < 8; J++){
                nav_edge edge = n_open->neighbors[J];
                if(!edge.accessible){continue;} //path blocked
                nav_node *n_neigh = edge.node;
                if(!n_neigh){continue;} //no neighbor condition
                if(task->closedSet.count(n_neigh)){continue;} //node already closed condition
                if(task->node_data.count(n_neigh)){//node already open
                    if(task->node_data[n_neigh].cost <= cost+1){continue;} //at same or better cost
                }
                task->node_data[n_neigh] = {cost+1,n_open};
                task->openSet.push_back(n_neigh);

                if((n_neigh != task->source) && (n_neigh != task->dest)){
                    n_neigh->E->r->color = vec3(255,255,0);
                }
            }
            //close this node
            nav_node *n_closed = n_open;
            task->closedSet.insert(n_closed);
            I = task->openSet.erase(I);
            if((n_closed != task->source) && (n_closed != task->dest)){
                n_closed->E->r->color = vec3(128,128,128);
            }
        }else{
            //destination node found!
            while(n_open){
                if((n_open != task->source) && (n_open != task->dest)){
                    n_open->E->r->color = vec3(0,255,0);
                }
                task->path.insert(task->path.begin(),n_open);
                n_open = task->node_data[n_open].parent;
            }
            task->done = true;
            return;
        }
    }
}


nav_path AStar(vec3 src, vec3 dest){
    if(!nodegraph.nodes.size()){printf("no node graph\n"); return empty_path();}
    //reset color
    for(int I = 0; I < nodegraph.nodes.size(); I++){
        nodegraph.nodes[I]->E->r->color = vec3(255,255,255);
    }

    //begin

    nav_node *destNode = findClosestNode(dest);
    if(!destNode){printf("no dest node\n"); return empty_path();}
    destNode->E->r->color = vec3(0,255,255);

    nav_node *srcNode = findClosestNode(src);
    if(!srcNode){printf("no src node\n"); return empty_path();}
    srcNode->E->r->color = vec3(0,0,255);

    pathTaskKind *task = &g_pathTask;
    AStarInit(task, srcNode, destNode);
    while(!task->done){AStarIterative(task);}
    return task->path;
}

pathTaskKind g_pathTask;

