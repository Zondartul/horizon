#ifndef GPU_DRIVER_GUARD
#define GPU_DRIVER_GUARD
#include "fonts.h"
#include "bitmap.h"
#include "vec.h"
#ifndef NO_GLEW
#include "GL/glew.h"
#endif
#include "rmodel.h"
#include "renderLayer.h"
struct renderStateKind{
	public:
	int renderMode = 0; 
	font *fnt = 0;
	vec2 textPos = vec2(0,0);
	vec3 pos = vec3(0,0,0);
	vec3 rot = vec3(0,0,0);
	vec3 scale = vec3(1,1,1);
	mat4 VP;
	vec3 camPos = vec3(0,0,0);
    int height;
    int width;
};
struct GPUdriverKind{
	renderStateKind renderState;
	int numVerts = 0;
	int num_textures = 0;
	long bytes_textures = 0;
	int num_rmodels = 0;
	long bytes_rmodels = 0;
	bool debug = false;
#ifndef NO_GLEW
	GLuint vaoHandle;
	GLuint positionBufferHandle;
	GLuint colorBufferHandle;
	GLuint normalBufferHandle;
	GLuint uvBufferHandle;
	map<string,int> locations;
	map<bitmap*,GLuint> bitmap_GPU_handles;
	map<texture*,GLuint> texture_GPU_handles;
	map<rmodel*,GLuint[4]> rmodel_GPU_handles;
#endif
	void renderLowInit();
	void parseQueue(renderQueue3 *rqueue);
    void parseCommand(const renderCommand3 &rcmd);
	void projectionToCamera();
	void reproject();
#ifndef NO_GLEW
	GLuint uploadShader(GLuint shaderType, const char *filepath);
	GLuint uploadShaderProgram(const char *filepath_vertex, const char *filepath_fragment);
#endif
};
#ifndef NO_GLEW
GLenum pixelFormatToGL(pixelFormat F);
#endif
#endif 
