#include "renderLow.h"
#include "globals.h"
#include "vec.h"
#include "stdio.h"
#include "stdlib.h"
#include "GL/glew.h"
#include "GL/gl.h"
#include "file.h"
#include "window.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "printw.h"
#include "GPUdriver.h"
using glm::mat4;
using glm::vec3;
using glm::vec2;
#include "model.h"
extern model *mBox;
#include "bitmap.h"
#include <map>
using std::map;

#include "fonts.h"
#include "util.h"
#include "paint.h"

renderLowKind *renderLow;
renderLowKind::renderLowKind(){
	GPUdriver = new GPUdriverKind();
    vec2 size = getScreenSize();
    setViewportSize(size.x,size.y);
}

void renderLowKind::renderTick(){
    #warning "renderLowKind::renderTick() shouldn't exist?"
}

void renderLowKind::renderLowInit(){
    GPUdriver->renderLowInit();
}

void renderLowKind::setViewportSize(int width, int height){
    GPUdriver->renderState.width = width;
    GPUdriver->renderState.height = height;
}

void renderLowKind::renderParseQueue(vector<renderCommand2*>* rqueue){
    GPUdriver->parseQueue(rqueue);
}
                                                                    //quick hack
void renderLowKind::renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L){

            //----------------------------------------------------
        //----------------------------------------------------
        //----------------------------------------------------
    //printf("parsing Layer [%s]---------\n",toCString(L));
    int cmdNum = 0;
    renderCommand3 *rcmd = 0;
    try{
        for(auto I = rqueue->begin(); I != rqueue->end(); I++){
            rcmd = *I;
            //printf("parsing command [%s]\n",toCString(rcmd));
            parseCommand(*rcmd);
            cmdNum++;
        }
    }catch(exKind &e){
        static bool FRsaved = false;
        if(!FRsaved){
            printf("exception occurred during render, saving frame report\n");
            frameReport();
            FRsaved = true;
        }
        //throw renderExKind(e.msg, L, cmdNum);
        try{
        string extra = string()+"\nwhere layer = ["+(L? toString(L) : "<null>")+"], rcmd #"+cmdNum+" = ["+(rcmd? toString(rcmd) : "<null>")+"]";
        e.msg.msg += extra;
        }catch(...){}

        throw;
    }
    //GPUdriver->parseQueue(rqueue);
}

void renderLowKind::parseCommand(const renderCommand3 &rcmd){
        bool passCommand = true;
       switch(rcmd.type){
            //case(LAYER):
                //if(rcmd.layer){rcmd.layer->render();}
                //do nothing
            //break;

            case(COLORING):
                //glUniform1i(locations["coloringOn"],rcmd.b);
                options.coloring = rcmd.b;
            break;

            case(UVCOLORING):
                //glUniform1i(locations["uvColoringOn"],rcmd.b);
                options.uvColoring = rcmd.b;
            break;

            case(NORMALCOLORING):
                //glUniform1i(locations["normalColoringOn"],rcmd.b);
                options.normalColoring = rcmd.b;
            break;

            case(TRANSPARENCY):
                //if(rcmd.b){
                //    glEnable(GL_BLEND);
                //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                //}else{
                //    glDisable(GL_BLEND);
                //}
                //glUniform1i(locations["transparencyOn"],rcmd.b);
                options.transparency = rcmd.b;
            break;

            case(DEPTHMASK):
                //if(rcmd.b){
                //    glDepthMask(GL_TRUE);
                //}else{
                //    glDepthMask(GL_FALSE);
                //}
                options.depthMask = rcmd.b;
            break;

            case(TEXTURING):
                //glUniform1i(locations["texturingOn"],rcmd.b);
                options.texturing = rcmd.b;
            break;

            case(DEBUG):
                //glUniform1i(locations["debuggingOn"],rcmd.b);
                options.debug = rcmd.b;
            break;

            case(PROJECTION):
                //renderState.VP = rcmd.cam->MVP;
                //renderState.camPos = rcmd.cam->pos;
                //reproject();
                options.cpj = rcmd.cam;
            break;

            case(POSITION):
                //renderState.pos = rcmd.v3;
                //reproject();
                options.pos = rcmd.v3;
            break;

            case(ROTATION):
                //renderState.rot = rcmd.v3;
                //reproject();
                options.rot = rcmd.v3;
            break;

            case(SCALE):
                //renderState.scale = rcmd.v3;
                //reproject();
                options.scale = rcmd.v3;
            break;

            /*case(TEXTURE_UPLOAD):{
                texture *t = rcmd.t;
                if(!t){error("attempt to upload null texture\n");}
                if(texture_GPU_handles.count(t)){break;}//texture already uploaded.
                if(bitmap_GPU_handles.count(t->bmp)){break;}//texture already uploaded.
                if(t->bmp){
                    while(glGetError() != GL_NO_ERROR){}
                    num_textures++;
                    bytes_textures += t->bmp->width*t->bmp->height*4;
                    GLuint textureID;
                    glGenTextures(1, &textureID);
                    glBindTexture(GL_TEXTURE_2D,textureID);
                    glTexImage2D(GL_TEXTURE_2D,0,pixelFormatToGL(t->bmp->format), t->bmp->width, t->bmp->height, 0, pixelFormatToGL(t->bmp->format), GL_UNSIGNED_BYTE, t->bmp->data.get());
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    bitmap_GPU_handles[t->bmp] = textureID;
                    printf("texture '%s' uploaded: %d (%dx%d)\n",t->name.c_str(),textureID,t->bmp->height,t->bmp->width);

                    int err = glGetError();
                    if(err){
                        if(err == GL_OUT_OF_MEMORY){error("Error: GL_OUT_OF_MEMORY");}
                        string errstr = string()+"GL Error: "+err;
                        error(errstr.c_str());
                    }
                }else{
                    error("no texture data to upload (%s)\n",t->name.c_str());
                }
            } break; do nothing */

            case(TEXTURE_SELECT):/*{
                texture *t = rcmd.t;
                if(!t){error("attempt to select null texture\n");}
                GLuint handle = 0;
                //render_sys_data_texture rdata;
                if(t->bmp){
                    if(!bitmap_GPU_handles.count(t->bmp)){error("image not uploaded: \"%s\"/%p\n",t->name.c_str(),t->bmp);}
                    handle = bitmap_GPU_handles[t->bmp];
                }else{
                    if(!texture_GPU_handles.count(t)){error("texture not uploaded: %s\n",t->name.c_str());}
                    handle = texture_GPU_handles[t];
                }
                if(debug){printf("texture select %s (%d)\n",t->name.c_str(), handle);}
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, handle);
                glUniform1i(locations["Tex"],0);//wat
                vec2 offset = vec2(t->UV.start.x,t->UV.start.y);
                vec2 size = vec2(t->UV.size.x,t->UV.size.y);
                //offset = vec2(0,0);
                //size = vec2(1,1);
                //printf("tex offset: (%f x %f), size: (%f x %f)\n",offset.x,offset.y,size.x,size.y);
                glUniform2fv(locations["TexOffset"],1,(GLfloat*)&offset);
                glUniform2fv(locations["TexSize"],1,(GLfloat*)&size);
            } */
                options.t = rcmd.t;
            break;

            /*case(RMODEL_UPLOAD):{
                rmodel *rm = rcmd.rm;
                if(!rm){error("attempt to upload null rmodel\n");}
                //if(debug){printf("uploading rmodel %p\n",rm);}
                if(
                    (!rm->vertices)||(!rm->normals)||(!rm->colors)||(!rm->uvs)||
                    (rm->normals->size() != rm->vertices->size())||
                    (rm->colors->size() != rm->vertices->size())||
                    (rm->uvs->size() != rm->vertices->size())
                ){
                    error("trying to upload incomplete rmodel");
                }
                num_rmodels++;
                bytes_rmodels += rm->vertices->size()*4*11;
                while(glGetError() != GL_NO_ERROR){}
                GLuint handles[4];
                glGenBuffers(4,handles); //fills handle[0,1,2,3].


                //verts
                glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->vertices->data(), GL_STATIC_DRAW);

                //colors
                glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->colors->data(), GL_STATIC_DRAW);

                //normals
                glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
                glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->normals->data(), GL_STATIC_DRAW);

                //uvs
                glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
                glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec2), rm->uvs->data(), GL_STATIC_DRAW);

                for(int i = 0; i<4; i++){rmodel_GPU_handles[rm][i] = handles[i];}

                int err = glGetError();
                if(err){
                    if(err == GL_OUT_OF_MEMORY){error("Error: GL_OUT_OF_MEMORY");}
                    string errstr = string()+"GL Error: "+err;
                    error(errstr.c_str());
                }
            } break; do nothing */

            /*case(RMODEL_RENDER):{
                rmodel *rm = rcmd.rm;
                if(!rm){error("attempt to render null rmodel\n");}
                //if(debug){printf("rendering rmodel %p\n",rm);}
                GLuint handles[4];
                for(int i = 0; i<4; i++){handles[i] = rmodel_GPU_handles[rm][i];}
                //verts
                glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                //colors
                glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                //normals
                glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
                glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
                //uvs
                glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
                glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

                switch(renderState.renderMode){
                    case(1):
                        glDrawArrays(GL_POINTS,0,rm->vertices->size());
                        break;
                    case(2):
                        glDrawArrays(GL_LINES,0,rm->vertices->size());
                        break;
                    case(3):
                        glDrawArrays(GL_TRIANGLES,0,rm->vertices->size());
                        break;
                    default:
                        error("unknown render mode '%d'\n",renderState.renderMode);
                        break;
                }
            } break; do nothing*/

            /*case(RMODEL_DELETE):{
                rmodel *rm = rcmd.rm;
                if(!rm){error("attempt to delete null rmodel\n");}
                //if(debug){printf("deleting rmodel %p\n",rm);}
                num_rmodels--;
                bytes_rmodels -= rm->vertices->size()*4*11;
                GLuint handles[4];
                for(int i = 0; i<4; i++){handles[i] = rmodel_GPU_handles[rm][i];}
                glDeleteBuffers(4,handles);
                rmodel_GPU_handles.erase(rm);
            } break; do nothing */

            //case(CLEAR_SCREEN):
                //glClearColor(125/255.0f,206/255.0f,250/255.0f,1.0f);
                //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                //do nothing
            //break;

            case(SCISSORING):
                //glUniform1i(locations["scissoringOn"],rcmd.b);
                options.scissoring = rcmd.b;
            break;

            case(COLOR):/*{
                vec3 val2 = rcmd.v3;
                val2.x /= 255.0f;
                val2.y /= 255.0f;
                val2.z /= 255.0f;
                glUniform3fv(locations["globalColor"],1,(const GLfloat*)&val2);
            }*/
                options.color = rcmd.v3;
            break;

            case(ALPHA):/*{
                float val2 = rcmd.f/255.0f;
                glUniform1fv(locations["globalAlpha"],1,(const GLfloat*)&val2);
            }*/
                options.alpha = rcmd.f;
            break;

            case(FONT_SELECT):
                //if(!rcmd.fnt){error("attempt to select null font\n");}
                //renderState.fnt = rcmd.fnt;
                options.fnt = rcmd.fnt;
            break;

            case(MODE_SELECT):
                //renderState.renderMode = rcmd.i;
                options.renderMode = rcmd.i;
            break;

            case(TEXT_POS):
                //renderState.textPos = rcmd.v2;
                options.textPos = rcmd.v2;
            break;

            case(SCISSOR):/*{
                //if(debug){printf("set scissor to %dx%d\n",val.size.x,val.size.y);}
                float R[4];
                //R[0] = val.start.x;
                //R[1] = val.start.y;
                //R[2] = val.end.x;
                //R[3] = val.end.y;
                float x1 = ((float)rcmd.r.start.x)/renderState.width;
                float y1 = ((float)(rcmd.r.start.y))/renderState.height;
                float x2 = ((float)rcmd.r.end.x)/renderState.width;
                float y2 = ((float)(rcmd.r.end.y))/renderState.height;
                R[0] = 2*(x1-0.5);//(2*(x2)-1);
                R[1] = 2*(0.5-y2);//(2*(1-y2)-1);
                R[2] = 2*(x2-0.5);
                R[3] = 2*(0.5-y1);

                glUniform4fv(locations["scissor"],1,(const GLfloat*)&R);
            }*/
                options.scissor = rcmd.r;
            break;

            case(POINTSIZE):
                //glPointSize(rcmd.f);
                options.pointSize = rcmd.f;
            break;

            case(LINEWIDTH):
                //glLineWidth(rcmd.f);
                options.lineWidth = rcmd.f;
            break;

            /*case(PRINT_TEXT):{
                if(!renderState.fnt){throw ex("no font selected\n");}
                //for convenience, ignores
                auto m = renderState.renderMode;
                renderState.renderMode = 3;
                printText2D(rcmd.s->c_str(),renderState.fnt,renderState.textPos);
                renderState.renderMode = m;
                if(debug){printf("done printing\n");}
            } break;*/

            case(COMMENT):
                if(options.debug){printf("%s", rcmd.s->c_str());}
                //delete rcmd.s; owned by command now
                  //should have some way of adding/removing commands i.e. stream goes in, different stream comes out
                passCommand = false;
            break;

            case(DEPTH_TEST):
                //if(rcmd.b){glEnable(GL_DEPTH_TEST);}
                //else{glDisable(GL_DEPTH_TEST);}
                options.depthTest = rcmd.b;
            break;

            case(LIGHTING):
                //glUniform1i(locations["lightingOn"],rcmd.b);
                options.lighting = rcmd.b;
            break;
            case(SUN_POS):{
                //vec3 v3 = normalize(rcmd.v3);
                //glUniform3fv(locations["sunPos"],1,(const GLfloat*)&v3);
                options.sunPos = rcmd.v3;
            }break;

            case(SUN_LIGHT_COLOR):
                //glUniform3fv(locations["sunColor"],    1,(const GLfloat*)&rcmd.v3);
                options.sunLightColor = rcmd.v3;
            break;

            case(AMBIENT_LIGHT_COLOR):
                //glUniform3fv(locations["ambientColor"],1,(const GLfloat*)&rcmd.v3);
                options.ambientLightColor = rcmd.v3;
            break;

            case(PUSH_OPTIONS):
                optionStack.push_back(options);
                passCommand = false;
            break;

            case(POP_OPTIONS):
                if(optionStack.size()){
                    options = optionStack.back();
                    optionStack.pop_back();
                    options.applyImmediate();
                }
                passCommand = false;
            break;
        }
        if(passCommand){
            GPUdriver->parseCommand(rcmd);
        }
}

/* this moved to paint.cpp
void projectionToCamera(){
	//renderCmd(RCMD::PROJECTION, m = camera.mProjection*camera.mView);
	rqueue->push_back(new rcmd_projection(camera.getProjection()));
}
*/
// void unloadModel(rmodel *rm){
	////can't delete immediately because it might not have been drawn yet
	// renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[0]);
	// renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[1]);
	// renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[2]);
	// renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[3]);
	// rm->handle[0] = 0;
	// rm->handle[1] = 0;
	// rm->handle[2] = 0;
	// rm->handle[3] = 0;
// }


/*
void uploadModel(model *m){
	m->rm = new rmodel(m);
	m->rm->finalize();
	rqueue->push_back(new rcmd_texture_upload(m->t));
	rqueue->push_back(new rcmd_rmodel_upload(m->rm));
	//uploadModel(m->rm);
}
*/
/*
void renderModel(model *m){
	//renderCmd(RCMD::COLORING, b=false);
	//renderCmd(RCMD::TEXTURING, b=true);
	//renderCmd(RCMD::TEXTURE_SELECT,i=m->t.handle.t);
	//renderModel(RCMD::DRAW_TRIANGLES, m->rm);
	rqueue->push_back(new rcmd_coloring(false));
	rqueue->push_back(new rcmd_texturing(true));
	rqueue->push_back(new rcmd_texture_select(m->t));
	rqueue->push_back(new rcmd_rmodel_render(m->rm,3));
}
*/
bool debug = false;




void rcmd_layer::execute(){
	if(val){
		renderLayer *L = (renderLayer*)val;
		L->render();
	}
}

void rcmd_coloring::execute(){
	//if(debug){printf("coloring %d\n",val);}
	glUniform1i(renderLow->GPUdriver->locations["coloringOn"],val);
}

void rcmd_uvcoloring::execute(){
	glUniform1i(renderLow->GPUdriver->locations["uvColoringOn"],val);
}

void rcmd_normalcoloring::execute(){
	glUniform1i(renderLow->GPUdriver->locations["normalColoringOn"],val);
}

void rcmd_transparency::execute(){
	//if(debug){printf("transparency %d\n",val);}
	if(val){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else{
		glDisable(GL_BLEND);
	}
	glUniform1i(renderLow->GPUdriver->locations["transparencyOn"],val);
}

void rcmd_depthmask::execute(){
	if(val){
		glDepthMask(GL_TRUE);
	}else{
		glDepthMask(GL_FALSE);
	}
}

void rcmd_texturing::execute(){
	//if(debug){printf("texturing %d\n",val);}
	glUniform1i(renderLow->GPUdriver->locations["texturingOn"],val);
}

void rcmd_debug::execute(){
	//if(debug){printf("debugging %d\n",val);}
	glUniform1i(renderLow->GPUdriver->locations["debuggingOn"],val);
}

void reproject(){
	vec3 mpos = renderLow->GPUdriver->renderState.pos;
	float mrot_ang = length(renderLow->GPUdriver->renderState.rot);
	vec3 mrot_axis = normalizeSafe(renderLow->GPUdriver->renderState.rot);
	vec3 mscale = renderLow->GPUdriver->renderState.scale;
	mat4 VP = mat4();
	mat4 M = mat4();
	mat4 Mrot = mat4();

	VP = renderLow->GPUdriver->renderState.VP;
	M = glm::translate(mat4(), mpos);
	if(mrot_ang){M = glm::rotate(M,mrot_ang,mrot_axis);}
	M = glm::scale(M,mscale);

	if(mrot_ang){Mrot = glm::rotate(mat4(),mrot_ang,mrot_axis);}

	vec3 campos = renderLow->GPUdriver->renderState.camPos;

	glUniformMatrix4fv(renderLow->GPUdriver->locations["VP"],1,GL_FALSE,(const GLfloat*)&VP);
	glUniformMatrix4fv(renderLow->GPUdriver->locations["M"],1,GL_FALSE,(const GLfloat*)&M);
	glUniformMatrix4fv(renderLow->GPUdriver->locations["Mrot"],1,GL_FALSE,(const GLfloat*)&Mrot);
	glUniform3fv(renderLow->GPUdriver->locations["WorldCamPos"],1,(const GLfloat*)&campos);
}

void rcmd_projection::execute(){
	//if(debug){printf("projection\n");}
	renderLow->GPUdriver->renderState.VP = val.MVP;
	renderLow->GPUdriver->renderState.camPos = val.pos;
	reproject();
	//glUniformMatrix4fv(locations["MVP"],1,GL_FALSE,(const GLfloat*)&val);
}

void rcmd_position::execute(){
	//if(debug){printf("position\n");}
	renderLow->GPUdriver->renderState.pos = val;
	reproject();

}

void rcmd_rotation::execute(){
	renderLow->GPUdriver->renderState.rot = val;
	reproject();
}

void rcmd_scale::execute(){
	//if(debug){printf("scale\n");}
	renderLow->GPUdriver->renderState.scale = val;
	reproject();
}

int num_textures = 0;
long bytes_textures = 0;
void rcmd_texture_upload::execute(){
	texture *t = val;
	if(!val){error("attempt to upload null texture\n");}
	if(renderLow->GPUdriver->texture_GPU_handles.count(t)){return;}//texture already uploaded.
	if(renderLow->GPUdriver->bitmap_GPU_handles.count(t->bmp)){return;}//texture already uploaded.
	if(t->bmp){
		while(glGetError() != GL_NO_ERROR){}
		num_textures++;
		bytes_textures += t->bmp->width*t->bmp->height*4;
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D,textureID);
		glTexImage2D(GL_TEXTURE_2D,0,pixelFormatToGL(t->bmp->format), t->bmp->width, t->bmp->height, 0, pixelFormatToGL(t->bmp->format), GL_UNSIGNED_BYTE, t->bmp->data.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		renderLow->GPUdriver->bitmap_GPU_handles[t->bmp] = textureID;
		printf("texture '%s' uploaded: %d (%dx%d)\n",t->name.c_str(),textureID,t->bmp->height,t->bmp->width);

		int err = glGetError();
		if(err){
			if(err == GL_OUT_OF_MEMORY){error("Error: GL_OUT_OF_MEMORY");}
			string errstr = string()+"GL Error: "+err;
			error(errstr.c_str());
		}
	}else{
		error("no texture data to upload (%s)\n",t->name.c_str());
	}
}

void rcmd_texture_select::execute(){
	texture *t = val;
	if(!val){error("attempt to select null texture\n");}
	GLuint handle = 0;
	//render_sys_data_texture rdata;
	if(t->bmp){
		if(!renderLow->GPUdriver->bitmap_GPU_handles.count(t->bmp)){error("image not uploaded: \"%s\"/%p\n",t->name.c_str(),t->bmp);}
		handle = renderLow->GPUdriver->bitmap_GPU_handles[t->bmp];
	}else{
		if(!renderLow->GPUdriver->texture_GPU_handles.count(t)){error("texture not uploaded: %s\n",t->name.c_str());}
		handle = renderLow->GPUdriver->texture_GPU_handles[t];
	}
	if(debug){printf("texture select %s (%d)\n",t->name.c_str(), handle);}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);
	glUniform1i(renderLow->GPUdriver->locations["Tex"],0);//wat
	vec2 offset = vec2(t->UV.start.x,t->UV.start.y);
	vec2 size = vec2(t->UV.size.x,t->UV.size.y);
	//offset = vec2(0,0);
	//size = vec2(1,1);
	//printf("tex offset: (%f x %f), size: (%f x %f)\n",offset.x,offset.y,size.x,size.y);
	glUniform2fv(renderLow->GPUdriver->locations["TexOffset"],1,(GLfloat*)&offset);
	glUniform2fv(renderLow->GPUdriver->locations["TexSize"],1,(GLfloat*)&size);
}

int num_rmodels = 0;
long bytes_rmodels = 0;

void rcmd_rmodel_upload::execute(){
	rmodel *rm = val;
	if(!val){error("attempt to upload null rmodel\n");}
	//if(debug){printf("uploading rmodel %p\n",rm);}
	if(
		(!rm->vertices)||(!rm->normals)||(!rm->colors)||(!rm->uvs)||
		(rm->normals->size() != rm->vertices->size())||
		(rm->colors->size() != rm->vertices->size())||
		(rm->uvs->size() != rm->vertices->size())
	){
		error("trying to upload incomplete rmodel");
	}
	num_rmodels++;
	bytes_rmodels += rm->vertices->size()*4*11;
	while(glGetError() != GL_NO_ERROR){}
	GLuint handles[4];
	glGenBuffers(4,handles); //fills handle[0,1,2,3].


	//verts
	glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->vertices->data(), GL_STATIC_DRAW);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->colors->data(), GL_STATIC_DRAW);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->normals->data(), GL_STATIC_DRAW);

	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
	glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec2), rm->uvs->data(), GL_STATIC_DRAW);

	for(int i = 0; i<4; i++){renderLow->GPUdriver->rmodel_GPU_handles[rm][i] = handles[i];}

	int err = glGetError();
	if(err){
		if(err == GL_OUT_OF_MEMORY){error("Error: GL_OUT_OF_MEMORY");}
		string errstr = string()+"GL Error: "+err;
		error(errstr.c_str());
	}
}

void rcmd_rmodel_render::execute(){
	rmodel *rm = val;
	if(!val){error("attempt to render null rmodel\n");}
	//if(debug){printf("rendering rmodel %p\n",rm);}
	GLuint handles[4];
	for(int i = 0; i<4; i++){handles[i] = renderLow->GPUdriver->rmodel_GPU_handles[rm][i];}
	//verts
	glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	//colors
	glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
	glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	switch(renderLow->GPUdriver->renderState.renderMode){
		case(1):
			glDrawArrays(GL_POINTS,0,rm->vertices->size());
			break;
		case(2):
			glDrawArrays(GL_LINES,0,rm->vertices->size());
			break;
		case(3):
			glDrawArrays(GL_TRIANGLES,0,rm->vertices->size());
			break;
		default:
			error("unknown render mode '%d'\n",renderLow->GPUdriver->renderState.renderMode);
			break;
	}
}

void rcmd_rmodel_delete::execute(){
	rmodel *rm = val;
    if(wasRecentlyDeleted(rm)){error("double delete\n");}
	if(!val){error("attempt to delete null rmodel\n");}
	//if(debug){printf("deleting rmodel %p\n",rm);}
	num_rmodels--;
	bytes_rmodels -= rm->vertices->size()*4*11;
	GLuint handles[4];
	for(int i = 0; i<4; i++){handles[i] = renderLow->GPUdriver->rmodel_GPU_handles[rm][i];}
	glDeleteBuffers(4,handles);
	renderLow->GPUdriver->rmodel_GPU_handles.erase(rm);
}

void rcmd_clear_screen::execute(){
	//if(debug){printf("clearing screen\n");}
	glClearColor(125/255.0f,206/255.0f,250/255.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void rcmd_scissoring::execute(){
	//if(debug){printf("scissoring: %d\n",val);}
	glUniform1i(renderLow->GPUdriver->locations["scissoringOn"],val);
}

void rcmd_color::execute(){
	//if(debug){printf("set color to %d %d %d\n",val.x,val.y,val.z);}
	vec3 val2 = val;
	val2.x /= 255.0f;
	val2.y /= 255.0f;
	val2.z /= 255.0f;
	glUniform3fv(renderLow->GPUdriver->locations["globalColor"],1,(const GLfloat*)&val2);
}

void rcmd_alpha::execute(){
	//if(debug){printf("set alpha to %f\n",val);}
	float val2 = val/255.0f;
	glUniform1fv(renderLow->GPUdriver->locations["globalAlpha"],1,(const GLfloat*)&val2);
}

void rcmd_font_select::execute(){
	if(!val){error("attempt to select null font\n");}
	//if(debug){printf("selecting font %s (%p)\n",val->name.c_str(),val);}
	renderLow->GPUdriver->renderState.fnt = val;
}

void rcmd_mode_select::execute(){
	//if(debug){printf("selecting render mode %d\n",val);}
	renderLow->GPUdriver->renderState.renderMode = val;
}

void rcmd_text_pos::execute(){
	//if(debug){printf("set text pos to %f,%f\n",val.x,val.y);}
	renderLow->GPUdriver->renderState.textPos = val;
}

void rcmd_scissor::execute(){
	//if(debug){printf("set scissor to %dx%d\n",val.size.x,val.size.y);}
	float R[4];
	//R[0] = val.start.x;
	//R[1] = val.start.y;
	//R[2] = val.end.x;
	//R[3] = val.end.y;
	float x1 = ((float)val.start.x)/width;
	float y1 = ((float)(val.start.y))/height;
	float x2 = ((float)val.end.x)/width;
	float y2 = ((float)(val.end.y))/height;
	R[0] = 2*(x1-0.5);//(2*(x2)-1);
	R[1] = 2*(0.5-y2);//(2*(1-y2)-1);
	R[2] = 2*(x2-0.5);
	R[3] = 2*(0.5-y1);

	glUniform4fv(renderLow->GPUdriver->locations["scissor"],1,(const GLfloat*)&R);
}
void rcmd_pointsize::execute(){
	//if(debug){printf("set point size to %f\n",val);}
	glPointSize(val);
}

void rcmd_linewidth::execute(){
	//if(debug){printf("set line width to %f\n",val);}
	glLineWidth(val);
}

void rcmd_print_text::execute(){
	//if(debug){printf("printing text '%s'\n",val.c_str());}
	if(!renderLow->GPUdriver->renderState.fnt){error("no font selected\n");}
	//for convenience, ignores
	auto m = renderLow->GPUdriver->renderState.renderMode;
	renderLow->GPUdriver->renderState.renderMode = 3;
	printText2D(val.c_str(),renderLow->GPUdriver->renderState.fnt,renderLow->GPUdriver->renderState.textPos);
	renderLow->GPUdriver->renderState.renderMode = m;
	if(debug){printf("done printing\n");}
}

void rcmd_comment::execute(){
	if(debug){printf("%s", val.c_str());}
}

void rcmd_depth_test::execute(){
	//if(debug){printf("set depth test %d\n",val);}
	if(val){glEnable(GL_DEPTH_TEST);}
	else{glDisable(GL_DEPTH_TEST);}
}

void rcmd_lighting::execute()			{glUniform1i( renderLow->GPUdriver->locations["lightingOn"],val);}//{renderState.lighting = val;}
void rcmd_sun_pos::execute(){
	//vec4 vp = vec4(val,0)*renderState.VP;
	//vp /= vp.w;
	//val = vp;
	val = normalize(val);

	glUniform3fv(renderLow->GPUdriver->locations["sunPos"],1,(const GLfloat*)&val);
}//{renderState.sun_pos = val;}
void rcmd_sun_light_color::execute()	{glUniform3fv(renderLow->GPUdriver->locations["sunColor"],    1,(const GLfloat*)&val);}//{renderState.sun_brightness = val;}
void rcmd_ambient_light_color::execute(){glUniform3fv(renderLow->GPUdriver->locations["ambientColor"],1,(const GLfloat*)&val);}//{renderState.ambient_brightness = val;}
// void uploadModel(rmodel *rm){

	////the number of primitives in a model is determined
	////by the number of vertices.
	////we need to either use dummy buffers or extend
	////the other attribute data to cover all the vertices
	////(vertices.size == colors.size == normals.size == uvs.size)

	////while(rm->colors->size() < rm->vertices->size()){rm->colors->push_back(vec3(1.0f,1.0f,1.0f));}//default color is white
	////while(rm->normals->size() < rm->vertices->size()){rm->normals->push_back(vec3(0.0f,0.0f,0.0f));}
	////while(rm->uvs->size() < rm->vertices->size()){rm->uvs->push_back(vec2(0.0f,0.0f));}

	// glGenBuffers(1,(GLuint*)&rm->handle[0]);
	// renderCmd(RCMD::VERTICES_SELECT,i=rm->handle[0]);
	// renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->vertices);


	// glGenBuffers(1,(GLuint*)&rm->handle[1]);
	// renderCmd(RCMD::COLORS_SELECT, i=rm->handle[1]);
	// renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->colors);

	// glGenBuffers(1,(GLuint*)&rm->handle[2]);
	// renderCmd(RCMD::NORMALS_SELECT, i=rm->handle[2]);
	// renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->normals);

	// glGenBuffers(1,(GLuint*)&rm->handle[3]);
	// renderCmd(RCMD::UVS_SELECT,i=rm->handle[3]);
	// renderCmd(RCMD::BUFFER_VEC2_LOAD,p=rm->uvs);
// }


// void renderModel(RCMD command, rmodel *rm){
	// renderCmd(RCMD::VERTICES_SELECT,i=rm->handle[0]);
	// renderCmd(RCMD::COLORS_SELECT,i=rm->handle[1]);
	// renderCmd(RCMD::NORMALS_SELECT,i=rm->handle[2]);
	// renderCmd(RCMD::UVS_SELECT,i=rm->handle[3]);
	// renderCmd(command,i=rm->vertices->size());
// }

/*
void renderTest1(){
	GLuint programHandle = uploadShaderProgram("shaders/vertShader.vert","shaders/fragShader.frag");

	glBindAttribLocation(programHandle,0, "VertexPosition");
	glBindAttribLocation(programHandle,1, "VertexColor");

	//GLuint vaoHandle;

	float positionData[] =
	{
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};

	float colorData[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), colorData, GL_STATIC_DRAW);

	//vertex array object stores the relationship between the buffers
	//and the input attributes
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	printf("renderLow: test1 OK\n");
}

void renderTest1Tick(){

	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES,0,3);

	OpenGL_swap();
}
*/
// textureHandle uploadImage(bitmap BMP){
	// GLuint textureID;
	// glGenTextures(1, &textureID);
	// glBindTexture(GL_TEXTURE_2D, textureID);
	// glGetError();
	// glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatToGL(BMP.format), BMP.width, BMP.height, 0, pixelFormatToGL(BMP.format), GL_UNSIGNED_BYTE, BMP.data.get());
	// GLenum error = glGetError();
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// textureHandle h;
	// h.BMP = BMP;
	// h.t = textureID;
	// printf("texture %p uploaded to GPU: %d (%dx%d)\n",BMP.data,textureID,BMP.height,BMP.width);
	// return h;
// }

// void renderParseCommandBuffer(renderCommandBuffer *cmdbuff){
	// vector<vec3> *v;
	// vector<vec2> *v2;
	// rmodel *rm;
	////bool debug = false;
	// for(auto I = cmdbuff->begin(); I != cmdbuff->end(); I++){
		// switch(I->type){
			// case(RCMD::VERTICES_SELECT):
				// if(debug){printf("verts select %d\n",I->i);}
				// glBindBuffer(GL_ARRAY_BUFFER, I->i);
				// glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				// break;
			// case(RCMD::COLORS_SELECT):
				// if(debug){printf("colors select %d\n",I->i);}
				// glBindBuffer(GL_ARRAY_BUFFER, I->i);
				// glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				// break;
			// case(RCMD::NORMALS_SELECT):
				// if(debug){printf("normals select %d\n",I->i);}
				// glBindBuffer(GL_ARRAY_BUFFER, I->i);
				// glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				// break;
			// case(RCMD::UVS_SELECT):
				// if(debug){printf("uvs select %d\n",I->i);}
				// glBindBuffer(GL_ARRAY_BUFFER, I->i);
				// glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				// break;
			// case(RCMD::BUFFER_VEC3_LOAD):
				// if(debug){printf("buffer vec3 load %p\n",I->p);}
				// v = (vector<vec3> *)I->p;
				////if(debug){for(int i = 0; i < v->size(); i++){printf("v[%d] = (%f,%f,%f)\n",i,(*v)[i].x,(*v)[i].y,(*v)[i].z);}}
				// glBufferData(GL_ARRAY_BUFFER, v->size()*sizeof(vec3), v->data(), GL_STATIC_DRAW);
				// numVerts = v->size();
				// break;
			// case(RCMD::BUFFER_VEC2_LOAD):
				// if(debug){printf("buffer vec2 load %p\n",I->p);}
				// v2 = (vector<vec2> *)I->p;
				// glBufferData(GL_ARRAY_BUFFER, v2->size()*sizeof(vec2), v2->data(), GL_STATIC_DRAW);
				// numVerts = v->size();
				// break;
			// case(RCMD::BUFFER_DELETE):
				// if(debug){printf("buffer delete %i\n",I->i);}
				// glDeleteBuffers(1,(GLuint*)&I->i);
				// break;
			// case(RCMD::RMODEL_DELETE):
				// if(debug){printf("rmodel delete %p\n",I->p);}
				// rm = (rmodel*)I->p;
				// delete rm->colors; //go fast, blobassus
				// delete rm->vertices; //karate lessons
				// delete rm->uvs;		//sunscreen
				// delete rm->normals;	//pants that fit
				// rm->colors = 0;
				// rm->vertices = 0;
				// rm->uvs = 0;
				// rm->normals = 0;
				// delete rm; //it's all yours
				// break;
			// case(RCMD::DRAW_TRIANGLES):
				// if(debug){printf("draw triangles %d =============\n",I->i);}
				// glBindVertexArray(vaoHandle);
				// glDrawArrays(GL_TRIANGLES,0,I->i);
				////printf("+");
				// break;
			// case(RCMD::DRAW_LINES):
				// if(debug){printf("draw triangles %d =============\n",I->i);}
				// glBindVertexArray(vaoHandle);
				// glDrawArrays(GL_LINES,0,I->i);
				////printf("+");
				// break;
			// case(RCMD::DRAW_POINTS):
				// if(debug){printf("draw triangles %d =============\n",I->i);}
				// glBindVertexArray(vaoHandle);
				// glDrawArrays(GL_POINTS,0,I->i);
				////printf("+");
				// break;
			// case(RCMD::PROJECTION):
				// if(debug){printf("projection\n");}
				// glUniformMatrix4fv(locations["MVP"],1,GL_FALSE,(const GLfloat*)&(I->m));
				////printf("p");
				// break;
			// case(RCMD::TEXTURE_SELECT):
				// if(debug){printf("texture select %d\n",I->i);}
				// glActiveTexture(GL_TEXTURE0);
				// glBindTexture(GL_TEXTURE_2D, I->i);
				// glUniform1i(locations["Tex"],0);//I->i);
				////printf("t");
				// break;
			// case(RCMD::TRANSPARENCY):
				// if(debug){printf("transparency %d\n",I->b);}
				// if(I->b){
					// glEnable(GL_BLEND);
					// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				// }else{
					// glDisable(GL_BLEND);
				// }
				// glUniform1i(locations["transparencyOn"],I->b);
				// break;
			// case(RCMD::TEXTURING):
				// if(debug){printf("texturing %d\n",I->b);}
				// glUniform1i(locations["texturingOn"],I->b);
				// break;
			// case(RCMD::COLORING):
				// if(debug){printf("coloring %d\n",I->b);}
				// glUniform1i(locations["coloringOn"],I->b);
				// break;
			// case(RCMD::DEBUG):
				// if(debug){printf("debugging %d\n",I->b);}
				// glUniform1i(locations["debuggingOn"],I->b);
				// break;
			// default:
				// error("unknown command!");
				// break;
		// }
	// }
	// cmdbuff->clear();
// }
