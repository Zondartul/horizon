#include "renderLayer.h"
#include "renderLow.h"
#include "stdio.h"

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

rcmd1_c_impl(coloring,bool);
rcmd1_c_impl(transparency,bool);
rcmd1_c_impl(texturing,bool);
rcmd1_c_impl(debug,bool);
rcmd1_c_impl(scissoring,bool);
rcmd1_c_impl(depth_test,bool);

rcmd1_c_impl(color,vec3f);
rcmd1_c_impl(alpha,float);
rcmd1_c_impl(texture_select,texture*);
rcmd1_c_impl(font_select,font*);
rcmd1_c_impl(mode_select,int);
rcmd1_c_impl(text_pos,vec2);
rcmd1_c_impl(scissor,rect);
rcmd1_c_impl(pointsize,float);
rcmd1_c_impl(linewidth,float);
rcmd1_c_impl(texture_upload,texture*);
rcmd1_c_impl(rmodel_upload,rmodel*);
rcmd1_c_impl(rmodel_delete,rmodel*);
rcmd1_c_impl(projection,mat4);
rcmd1_c_impl(position,vec3);
rcmd1_c_impl(scale,vec3);
rcmd0_c_impl(clear_screen);
rcmd1_c_impl(rmodel_render,rmodel*);
rcmd1_c_impl(print_text,string);
rcmd1_c_impl(comment,string);

void renderLayer::render(){renderParseQueue(&queue);}
void renderLayer::clear(){
	for(auto I = queue.begin(); I != queue.end(); I++){
		delete *I;
	}
	queue.clear();
}
void renderLayer::print(){
	int J = 0;
	for(auto I = queue.begin(); I != queue.end(); I++,J++){
		printf("%d: %s\n",J,(*I)->toString().c_str());
	}
}

// rcmd_scissoring::rcmd_scissoring(bool on){b = on;}
// rcmd_coloring::rcmd_coloring(bool on){b = on;}
// rcmd_alpha::rcmd_alpha(float a){alpha = a;}
// rcmd_font_select::rcmd

//execute is implemented in renderLow.cpp