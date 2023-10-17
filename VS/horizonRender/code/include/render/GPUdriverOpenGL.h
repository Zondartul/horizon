#pragma once
#include <map>
#include "render/GPUdriver.h"
#include "render/RenderState.h"
#include "resource/bitmap.h" /// for pixelFormat

#ifdef USE_GLEW
	#include "Ext/GL/glew.h"
#endif

class DLLAPI_RENDER GPUdriverOpenGL : public GPUdriver {
	RenderStateStack stateStack;

	int numVerts = 0;
	int num_textures = 0;
	long bytes_textures = 0;
	int num_rmodels = 0;
	long bytes_rmodels = 0;
	bool debug = false;

#ifndef NO_GLEW
	GLuint vaoHandle = 0;
	GLuint positionBufferHandle = 0;
	GLuint colorBufferHandle = 0;
	GLuint normalBufferHandle = 0;
	GLuint uvBufferHandle = 0;
	std::map<string, int> locations;
	std::map<bitmap*, GLuint> bitmap_GPU_handles;
	std::map<texture*, GLuint> texture_GPU_handles;
	std::map<rmodel*, GLuint[4]> rmodel_GPU_handles;
#endif

	void renderLowInit();
	void parseQueue(renderQueue3* rqueue);
	void command_layer(renderLayer* layer);
	void command_coloring(bool b);
	void command_uvcoloring(bool b);
	void command_normalcoloring(bool b);
	void command_transparency(bool b);
	void command_depthmask(bool b);
	void command_texturing(bool b);
	void command_debugging(bool b);
	void command_projection(camprojection* cam);
	void command_position(vec3 v);
	void command_rotation(vec3 v);
	void command_scale(vec3 v);
	void command_texture_upload(texture* t);
	void command_texture_select(texture* t);
	void command_texture_pos(vec2 v);
	void command_texture_scale(vec2 v);
	void command_rmodel_upload(rmodel* rm);
	void command_clear_screen();
	void command_scissoring(bool b);
	void command_color(vec3 v);
	void command_alpha(float f);
	//void command_font_select(font* fnt);
	void command_mode_select(int i);
	//void command_text_pos(vec2 v);
	void command_scissor(rect r);
	void command_pointsize(float f);
	void command_rmodel_render(rmodel* rm);
	void command_rmodel_delete(rmodel* rm);
	void command_linewidth(float f);
	void command_print_text(string* S);
	void command_depth_test(bool b);
	void command_lighting(bool b);
	void command_sun_pos(vec3 v);
	void command_sun_light_color(vec3 v);
	void command_ambient_light_color(vec3 v);
	void command_read_pixels(void* buff);
	void command_viewport(rect r);
	void command_face_culling(bool b);
	void command_face_cull_ccw();
	void command_push_options();
	void command_pop_options();
	void command_reset_options();
	void command_print_options();
	void parseCommand(const renderCommand3& rcmd);
	//void resetRenderState();
	//void applyRenderState(RenderState& state);
	//void projectionToCamera();
	void reproject();
	void setViewportSize(int width, int height);

#ifndef NO_GLEW
	GLuint uploadShader(GLuint shaderType, const char* filepath);
	GLuint uploadShaderProgram(const char* filepath_vertex, const char* filepath_fragment);
#endif
};

#ifndef NO_GLEW
GLenum pixelFormatToGL(pixelFormat F);
#endif
