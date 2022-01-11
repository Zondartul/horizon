#ifndef MAIN_GUARD
#define MAIN_GUARD
#include "octree.h"
#include "GUI.h"

extern GUIbase *GUI;
extern octree_node *octree_root;
extern float frametime;
extern float fps;
extern model *m;

void tick();
void openGUI();

#endif