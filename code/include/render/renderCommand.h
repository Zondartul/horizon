#ifndef RENDERCOMMAND_GUARD
#define RENDERCOMMAND_GUARD
#include "vec.h"
#include <string>
#include "camera.h"
using std::string;
#include "stringUtils.h"

class renderLayer;
struct texture;
class rmodel;
struct camprojection;
struct font;

enum RC3T{
        ERROR = 0,
        COLORING,           UVCOLORING,         NORMALCOLORING,
        TRANSPARENCY,       DEPTHMASK,          TEXTURING,
        DEBUG,              SCISSORING,         DEPTH_TEST,
        LIGHTING,           LAYER,              COLOR,
        ALPHA,              TEXTURE_SELECT,     FONT_SELECT,
        MODE_SELECT,        TEXT_POS,           SCISSOR,
        POINTSIZE,          LINEWIDTH,          SUN_POS,
        SUN_LIGHT_COLOR,    AMBIENT_LIGHT_COLOR,
        TEXTURE_UPLOAD,     RMODEL_UPLOAD,      RMODEL_DELETE,
        PROJECTION,         POSITION,           ROTATION,
        SCALE,              CLEAR_SCREEN,       RMODEL_RENDER,
        PRINT_TEXT,         COMMENT,			TEXTURE_SCALE,
        PUSH_OPTIONS,       POP_OPTIONS,		TEXTURE_POS,

        READ_PIXELS,        FACE_CULLING,       FACE_CULL_CCW,
        VIEWPORT
};
string toString(RC3T);
//takes up 20 bytes (so an average frame is 40 kb)
struct renderCommand3{
    renderCommand3();
    renderCommand3(const renderCommand3 &other) = delete; //may own stuff, so can't be copied.
    renderCommand3(RC3T type, ...);
    renderCommand3 *clone(); //allowed for non-privileged commands.
                            //priviliged commands are those that own big objects (like rmodels)
                            //but in less BS terms it's just to prevent access-after-delete.
    ~renderCommand3();
    //types

    //data
    RC3T type;  //type of command
    union{
        bool b;
        renderLayer *layer;
        vec3 v3;
        texture *t;
        font *fnt;
        int i;
        vec2 v2;
        rect r;
        float f;
        rmodel *rm;
        camprojection *cam; //huuuge, 4x4+3 floats = 60 bytes
        string *s;          //needs variable length serializing //also, non-POD constructor
        void* buff;
    };
};
string toString(renderCommand3 *rcmd);


//---------------------------------------
//
//class renderCommand2{
//	public:
//	//RCMD type;
//	const char *type;
//	virtual void execute()=0;
//	virtual string toString()=0;
//	virtual renderCommand2* clone() = 0;
//	virtual ~renderCommand2() = default;
//};
//
//string toString(renderCommand2 *rcmd);
//#define rcmd0(name) \
//class rcmd_##name:public renderCommand2{\
//	public:\
//	rcmd_##name();\
//	void execute();\
//	string toString();\
//	rcmd_##name* clone();\
//};
//
//#define rcmd1(name,T) \
//class rcmd_##name:public renderCommand2{\
//	public:\
//	T val;\
//	rcmd_##name(T val);\
//	void execute();\
//	string toString();\
//	rcmd_##name* clone();\
//	virtual ~rcmd_##name() = default;\
//};
//
//#define rcmd0_c_impl(name) \
//rcmd_##name::rcmd_##name(){type = #name;} \
//string rcmd_##name::toString(){return string(type)+"()";}\
//rcmd_##name* rcmd_##name::clone(){return new rcmd_##name(*this);}
//
//#define rcmd1_c_impl(name,T) \
//rcmd_##name::rcmd_##name(T val){type = #name; this->val = val;} \
//string rcmd_##name::toString(){return string(type)+"("+::toString(val)+")";}\
//rcmd_##name* rcmd_##name::clone(){return new rcmd_##name(*this);}
//
//
//class rmodel;
//struct texture;
//struct font;
//class renderLayer;
//
//rcmd1(coloring,bool);
//rcmd1(uvcoloring,bool);
//rcmd1(normalcoloring,bool);
//rcmd1(transparency,bool);
//rcmd1(depthmask,bool);
//rcmd1(texturing,bool);
//rcmd1(debug,bool);
//rcmd1(scissoring,bool);
//rcmd1(depth_test,bool);
//rcmd1(lighting,bool);
//
//rcmd1(layer,renderLayer*);
//rcmd1(color,vec3);
//rcmd1(alpha,float);
//rcmd1(texture_select,texture*);
//rcmd1(font_select,font*);
//rcmd1(mode_select,int);
//rcmd1(text_pos,vec2);
//rcmd1(scissor,rect);
//rcmd1(pointsize,float);
//rcmd1(linewidth,float);
//rcmd1(sun_pos,vec3);
//rcmd1(sun_light_color,vec3);
//rcmd1(ambient_light_color,vec3);
//rcmd1(texture_upload,texture*);
//rcmd1(rmodel_upload,rmodel*);
//rcmd1(rmodel_delete,rmodel*);
//rcmd1(projection,camprojection);
//rcmd1(position,vec3);
//rcmd1(rotation,vec3);
//rcmd1(scale,vec3);
//rcmd0(clear_screen);
//rcmd1(rmodel_render,rmodel*);
//rcmd1(print_text,string);
//rcmd1(comment,string);

#endif // RENDERCOMMAND_H_INCLUDED
