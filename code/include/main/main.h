#ifndef MAIN_GUARD
#define MAIN_GUARD
#include "octree.h"
#include "GUI.h"

extern GUIbase *g_GUI;
extern octree_node *g_octree_root;
extern float g_frametime;
extern float g_fps;
extern model *g_m;

void tick();
void openGUI();

#endif