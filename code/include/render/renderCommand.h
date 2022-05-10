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

struct renderCommand3{
    renderCommand3();
    renderCommand3(const renderCommand3 &other) = delete; 
    renderCommand3(RC3T type, ...);
    renderCommand3 *clone(); 
                            
                            
    ~renderCommand3();
    

    
    RC3T type;  
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
        camprojection *cam; 
        string *s;          
        void* buff;
    };
};
string toString(renderCommand3 *rcmd);

#endif 
