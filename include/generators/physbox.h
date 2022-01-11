#ifndef PHYSBOX_GUARD
#define PHYSBOX_GUARD
#include "vec.h"
#include <string>
using std::string;
struct entity;
struct texture;

entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider=true,bool is_static=true);
void wall(vec3 start, vec3 end,float scale = 0.5f);
void floor(vec3 start, vec3 end);
void physbox(string tex, float size, float mass, float bouncyness, float friction);
void makeScene1(vec3 offset = vec3(0,0,0));
void makeScene2helper(vec2 start, vec2 size);
void makeScene2(vec3 offset = vec3(0,0,0));
void obelisk();

#endif