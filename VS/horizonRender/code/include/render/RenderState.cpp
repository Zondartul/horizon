#include <iostream>
#include "render/RenderState.h"
#include "render/GPUdriver.h"
using namespace std;

/// ----------- RenderState -------------------

RenderState::RenderState() {
	renderMode = 3; // 0;
	//fnt = 0;
	//textPos = vec2(0, 0);
	pos = vec3(0, 0, 0);
	rot = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);
	//VP;
	camPos = vec3(0, 0, 0);
	height = 0;
	width = 0;
	/// 28.01.2023: dealing with [bug 13]
	/// 11.02.2023: trying to change a bunch of stuff for [bug 16]
	coloring = false;
	uvcoloring = false;
	normalcoloring = false;
	transparency = false;
	depthmask = false;
	texturing = false;
	debugging = false;
	texture_pos = vec2(0, 0);
	texture_scale = vec2(1, 1);
	scissoring = false;
	scissor = rect(vec2(0, 0));
	color = vec3(0, 0, 0);
	alpha = 1.0;
	pointsize = 1.0;
	linewidth = 1.0;
	depth_test = true;
	lighting = false;
	sun_pos = vec3(0, 0, 1);
	sun_light_color = vec3(1, 1, 1);
	ambient_light_color = vec3(1, 1, 1);
	viewport = rect(vec2(0, 0));
	face_culling = true;
	cur_texture = nullptr;
}

void RenderState::apply(GPUdriver& gpu) {
	gpu.command_mode_select(renderMode);
	//gpu.command_font_select(fnt);
	//gpu.command_text_pos(textPos);
	gpu.command_position(pos);
	gpu.command_rotation(rot);
	gpu.command_scale(scale);

	{
		camprojection cam;
		cam.MVP = VP;
		cam.pos = camPos;
		gpu.command_projection(&cam);
	}

	/// ignore height, width
	/// now add all the extra stuff circa [bug 13]
	gpu.command_coloring(coloring);
	gpu.command_uvcoloring(uvcoloring);
	gpu.command_normalcoloring(normalcoloring);
	gpu.command_transparency(transparency);
	gpu.command_depthmask(depthmask);
	gpu.command_texturing(texturing);
	gpu.command_debugging(debugging);
	gpu.command_texture_pos(texture_pos);
	gpu.command_texture_scale(texture_scale);
	gpu.command_scissoring(scissoring);
	gpu.command_scissor(scissor);
	gpu.command_color(color);
	gpu.command_alpha(alpha);
	gpu.command_pointsize(pointsize);
	gpu.command_linewidth(linewidth);
	gpu.command_depth_test(depth_test);
	gpu.command_lighting(lighting);
	gpu.command_sun_pos(sun_pos);
	gpu.command_sun_light_color(sun_light_color);
	gpu.command_ambient_light_color(ambient_light_color);
	gpu.command_viewport(viewport);
	gpu.command_face_culling(face_culling);
	gpu.command_texture_select(cur_texture);
}

/// ------- RenderStateStack ---------------------

void RenderStateStack::apply(GPUdriver& gpu) {
	state.apply(gpu);
}

void RenderStateStack::apply(GPUdriver& gpu, RenderState rs) {
	rs.apply(gpu);
	/// end
	state = rs;
}

void RenderStateStack::reset(GPUdriver& gpu) {
	RenderState rs{};
	apply(gpu, rs);
}

void RenderStateStack::push() {
	stack.push_back(state);
}

void RenderStateStack::pop(GPUdriver& gpu) {
	assert(stack.size());
	RenderState& rs = stack.back();
	rs.apply(gpu);
	state = rs;
	stack.pop_back();
}

void RenderStateStack::print() {
	cout << "--------------------" << endl;
	cout << "current rs:" << endl;
	cout << state << endl;

	cout << "---- diff: ---------" << endl;
	cout << RenderStateDiff{ prevState, state };
	prevState = state;
	cout << "--------------------" << endl;
}


std::ostream& operator<<(std::ostream& stream, RenderState rs) {
#define Pr(x) stream << #x << ": " << toString(rs.x) << endl;
	Pr(renderMode); //Pr(fnt); Pr(textPos); 
	Pr(pos); Pr(rot);
	Pr(scale); Pr(VP); Pr(camPos); Pr(height); Pr(width);
	Pr(coloring); Pr(uvcoloring); Pr(normalcoloring);
	Pr(transparency); Pr(depthmask); Pr(texturing); Pr(debugging);
	Pr(texture_pos); Pr(texture_scale); Pr(scissoring);
	Pr(color); Pr(alpha); Pr(pointsize); Pr(linewidth);
	Pr(depth_test); Pr(lighting); Pr(sun_pos);
	Pr(sun_light_color); Pr(ambient_light_color);
	Pr(viewport); Pr(face_culling); Pr(cur_texture);
#undef Pr
	return stream;
}

std::ostream& operator<<(std::ostream& stream, RenderStateDiff diff) {
#define Pr(x) if(!(diff.rs1.x == diff.rs2.x)){stream << #x << ": " << toString(diff.rs1.x) << " -> " << toString(diff.rs2.x) << endl;}
	Pr(renderMode); //Pr(fnt); Pr(textPos); 
	Pr(pos); Pr(rot);
	Pr(scale); Pr(VP); Pr(camPos); Pr(height); Pr(width);
	Pr(coloring); Pr(uvcoloring); Pr(normalcoloring);
	Pr(transparency); Pr(depthmask); Pr(texturing); Pr(debugging);
	Pr(texture_pos); Pr(texture_scale); Pr(scissoring);
	Pr(color); Pr(alpha); Pr(pointsize); Pr(linewidth);
	Pr(depth_test); Pr(lighting); Pr(sun_pos);
	Pr(sun_light_color); Pr(ambient_light_color);
	Pr(viewport); Pr(face_culling); Pr(cur_texture);
#undef Pr
	return stream;
}