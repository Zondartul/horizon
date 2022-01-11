#ifndef RENDERLAYER_GUARD
#define RENDERLAYER_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "stdint.h"
#include <string>
using std::string;
#include "camera.h"
#include "stringUtils.h"
//texture has pimpl: textureRenderHandle *handle;
//is usually GLuint;
//rmodel has pimpl: rmodelRenderHandle *handle;
//is usually GLuint[4];

class renderCommand2{
	public:
	//RCMD type;
	const char *type;
	virtual void execute()=0;
	virtual string toString()=0;
	virtual renderCommand2* clone() = 0;
};

#define rcmd0(name) \
class rcmd_##name:public renderCommand2{\
	public:\
	rcmd_##name();\
	void execute();\
	string toString();\
	rcmd_##name* clone();\
};

#define rcmd1(name,T) \
class rcmd_##name:public renderCommand2{\
	public:\
	T val;\
	rcmd_##name(T val);\
	void execute();\
	string toString();\
	rcmd_##name* clone();\
};

#define rcmd0_c_impl(name) \
rcmd_##name::rcmd_##name(){type = #name;} \
string rcmd_##name::toString(){return string(type)+"()";}\
rcmd_##name* rcmd_##name::clone(){return new rcmd_##name(*this);}

#define rcmd1_c_impl(name,T) \
rcmd_##name::rcmd_##name(T val){type = #name; this->val = val;} \
string rcmd_##name::toString(){return string(type)+"("+::toString(val)+")";}\
rcmd_##name* rcmd_##name::clone(){return new rcmd_##name(*this);}


struct rmodel;
struct texture;
struct font;
struct renderLayer;

rcmd1(coloring,bool);
rcmd1(transparency,bool);
rcmd1(depthmask,bool);
rcmd1(texturing,bool);
rcmd1(debug,bool);
rcmd1(scissoring,bool);
rcmd1(depth_test,bool);
rcmd1(lighting,bool);

rcmd1(layer,renderLayer*);
rcmd1(color,vec3);
rcmd1(alpha,float);
rcmd1(texture_select,texture*);
rcmd1(font_select,font*);
rcmd1(mode_select,int);
rcmd1(text_pos,vec2);
rcmd1(scissor,rect);
rcmd1(pointsize,float);
rcmd1(linewidth,float);
rcmd1(sun_pos,vec3);
rcmd1(sun_light_color,vec3);
rcmd1(ambient_light_color,vec3);
rcmd1(texture_upload,texture*);
rcmd1(rmodel_upload,rmodel*);
rcmd1(rmodel_delete,rmodel*);
rcmd1(projection,camprojection);
rcmd1(position,vec3);
rcmd1(rotation,vec3);
rcmd1(scale,vec3);
rcmd0(clear_screen);
rcmd1(rmodel_render,rmodel*);
rcmd1(print_text,string);
rcmd1(comment,string);


typedef vector<renderCommand2*> renderQueue;

class renderLayer{
	public:
	renderQueue queue;
	renderLayer *resetLayer = 0;
	void render();
	void clear();
	void reset();	//add a call to the resetLayer
	void print();
};

#endif