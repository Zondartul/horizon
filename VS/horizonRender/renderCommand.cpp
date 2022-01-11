#include "renderCommand.h"
#include "util.h"
#include "globals.h"
#include "rmodel.h"
/*
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
        PRINT_TEXT,         COMMENT
};
*/
string toString(RC3T type){
    switch(type){
        case(RC3T::COLORING):              return "coloring";
        case(RC3T::UVCOLORING):            return "uv_coloring";
        case(RC3T::NORMALCOLORING):        return "normal_coloring";
        case(RC3T::TRANSPARENCY):          return "transparency";
        case(RC3T::DEPTHMASK):             return "depth_mask";
        case(RC3T::TEXTURING):             return "texturing";
        case(RC3T::DEBUG):                 return "debug";
        case(RC3T::SCISSORING):            return "scissoring";
        case(RC3T::DEPTH_TEST):            return "depth_test";
        case(RC3T::LIGHTING):              return "lighting";

        case(RC3T::LAYER):                 return "layer";
        case(RC3T::COLOR):                 return "color";
        case(RC3T::ALPHA):                 return "alpha";
        case(RC3T::TEXTURE_SELECT):        return "texture_select";
        case(RC3T::FONT_SELECT):           return "font_select";
        case(RC3T::MODE_SELECT):           return "mode_select";
        case(RC3T::TEXT_POS):              return "text_pos";
        case(RC3T::SCISSOR):               return "scissor";
        case(RC3T::POINTSIZE):             return "point_size";
        case(RC3T::LINEWIDTH):             return "line_width";
        case(RC3T::SUN_POS):               return "sun_pos";
        case(RC3T::SUN_LIGHT_COLOR):       return "sun_light_color";
        case(RC3T::AMBIENT_LIGHT_COLOR):   return "ambient_light_color";
        case(RC3T::TEXTURE_UPLOAD):        return "texture_upload";
        case(RC3T::RMODEL_UPLOAD):         return "rmodel_upload";
        case(RC3T::RMODEL_DELETE):         return "rmodel_delete";
        case(RC3T::PROJECTION):            return "projection";
        case(RC3T::POSITION):              return "position";
        case(RC3T::ROTATION):              return "rotation";
        case(RC3T::SCALE):                 return "scale";
		case(RC3T::TEXTURE_POS):		   return "texture_pos";
		case(RC3T::TEXTURE_SCALE):		   return "texture_scale";
        case(RC3T::CLEAR_SCREEN):          return "clear_screen";
        case(RC3T::RMODEL_RENDER):         return "rmodel_render";
        case(RC3T::PRINT_TEXT):            return "print_text";
        case(RC3T::COMMENT):               return "comment";
    }
    return "<error>";
}

renderCommand3::renderCommand3(){type = RC3T::ERROR;}

renderCommand3::renderCommand3(RC3T type, ...):type(type){
    va_list args;
    va_start(args,type);
    switch(type){
        case(RC3T::COLORING):               b = va_arg(args,int); break;
        case(RC3T::UVCOLORING):             b = va_arg(args,int); break;
        case(RC3T::NORMALCOLORING):         b = va_arg(args,int); break;
        case(RC3T::TRANSPARENCY):           b = va_arg(args,int); break;
        case(RC3T::DEPTHMASK):              b = va_arg(args,int); break;
        case(RC3T::TEXTURING):              b = va_arg(args,int); break;
        case(RC3T::DEBUG):                  b = va_arg(args,int); break;
        case(RC3T::SCISSORING):             b = va_arg(args,int); break;
        case(RC3T::DEPTH_TEST):             b = va_arg(args,int); break;
        case(RC3T::LIGHTING):               b = va_arg(args,int); break;

        case(RC3T::LAYER):                  layer = va_arg(args,renderLayer*); break;
        case(RC3T::COLOR):                  v3 = va_arg(args,vec3); break;
        case(RC3T::ALPHA):                  f = va_arg(args,double); break;
        case(RC3T::TEXTURE_SELECT):         t = va_arg(args,texture*); break;
        case(RC3T::FONT_SELECT):            fnt = va_arg(args,font*); break;
        case(RC3T::MODE_SELECT):            i = va_arg(args,int); break;
        case(RC3T::TEXT_POS):               v2 = va_arg(args,vec2); break;
        case(RC3T::SCISSOR):                r = va_arg(args,rect); break;
        case(RC3T::POINTSIZE):              f = va_arg(args,double); break;
        case(RC3T::LINEWIDTH):              f = va_arg(args,double); break;
        case(RC3T::SUN_POS):                v3 = va_arg(args,vec3); break;
        case(RC3T::SUN_LIGHT_COLOR):        v3 = va_arg(args,vec3); break;
        case(RC3T::AMBIENT_LIGHT_COLOR):    v3 = va_arg(args,vec3); break;
        case(RC3T::TEXTURE_UPLOAD):         t = va_arg(args,texture*); break;
        case(RC3T::RMODEL_UPLOAD):          rm = va_arg(args,rmodel*); break;
        case(RC3T::RMODEL_DELETE):          rm = va_arg(args,rmodel*); break;
        case(RC3T::PROJECTION):             cam = va_arg(args,camprojection*); break;
        case(RC3T::POSITION):               v3 = va_arg(args,vec3); break;
        case(RC3T::ROTATION):               v3 = va_arg(args,vec3); break;
        case(RC3T::SCALE):                  v3 = va_arg(args,vec3); break;
		case(RC3T::TEXTURE_POS):		    v2 = va_arg(args,vec2); break;
		case(RC3T::TEXTURE_SCALE):			v2 = va_arg(args,vec2); break;
        case(RC3T::CLEAR_SCREEN):           break;
        case(RC3T::RMODEL_RENDER):          rm = va_arg(args,rmodel*); break;
        case(RC3T::PRINT_TEXT):             s = va_arg(args,string*); break;
        case(RC3T::COMMENT):                s = va_arg(args,string*); break;
    }
    va_end(args);
}

renderCommand3 *renderCommand3::clone(){
    renderCommand3 *rcmd = new renderCommand3();
    rcmd->type = type;
    switch(type){
        case(RC3T::COLORING):
        case(RC3T::UVCOLORING):
        case(RC3T::NORMALCOLORING):
        case(RC3T::TRANSPARENCY):
        case(RC3T::DEPTHMASK):
        case(RC3T::TEXTURING):
        case(RC3T::DEBUG):
        case(RC3T::SCISSORING):
        case(RC3T::DEPTH_TEST):
        case(RC3T::LIGHTING):
                                            rcmd->b = b; break;

        case(RC3T::LAYER):                  rcmd->layer = layer; break;
        case(RC3T::COLOR):
        case(RC3T::POSITION):
        case(RC3T::ROTATION):
        case(RC3T::SCALE):
                                            rcmd->v3 = v3;
        case(RC3T::ALPHA):
        case(RC3T::POINTSIZE):
        case(RC3T::LINEWIDTH):
                                            rcmd->f = f; break;
        case(RC3T::TEXTURE_SELECT):         rcmd->t = t; break;
        case(RC3T::FONT_SELECT):            rcmd->fnt = fnt; break;
        case(RC3T::MODE_SELECT):            rcmd->i = i; break;
        case(RC3T::TEXT_POS):               rcmd->v2 = v2; break;
        case(RC3T::SCISSOR):                rcmd->r = r; break;
        case(RC3T::SUN_POS):
        case(RC3T::SUN_LIGHT_COLOR):
        case(RC3T::AMBIENT_LIGHT_COLOR):
                                            rcmd->v3 = v3; break;
        case(RC3T::TEXTURE_UPLOAD):         rcmd->t = t; break;
        case(RC3T::RMODEL_UPLOAD):
        case(RC3T::RMODEL_RENDER):
                                            rcmd->rm = rm; break;
        case(RC3T::PROJECTION):             rcmd->cam = cam; break;
        case(RC3T::CLEAR_SCREEN):           break;
        case(RC3T::PRINT_TEXT):             rcmd->s = copyToHeap(*s); break;
        case(RC3T::COMMENT):                rcmd->s = copyToHeap(*s); break;


        case(RC3T::RMODEL_DELETE):          return 0; //copying of this command is forbidden
    }
    return rcmd;
}


renderCommand3::~renderCommand3(){
    switch(type){
        case(RC3T::RMODEL_DELETE):          delete rm; break;
        case(RC3T::PROJECTION):             delete cam; break;
        case(RC3T::PRINT_TEXT):             delete s; break;
        case(RC3T::COMMENT):                delete s; break;
    }
}

#define isValid(x) (x && !wasRecentlyDeleted(x))

string toString(renderCommand3 *rcmd){
    string S = toString(rcmd->type)+" ";
    switch(rcmd->type){
        case(RC3T::COLORING):               S +=  toString(rcmd->b); break;
        case(RC3T::UVCOLORING):             S +=  toString(rcmd->b); break;
        case(RC3T::NORMALCOLORING):         S +=  toString(rcmd->b); break;
        case(RC3T::TRANSPARENCY):           S +=  toString(rcmd->b); break;
        case(RC3T::DEPTHMASK):              S +=  toString(rcmd->b); break;
        case(RC3T::TEXTURING):              S +=  toString(rcmd->b); break;
        case(RC3T::DEBUG):                  S +=  toString(rcmd->b); break;
        case(RC3T::SCISSORING):             S +=  toString(rcmd->b); break;
        case(RC3T::DEPTH_TEST):             S +=  toString(rcmd->b); break;
        case(RC3T::LIGHTING):               S +=  toString(rcmd->b); break;

        case(RC3T::LAYER):                  S +=  isValid(rcmd->layer)? toString(rcmd->layer) : fstring("<deleted %p>",rcmd->layer); break;
        case(RC3T::COLOR):                  S +=  toString(rcmd->v3); break;
        case(RC3T::ALPHA):                  S +=  toString(rcmd->f); break;
        case(RC3T::TEXTURE_SELECT):         S +=  toString(rcmd->t); break;
        case(RC3T::FONT_SELECT):            S +=  isValid(rcmd->fnt)? toString(rcmd->fnt) : fstring("<deleted %p>",rcmd->fnt); break;
        case(RC3T::MODE_SELECT):            S +=  toString(rcmd->i); break;
        case(RC3T::TEXT_POS):               S +=  toString(rcmd->v2); break;
        case(RC3T::SCISSOR):                S +=  toString(rcmd->r); break;
        case(RC3T::POINTSIZE):              S +=  toString(rcmd->f); break;
        case(RC3T::LINEWIDTH):              S +=  toString(rcmd->f); break;
        case(RC3T::SUN_POS):                S +=  toString(rcmd->v3); break;
        case(RC3T::SUN_LIGHT_COLOR):        S +=  toString(rcmd->v3); break;
        case(RC3T::AMBIENT_LIGHT_COLOR):    S +=  toString(rcmd->v3); break;
        case(RC3T::TEXTURE_UPLOAD):         S +=  isValid(rcmd->t)? toString(rcmd->t) : fstring("<deleted %p>",rcmd->rm);  break;
        case(RC3T::RMODEL_UPLOAD):          S +=  isValid(rcmd->rm)? toString(rcmd->rm) : fstring("<deleted %p>",rcmd->rm); break;
        case(RC3T::RMODEL_DELETE):          S +=  isValid(rcmd->rm)? toString(rcmd->rm) : fstring("<deleted %p>",rcmd->rm); break;
        case(RC3T::PROJECTION):             S +=  isValid(rcmd->cam)? toString(rcmd->cam) : fstring("<deleted %p>",rcmd->rm); break;
        case(RC3T::POSITION):               S +=  toString(rcmd->v3); break;
        case(RC3T::ROTATION):               S +=  toString(rcmd->v3); break;
        case(RC3T::SCALE):                  S +=  toString(rcmd->v3); break;
        case(RC3T::CLEAR_SCREEN):           break;
        case(RC3T::RMODEL_RENDER):          S +=  isValid(rcmd->rm)? toString(rcmd->rm) : fstring("<deleted %p>",rcmd->rm); break;
        case(RC3T::PRINT_TEXT):             S +=  isValid(rcmd->s)? ("\""+toString(*(rcmd->s))+"\"") : fstring("<deleted %p>",rcmd->s); break;
        case(RC3T::COMMENT):                S +=  isValid(rcmd->s)? ("\""+toString(*(rcmd->s))+"\"") : fstring("<deleted %p>",rcmd->s); break;
    }
    return S;
}
// rcmd_coloring::rcmd_coloring(bool on){b = on;}
// rcmd_transparency::rcmd_transparency(bool on){b = on;}

// rcmd_texturing::rcmd_texturing(bool on){b = on;}
// rcmd_debug_shader::rcmd_debug_shader(bool on){b = on;}

// rcmd_projection::rcmd_projection(mat4 m){this->m = m;}
// rcmd_texture_upload::rcmd_texture_upload(texture *t){this->t = t;}
// rcmd_texture_select::rcmd_texture_select(texture *t){this->t = t;}

// rcmd_rmodel_upload::rcmd_rmodel_upload(rmodel *rm){this->rm = rm;}
// rcmd_rmodel_render::rcmd_rmodel_render(rmodel *rm){this->rm = rm;}
// rcmd_rmodel_delete::rcmd_rmodel_delete(rmodel *rm){this->rm = rm;}
//
//rcmd1_c_impl(coloring,bool);
//rcmd1_c_impl(uvcoloring,bool);
//rcmd1_c_impl(normalcoloring,bool);
//rcmd1_c_impl(transparency,bool);
//rcmd1_c_impl(depthmask,bool);
//rcmd1_c_impl(texturing,bool);
//rcmd1_c_impl(debug,bool);
//rcmd1_c_impl(scissoring,bool);
//rcmd1_c_impl(depth_test,bool);
//rcmd1_c_impl(lighting,bool);
//
//rcmd1_c_impl(layer,renderLayer*);
//rcmd1_c_impl(color,vec3);
//rcmd1_c_impl(alpha,float);
//rcmd1_c_impl(texture_select,texture*);
//rcmd1_c_impl(font_select,font*);
//rcmd1_c_impl(mode_select,int);
//rcmd1_c_impl(text_pos,vec2);
//rcmd1_c_impl(scissor,rect);
//rcmd1_c_impl(pointsize,float);
//rcmd1_c_impl(linewidth,float);
//rcmd1_c_impl(sun_pos,vec3);
//rcmd1_c_impl(sun_light_color,vec3);
//rcmd1_c_impl(ambient_light_color,vec3);
//rcmd1_c_impl(texture_upload,texture*);
//rcmd1_c_impl(rmodel_upload,rmodel*);
//rcmd1_c_impl(rmodel_delete,rmodel*);
//rcmd1_c_impl(projection,camprojection);
//rcmd1_c_impl(position,vec3);
//rcmd1_c_impl(rotation,vec3);
//rcmd1_c_impl(scale,vec3);
//rcmd0_c_impl(clear_screen);
//rcmd1_c_impl(rmodel_render,rmodel*);
//rcmd1_c_impl(print_text,string);
//rcmd1_c_impl(comment,string);
