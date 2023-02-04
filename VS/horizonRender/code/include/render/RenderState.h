#pragma once
#include "util/globals_render.h" /// for DLLAPI_RENDER
#include "resource/fonts.h"

struct DLLAPI_RENDER GPUdriver;

struct DLLAPI_RENDER RenderState {
	RenderState();
	void apply(GPUdriver& driver);

	int renderMode = 0; /// 1 - points, 2 - edges, 3 - triangles
	font* fnt = 0;
	vec2 textPos = vec2(0, 0);
	vec3 pos = vec3(0, 0, 0);
	vec3 rot = vec3(0, 0, 0);
	vec3 scale = vec3(1, 1, 1);
	mat4 VP;
	vec3 camPos = vec3(0, 0, 0);
	int height = 0;
	int width = 0;
	/// 28.01.2023: dealing with [bug 13]
	bool coloring = false;
	bool uvcoloring = false;
	bool normalcoloring = false;
	bool transparency = false;
	bool depthmask = false;
	bool texturing = false;
	bool debugging = false;
	vec2 texture_pos = vec2(0, 0);
	vec2 texture_scale = vec2(1, 1);
	bool scissoring = false;
	rect scissor = rect(vec2(0, 0));
	vec3 color = vec3(0, 0, 0);
	float alpha = 1.0;
	float pointsize = 1.0;
	float linewidth = 1.0;
	bool depth_test = true;
	bool lighting = false;
	vec3 sun_pos = vec3(0, 0, 1);
	vec3 sun_light_color = vec3(1, 1, 1);
	vec3 ambient_light_color = vec3(1, 1, 1);
	rect viewport = rect(vec2(0, 0));
	bool face_culling = true;
	texture* cur_texture = nullptr;
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


