#pragma once
#include "util/globals_render.h" /// for DLLAPI_RENDER
#include "resource/fonts.h"

class DLLAPI_RENDER GPUdriver;

struct DLLAPI_RENDER RenderState {
	RenderState();
	void apply(GPUdriver& driver);

	int renderMode; /// 1 - points, 2 - edges, 3 - triangles
	//font* fnt;
	//vec2 textPos;
	vec3 pos;
	vec3 rot;
	vec3 scale;
	mat4 VP;
	vec3 camPos;
	int height;
	int width;
	/// 28.01.2023: dealing with [bug 13]
	bool coloring;
	bool uvcoloring;
	bool normalcoloring;
	bool transparency;
	bool depthmask;
	bool texturing;
	bool debugging;
	vec2 texture_pos;
	vec2 texture_scale;
	bool scissoring;
	rect scissor;
	vec3 color;
	float alpha;
	float pointsize;
	float linewidth;
	bool depth_test;
	bool lighting;
	vec3 sun_pos;
	vec3 sun_light_color;
	vec3 ambient_light_color;
	rect viewport;
	bool face_culling;
	texture* cur_texture;
};

std::ostream& operator<<(std::ostream& stream, RenderState rs);

struct DLLAPI_RENDER RenderStateDiff { RenderState& rs1; RenderState& rs2; };
std::ostream& operator<<(std::ostream& stream, RenderStateDiff diff);

struct DLLAPI_RENDER RenderStateStack {
	RenderState state;
	RenderState prevState;
	vector<RenderState> stack;
	void apply(GPUdriver& gpu);
	void apply(GPUdriver& gpu, RenderState state);
	void reset(GPUdriver& gpu);
	void print();
	void push();
	void pop(GPUdriver& gpu);
};


