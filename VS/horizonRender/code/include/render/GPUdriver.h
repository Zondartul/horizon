#ifndef GPU_DRIVER_GUARD
#define GPU_DRIVER_GUARD

#ifdef USE_GLEW
	#include "Ext/GL/glew.h"
#endif

#include "resource/fonts.h"
#include "resource/bitmap.h"
#include "math/vec.h"
#include "render/rmodel.h"
#include "render/renderLayer.h"
#include "util/globals_render.h"

struct DLLAPI_RENDER renderStateKind{
	public:
	int renderMode = 0; //1 - points, 2 - edges, 3 - triangles
	font *fnt = 0;
	vec2 textPos = vec2(0,0);
	vec3 pos = vec3(0,0,0);
	vec3 rot = vec3(0,0,0);
	vec3 scale = vec3(1,1,1);
	mat4 VP;
	vec3 camPos = vec3(0,0,0);
	int height = 0;
	int width = 0;
};

void DLLAPI_RENDER applyRenderState(renderStateKind state);

struct DLLAPI_RENDER GPUdriverKind {
	renderStateKind renderState;
	vector<renderStateKind> renderStateStack;
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
	map<string, int> locations;
	map<bitmap*, GLuint> bitmap_GPU_handles;
	map<texture*, GLuint> texture_GPU_handles;
	map<rmodel*, GLuint[4]> rmodel_GPU_handles;
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
	void command_font_select(font* fnt);
	void command_mode_select(int i);
	void command_text_pos(vec2 v);
	void command_scissor(rect r);
	void command_pointsize(float f);
	void command_rmodel_render(rmodel* rm);
	void command_rmodel_delete(rmodel* rm);
	void command_linewidth(float f);
	void command_print_text(string *S);
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
	void parseCommand(const renderCommand3& rcmd);
	void projectionToCamera();
	void reproject();

#ifndef NO_GLEW
	GLuint uploadShader(GLuint shaderType, const char* filepath);
	GLuint uploadShaderProgram(const char* filepath_vertex, const char* filepath_fragment);
#endif
};

#ifndef NO_GLEW
GLenum pixelFormatToGL(pixelFormat F);
#endif

#endif // GPU_DRIVER_GUARD
