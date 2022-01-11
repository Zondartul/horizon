#include "renderQueue.h"



rcmd_coloring::rcmd_coloring(bool on){b = on;}
rcmd_transparency::rcmd_transparency(bool on){b = on;}
rcmd_texturing::rcmd_texturing(bool on){b = on;}
rcmd_debug_shader::rcmd_debug_shader(bool on){b = on;}

rcmd_projection::rcmd_projection(mat4 m){this->m = m;}
rcmd_texture_upload::rcmd_texture_upload(texture *t){this->t = t;}
rcmd_texture_select::rcmd_texture_select(texture *t){this->t = t;}

rcmd_rmodel_upload::rcmd_rmodel_upload(rmodel *rm){this->rm = rm;}
rcmd_rmodel_render::rcmd_rmodel_render(rmodel *rm, int mode){this->rm = rm; this->mode = mode;} //1 - points, 2 - lines, 3 - triangles
rcmd_rmodel_delete::rcmd_rmodel_delete(rmodel *rm){this->rm = rm;}

//execute is implemented in renderLow.cpp