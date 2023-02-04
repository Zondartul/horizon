#include <sstream>
#include <stdexcept>
#include <iostream>
#include "program/file.h"
#include "render/GPUdriverSoft.h"
#include "resource/texture.h"
#include "render/printw.h"
#include "util/util.h"
//#include "util/debug.h"
#include "util/globals_render.h"
//#include "util/stringUtils.h" /// to print vec2 and stuff
#include "render/rmodel.h"
using namespace std;

//GPUdriverSoft::~GPUdriverSoft(){}

#define rs stateStack.state

void GPUdriverSoft::command_layer(renderLayer* layer) {
}
void GPUdriverSoft::command_coloring(bool b) {
    rs.coloring = b;
}
void GPUdriverSoft::command_uvcoloring(bool b) {
    rs.uvcoloring = b;
}
void GPUdriverSoft::command_normalcoloring(bool b) {
    rs.normalcoloring = b;
}
void GPUdriverSoft::command_transparency(bool b) {
    rs.transparency = b;
}
void GPUdriverSoft::command_depthmask(bool b) {
    rs.depthmask = b;
}
void GPUdriverSoft::command_texturing(bool b) {
    rs.texturing = b;
}
void GPUdriverSoft::command_debugging(bool b) {
    rs.debugging = b;
}
void GPUdriverSoft::command_projection(camprojection* cam) {
    rs.VP = cam->MVP;
    rs.camPos = cam->pos;
    //delete rcmd.cam; owned by command now.
    reproject();
}
void GPUdriverSoft::command_position(vec3 v) {
    rs.pos = v;
    reproject();
}
void GPUdriverSoft::command_rotation(vec3 v) {
    rs.rot = v;
}
void GPUdriverSoft::command_scale(vec3 v) {
    rs.scale = v;
}
void GPUdriverSoft::command_texture_upload(texture* t) {
}
void GPUdriverSoft::command_texture_select(texture* t) {
    rs.cur_texture = t;
}
void GPUdriverSoft::command_texture_pos(vec2 v) {
    rs.texture_pos = v;
}
void GPUdriverSoft::command_texture_scale(vec2 v) {
    rs.texture_scale = v; 
}
void GPUdriverSoft::command_rmodel_upload(rmodel* rm) {
}
void GPUdriverSoft::command_rmodel_render(rmodel* rm) {
}
void GPUdriverSoft::command_rmodel_delete(rmodel* rm) {
}
void GPUdriverSoft::command_clear_screen() {
}
void GPUdriverSoft::command_scissoring(bool b) {
    rs.scissoring = b;
}
void GPUdriverSoft::command_color(vec3 v) {
    rs.color = v;
}
void GPUdriverSoft::command_alpha(float f) {
    rs.alpha = f;
}
void GPUdriverSoft::command_font_select(font* fnt) {
    //if (!fnt) {
    //    //error("attempt to select null font\n");
    //    throw runtime_error("attempt to select null font");
    //}
    rs.fnt = fnt;
}
void GPUdriverSoft::command_mode_select(int i) {
    rs.renderMode = i;
}
void GPUdriverSoft::command_text_pos(vec2 v) {
    rs.textPos = v;
}
void GPUdriverSoft::command_scissor(rect r) {
    rs.scissor = r;
}
void GPUdriverSoft::command_pointsize(float f) {
    rs.pointsize = f;
}
void GPUdriverSoft::command_linewidth(float f) {
    rs.linewidth = f;
}
void GPUdriverSoft::command_print_text(string* S) {
}
void GPUdriverSoft::command_depth_test(bool b) {
    rs.depth_test = b;
}
void GPUdriverSoft::command_lighting(bool b) {
    rs.lighting = b;
}
void GPUdriverSoft::command_sun_pos(vec3 v) {
    rs.sun_pos = v;
}
void GPUdriverSoft::command_sun_light_color(vec3 v) {
    rs.sun_light_color = v;
}
void GPUdriverSoft::command_ambient_light_color(vec3 v) {
    rs.ambient_light_color = v;
}
void GPUdriverSoft::command_read_pixels(void* buff) {
}
void GPUdriverSoft::command_viewport(rect r) {
    rs.viewport = r;
}
void GPUdriverSoft::command_face_culling(bool b) {
    rs.face_culling = b;
}
void GPUdriverSoft::command_face_cull_ccw() {}

void GPUdriverSoft::command_push_options() {
    stateStack.push();
}
void GPUdriverSoft::command_pop_options() {
    stateStack.pop(*this);
}

void GPUdriverSoft::command_reset_options() {
    stateStack.reset(*this);
}

#undef rs


void GPUdriverSoft::command_print_options() {
    stateStack.print();
}