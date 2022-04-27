#ifndef PHYSBOX_GUARD
#define PHYSBOX_GUARD
#include "vec.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
class entity;
struct texture; 
class entity;
class timer;


entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider=true,bool is_static=true);
entity *physplane(vec3 A, vec3 B, vec3 C, texture *t, float texscale = 1.f);
void wall(vec3 start, vec3 end,float scale = 0.5f);
void floor(vec3 start, vec3 end);
entity *physbox(string tex, float size, float mass, float bouncyness, float friction);
void phystree(float mass, float bouncyness, float friction);
void makeSheet(vec3 start, vec3 end);

void makeScene1(vec3 offset = vec3(0,0,0));
void makeScene2helper(vec2 start, vec2 size);
void makeScene2(vec3 offset = vec3(0,0,0));
void makeScene3(vec3 offset = vec3(0,0,0));
void obelisk();
void spawnFlag(vec3 pos);

enum mptype {PRIM_BOX, PRIM_CYL, PRIM_CONE, PRIM_SPHERE};
struct physprim {mptype type; string texname; vec3 pos; float args[4];};

void physcombo(vector<physprim> combo);


struct gs_physboxKind {
	entity* g_ent_flag = 0;
	timer* g_timer_flag = 0;
};

#endif
