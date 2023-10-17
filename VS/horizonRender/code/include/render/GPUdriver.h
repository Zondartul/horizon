#pragma once

//#ifdef USE_GLEW
//	#include "Ext/GL/glew.h"
//#endif

//#include "resource/fonts.h"
//#include "resource/bitmap.h"
//#include "math/vec.h"
//#include "render/rmodel.h"
#include "render/renderLayer.h"
//#include "util/globals_render.h"
//#include "render/RenderState.h"
class RenderState;

class DLLAPI_RENDER GPUdriver {
public:
	GPUdriver() = default;
	virtual ~GPUdriver();

	virtual void renderLowInit() = 0;
	virtual void parseQueue(renderQueue3* rqueue) = 0;
	virtual void command_layer(renderLayer* layer) = 0;
	virtual void command_coloring(bool b) = 0;
	virtual void command_uvcoloring(bool b) = 0;
	virtual void command_normalcoloring(bool b) = 0;
	virtual void command_transparency(bool b) = 0;
	virtual void command_depthmask(bool b) = 0;
	virtual void command_texturing(bool b) = 0;
	virtual void command_debugging(bool b) = 0;
	virtual void command_projection(camprojection* cam) = 0;
	virtual void command_position(vec3 v) = 0;
	virtual void command_rotation(vec3 v) = 0;
	virtual void command_scale(vec3 v) = 0;
	virtual void command_texture_upload(texture* t) = 0;
	virtual void command_texture_select(texture* t) = 0;
	virtual void command_texture_pos(vec2 v) = 0;
	virtual void command_texture_scale(vec2 v) = 0;
	virtual void command_rmodel_upload(rmodel* rm) = 0;
	virtual void command_clear_screen() = 0;
	virtual void command_scissoring(bool b) = 0;
	virtual void command_color(vec3 v) = 0;
	virtual void command_alpha(float f) = 0;
	//virtual void command_font_select(font* fnt) = 0;
	virtual void command_mode_select(int i) = 0;
	//virtual void command_text_pos(vec2 v) = 0;
	virtual void command_scissor(rect r) = 0;
	virtual void command_pointsize(float f) = 0;
	virtual void command_rmodel_render(rmodel* rm) = 0;
	virtual void command_rmodel_delete(rmodel* rm) = 0;
	virtual void command_linewidth(float f) = 0;
	virtual void command_print_text(string* S) = 0;
	virtual void command_depth_test(bool b) = 0;
	virtual void command_lighting(bool b) = 0;
	virtual void command_sun_pos(vec3 v) = 0;
	virtual void command_sun_light_color(vec3 v) = 0;
	virtual void command_ambient_light_color(vec3 v) = 0;
	virtual void command_read_pixels(void* buff) = 0;
	virtual void command_viewport(rect r) = 0;
	virtual void command_face_culling(bool b) = 0;
	virtual void command_face_cull_ccw() = 0;
	virtual void command_push_options() = 0;
	virtual void command_pop_options() = 0;
	virtual void command_reset_options() = 0;
	virtual void command_print_options() = 0;
	
	virtual void parseCommand(const renderCommand3& rcmd) = 0;
	//virtual void resetRenderState() = 0;
	//virtual void applyRenderState(RenderState& state) = 0;
	//virtual void projectionToCamera() = 0;
	virtual void reproject() = 0;
	virtual void setViewportSize(int width, int height) = 0;
};

//void DLLAPI_RENDER applyRenderState(renderStateKind state);

