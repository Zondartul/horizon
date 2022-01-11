#ifndef RENDERLAYER_GUARD
#define RENDERLAYER_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "stdint.h"
#include <string>
using std::string;
#include "stringUtils.h"
//texture has pimpl: textureRenderHandle *handle;
//is usually GLuint;
//rmodel has pimpl: rmodelRenderHandle *handle;
//is usually GLuint[4];
/*
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
*/
class renderCommand2{
	public:
	//RCMD type;
	const char *type;
	virtual void execute()=0;
	virtual string toString()=0;
};

#define rcmd0(name) \
class rcmd_##name:public renderCommand2{\
	public:\
	rcmd_##name();\
	void execute();\
	string toString();\
};

#define rcmd1(name,T) \
class rcmd_##name:public renderCommand2{\
	public:\
	T val;\
	rcmd_##name(T val);\
	void execute();\
	string toString();\
};

#define rcmd0_c_impl(name) \
rcmd_##name::rcmd_##name(){type = #name;} \
string rcmd_##name::toString(){return string(type)+"()";}

#define rcmd1_c_impl(name,T) \
rcmd_##name::rcmd_##name(T val){type = #name; this->val = val;} \
string rcmd_##name::toString(){return string(type)+"("+::toString(val)+")";}


struct rmodel;
struct texture;
struct font;

rcmd1(coloring,bool);
rcmd1(transparency,bool);
rcmd1(texturing,bool);
rcmd1(debug,bool);
rcmd1(scissoring,bool);
rcmd1(depth_test,bool);

rcmd1(color,vec3f);
rcmd1(alpha,float);
rcmd1(texture_select,texture*);
rcmd1(font_select,font*);
rcmd1(mode_select,int);
rcmd1(text_pos,vec2);
rcmd1(scissor,rect);
rcmd1(pointsize,float);
rcmd1(linewidth,float);
rcmd1(texture_upload,texture*);
rcmd1(rmodel_upload,rmodel*);
rcmd1(rmodel_delete,rmodel*);
rcmd1(projection,mat4);
rcmd1(position,vec3);
rcmd1(scale,vec3);
rcmd0(clear_screen);
rcmd1(rmodel_render,rmodel*);
rcmd1(print_text,string);
rcmd1(comment,string);

// class rcmd_coloring:public renderCommand2{
	// public:
	// bool b;
	// rcmd_coloring(bool on);
	// void execute();
// };

// class rcmd_transparency:public renderCommand2{
	// public:
	// bool b;
	// rcmd_transparency(bool on);
	// void execute();
// };

// class rcmd_texturing:public renderCommand2{
	// public:
	// bool b;
	// rcmd_texturing(bool on);
	// void execute();
// };

// class rcmd_debug_shader:public renderCommand2{
	// public:
	// bool b;
	// rcmd_debug_shader(bool on);
	// void execute();
// };

// class rcmd_scissoring:public renderCommand2{
	// public:
	// bool b;
	// rcmd_scissoring(bool on);
	// void execute();
// };

// class rcmd_color:public renderCommand2{
	// public:
	// vec3f col;
	// rcmd_color(vec3f col);
	// void execute();
// };

// class rcmd_alpha:public renderCommand2{
	// public:
	// float alpha;
	// rcmd_alpha(float a);
	// void execute();
// };

// class rcmd_projection:public renderCommand2{
	// public:
	// mat4 m;
	// rcmd_projection(mat4 proj);
	// void execute();
// };
// struct texture;
// class rcmd_texture_upload:public renderCommand2{
	// public:
	// texture *t;
	// rcmd_texture_upload(texture *t);
	// void execute();
// };

// class rcmd_texture_select:public renderCommand2{
	// public:
	// texture *t;
	// rcmd_texture_select(texture *t);
	// void execute();
// };

// struct font;
// class rcmd_font_select:public renderCommand2{
	// public:
	// font *fnt;
	// rcmd_font_select(font *f);
	// void execute();
// };

// class rcmd_mode_select:public renderCommand2{
	// public:
	// int mode;
	// rcmd_mode_select(int m);
	// void execute();
// };

// class rcmd_text_pos:public renderCommand2{
	// public:
	// vec2 textPos;
	// rcmd_text_pos(vec2 pos);
	// void execute();
// };

// class rcmd_print_text:public renderCommand2{
	// public:
	// const char *text;
	// rcmd_print_text(const char *txt);
	// void execute();
// };

// class rcmd_scissor:public renderCommand2{
	// public:
	// rect scissor;
	// rcmd_scissor(rect S);
	// void execute();
// };

// class rcmd_clear_screen:public renderCommand2{
	// public:
	// rcmd_clear_screen();
	// void execute();
// };

// struct rmodel;
// class rcmd_rmodel_upload:public renderCommand2{
	// public:
	// rmodel *rm;
	// rcmd_rmodel_upload(rmodel *rm);
	// void execute();
// };


// class rcmd_rmodel_render:public renderCommand2{
	// public:
	// rmodel *rm;
	// rcmd_rmodel_render(rmodel *rm);
	// void execute();
// };


// class rcmd_rmodel_delete:public renderCommand2{
	// public:
	// rmodel *rm;
	// rcmd_rmodel_delete(rmodel *rm);
	// void execute();
// };

//typedef vector<renderCommand> renderCommandBuffer;
//typedef	vector<renderCommandBuffer*> layers;

typedef vector<renderCommand2*> renderQueue;

class renderLayer{
	public:
	renderQueue queue;
	
	void render();
	void clear();
	void print();
};

#endif