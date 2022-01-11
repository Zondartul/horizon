#ifndef RENDERQUEUE_GUARD
#define RENDERQUEUE_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "stdint.h"

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

#endif