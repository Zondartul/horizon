#pragma once
#include <map>
#include <vector>
void deleteDynamicBodies();
void spawnPhysbox1();
void spawnFancyBox();
void spawnPhysbox2();
void spawnTree();
void addKeybinds();
void test1render();
void addConsoleCommands();
//struct gs_commandsKind {
	//map<const char*, struct_alloc_file> g_prev_allocation_map;
	//int g_memreport_last_frame = 0;
	//int g_prev_total_size = 0;
	//bool g_has_prev_alloc_map = 0;
//};

class GUIbase;

struct gs_commandsKind {
	std::vector<GUIbase*> testWindows;
};
