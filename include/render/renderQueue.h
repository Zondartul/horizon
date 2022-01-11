#ifndef RENDERQUEUE_GUARD
#define RENDERQUEUE_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "stdint.h"
//texture has pimpl: textureRenderHandle *handle;
//is usually GLuint;
//rmodel has pimpl: rmodelRenderHandle *handle;
//is usually GLuint[4];

enum class RCMD{
	COLORING,
	TRANSPARENCY,
	TEXTURING,
	DEBUG,
	LIGHTING,
	LIGHT_VECTOR,
	LIGHT_COLOR,
	LIGHT_AMBIENT,
	LINE_THICKNESS,
	SCISSOR,
	SCISSOR_RECT,
	CULLING,
	CULLING_DIR,
	VERTICES_SELECT,
	COLORS_SELECT,
	ALPHAS,
	NORMALS_SELECT,
	UVS_SELECT,
	BUFFER_VEC3_LOAD,
	BUFFER_VEC2_LOAD,
	BUFFER_DELETE,
	RMODEL_DELETE,
	PROJECTION,
	TEXTURE_LOAD,
	TEXTURE_SELECT,
	DRAW_POINTS,
	DRAW_LINES,
	DRAW_TRIANGLES,
	DRAW_TRIANGLE_FAN
};

class renderCommand2{
	public:
	RCMD type;
	virtual void execute()=0;
};

class rcmd_coloring:public renderCommand2{
	public:
	bool b;
	rcmd_coloring(bool on);
	void execute();
};

class rcmd_transparency:public renderCommand2{
	public:
	bool b;
	rcmd_transparency(bool on);
	void execute();
};

class rcmd_texturing:public renderCommand2{
	public:
	bool b;
	rcmd_texturing(bool on);
	void execute();
};

class rcmd_debug_shader:public renderCommand2{
	public:
	bool b;
	rcmd_debug_shader(bool on);
	void execute();
};

class rcmd_projection:public renderCommand2{
	public:
	mat4 m;
	rcmd_projection(mat4 proj);
	void execute();
};
struct texture;
class rcmd_texture_upload:public renderCommand2{
	public:
	texture *t;
	rcmd_texture_upload(texture *t);
	void execute();
};

class rcmd_texture_select:public renderCommand2{
	public:
	texture *t;
	rcmd_texture_select(texture *t);
	void execute();
};

struct rmodel;
class rcmd_rmodel_upload:public renderCommand2{
	public:
	rmodel *rm;
	rcmd_rmodel_upload(rmodel *rm);
	void execute();
};


class rcmd_rmodel_render:public renderCommand2{
	public:
	rmodel *rm;
	int mode;
	rcmd_rmodel_render(rmodel *rm, int mode); //1 - points, 2 - lines, 3 - triangles
	void execute();
};


class rcmd_rmodel_delete:public renderCommand2{
	public:
	rmodel *rm;
	rcmd_rmodel_delete(rmodel *rm);
	void execute();
};

struct renderCommand{
	public:
	RCMD type;
	//char data[64];
	bool b;
	uint32_t i;
	void *p;
	mat4 m;	//can't put into union
};

typedef vector<renderCommand> renderCommandBuffer;
typedef	vector<renderCommandBuffer*> layers;

typedef vector<renderCommand2*> renderQueue;


#endif