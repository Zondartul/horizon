#include "program/file.h"
#include "render/GPUdriver.h"
#include "resource/texture.h"
#include "render/printw.h"
#include "util/util.h"
//#include "util/debug.h"
#include "util/globals_render.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
using namespace std;



void GPUdriverKind::renderLowInit(){
#ifndef NO_GLEW

	GLenum err = glewInit();
	if(GLEW_OK != err){
		//error("Error initializing GLEW: %s\n", glewGetErrorString(err));
        stringstream ss;
        ss << "Error initializing GLEW: " << glewGetErrorString(err);
        throw runtime_error(ss.str());
    }


	GLuint programHandle = uploadShaderProgram("code/shaders/vertShader.vert","code/shaders/fragShader.frag");

	glBindAttribLocation(programHandle,0, "VertexPosition");
	glBindAttribLocation(programHandle,1, "VertexColor");
	glBindAttribLocation(programHandle,2, "VertexNormal");
	glBindAttribLocation(programHandle,3, "VertexUV");

	//vertex array object stores the relationship between the buffers
	//and the input attributes
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	locations["VP"] = 0;
	locations["M"] = 0;
	locations["Mrot"] = 0;
	locations["Tex"] = 0;
	locations["texturingOn"] = 0;
	locations["coloringOn"] = 0;
	locations["uvColoringOn"] = 0;
	locations["normalColoringOn"] = 0;
	locations["debuggingOn"] = 0;
	locations["transparencyOn"] = 0;
	locations["TexOffset"] = 0;
	locations["TexSize"] = 0;
	locations["globalColor"] = 0;
	locations["globalAlpha"] = 0;
	locations["scissoringOn"] = 0;
	locations["scissor"] = 0;
	locations["lightingOn"] = 0;
	locations["sunPos"] = 0;
	locations["sunColor"] = 0;
	locations["ambientColor"] = 0;
	locations["WorldCamPos"] = 0;
	locations["TexPos"] = 0;
	locations["TexScale"] = 0;
	for(auto I = locations.begin(); I != locations.end(); I++){
		I->second = glGetUniformLocation(programHandle,I->first.c_str());
		printf("uniform '%s' at '%d'\n",I->first.c_str(),I->second);
	}

	printf("renderLow: init OK\n");
#else
    printf("NO GLEW, renderlow quits\n");
#endif
}

#ifndef NO_GLEW
GLuint GPUdriverKind::uploadShader(GLuint shaderType, const char *filepath){
	GLuint shader = glCreateShader(shaderType);
	if(0 == shader){printf("Error creating shader\n");exit(1);}

	string s = fileToString(filepath);
	printf("reading file %s:\n"
			"------------------------------------------------\n"
			"%s\n"
			"------------------------------------------------\n",filepath,s.c_str());



	const GLchar *codeArray[] = {s.c_str()};
	glShaderSource(shader, 1, codeArray, NULL);

	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(GL_FALSE == result){
		printf("Vertex shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logLen);
		if(logLen>0){
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader,logLen,&written,log);
			printf("shader log:\n"
			"------------------------------------------------\n"
			"%s\n"
			"------------------------------------------------\n",log);
			free(log);
		}
		exit(1);
	}
	printf("shader uploaded\n");
	return shader;
}

GLuint GPUdriverKind::uploadShaderProgram(const char *filepath_vertex, const char *filepath_fragment){
	GLuint vertShader = uploadShader(GL_VERTEX_SHADER, filepath_vertex);
	GLuint fragShader = uploadShader(GL_FRAGMENT_SHADER, filepath_fragment);

	GLuint programHandle = glCreateProgram();
	if(0 == programHandle){
		//error("error creating program object\n");
        throw runtime_error("error creating shader program object");
    }

	glAttachShader(programHandle,vertShader);
	glAttachShader(programHandle,fragShader);

	glLinkProgram(programHandle);

	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if(GL_FALSE == status){
		printf("Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0){
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			printf("log:\n"
			"------------------------------------------------\n"
			"%s\n"
			"------------------------------------------------\n",log);
			free(log);
		}
		exit(1);
	}else{
		glUseProgram(programHandle);
	}
	return programHandle;
}
#endif

void GPUdriverKind::reproject(){
	vec3 mpos = renderState.pos;
	float mrot_ang = length(renderState.rot);
	vec3 mrot_axis = normalizeSafe(renderState.rot);
	vec3 mscale = renderState.scale;
	mat4 VP = mat4();
	mat4 M = mat4();
	mat4 Mrot = mat4();

	VP = renderState.VP;
	M = glm::translate(mat4(), mpos);
	if(mrot_ang){M = glm::rotate(M,mrot_ang,mrot_axis);}
	M = glm::scale(M,mscale);

	if(mrot_ang){Mrot = glm::rotate(mat4(),mrot_ang,mrot_axis);}

	vec3 campos = renderState.camPos;
#ifndef NO_GLEW
	glUniformMatrix4fv(locations["VP"],1,GL_FALSE,(const GLfloat*)&VP);
	glUniformMatrix4fv(locations["M"],1,GL_FALSE,(const GLfloat*)&M);
	glUniformMatrix4fv(locations["Mrot"],1,GL_FALSE,(const GLfloat*)&Mrot);
	glUniform3fv(locations["WorldCamPos"],1,(const GLfloat*)&campos);
#endif
}

void GPUdriverKind::parseQueue(renderQueue3 *rqueue){
    for(auto I = rqueue->begin(); I != rqueue->end(); I++){
        renderCommand3 &rcmd = **I;
        parseCommand(rcmd);

    }
}


std::ostream& operator<<(std::ostream& stream, rect R) {
    stream << "(" << R.start.x << ", " << R.start.y <<" x " << R.size.x << ", " << R.size.y << ")";
    return stream;
}

void GPUdriverKind::command_layer(renderLayer* layer) {if (layer) { layer->render(); }}
void GPUdriverKind::command_coloring(bool b){glUniform1i(locations["coloringOn"], b);}
void GPUdriverKind::command_uvcoloring(bool b){ glUniform1i(locations["uvColoringOn"], b);}
void GPUdriverKind::command_normalcoloring(bool b) {glUniform1i(locations["normalColoringOn"], b);}
void GPUdriverKind::command_transparency(bool b) {
    if (b) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {glDisable(GL_BLEND);}
    glUniform1i(locations["transparencyOn"], b);
}
void GPUdriverKind::command_depthmask(bool b) {
    if (b) {glDepthMask(GL_TRUE);}
    else {glDepthMask(GL_FALSE);}
}
void GPUdriverKind::command_texturing(bool b){ glUniform1i(locations["texturingOn"], b); }
void GPUdriverKind::command_debugging(bool b){ glUniform1i(locations["debuggingOn"], b); }
void GPUdriverKind::command_projection(camprojection* cam) {
    renderState.VP = cam->MVP;
    renderState.camPos = cam->pos;
    //delete rcmd.cam; owned by command now.
    reproject();
}
void GPUdriverKind::command_position(vec3 v) {
    renderState.pos = v;
    reproject();
}
void GPUdriverKind::command_rotation(vec3 v) {
    renderState.rot = v;
    reproject();
}
void GPUdriverKind::command_scale(vec3 v) {
    renderState.scale = v;
    reproject();
}
void GPUdriverKind::command_texture_upload(texture* t) {
    //texture* t = rcmd.t;
    //if(wasRecentlyDeleted(t)){error("accessing a deleted texture");}
    if (!t) {
        //error("attempt to upload null texture\n");
        throw runtime_error("attempt to upload a null texture");
    }

    if (texture_GPU_handles.count(t)) { return; }//texture already uploaded.
    if (bitmap_GPU_handles.count(t->bmp)) { return; }//texture already uploaded.

    if (t->bmp) {
        while (glGetError() != GL_NO_ERROR) {}
        printf("GPUdriver: TEX_UPLOAD for (%s)\n", toCString(t));
        num_textures++;
        bytes_textures += t->bmp->width * t->bmp->height * 4;
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        vector<unsigned char>* V = &*(t->bmp->data);
        unsigned char* buff = V->data();
        glTexImage2D(GL_TEXTURE_2D, 0, pixelFormatToGL(t->bmp->format), t->bmp->width, t->bmp->height, 0, pixelFormatToGL(t->bmp->format), GL_UNSIGNED_BYTE, buff);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        bitmap_GPU_handles[t->bmp] = textureID;
        printf("texture '%s' uploaded: %d (%dx%d)\n", t->name.c_str(), textureID, t->bmp->height, t->bmp->width);

        int err = glGetError();
        if (err) {
            if (err == GL_OUT_OF_MEMORY) {
                //error("Error: GL_OUT_OF_MEMORY");
                throw runtime_error("Error: GL_OUT_OF_MEMORY");
            }
            stringstream ss;
            ss << "GL Error: " << err;
            //error(ss.str().c_str());
            throw runtime_error(ss.str());
        }
    }
    else {
        //error("no texture data to upload (%s)\n",t->name.c_str());
        stringstream ss;
        ss << "no texture data to upload (" << t->name << ")";
        throw runtime_error(ss.str());
    }
    
}
void GPUdriverKind::command_texture_select(texture* t) {
    //texture* t = rcmd.t;
    //if(wasRecentlyDeleted(t)){error("accessing a deleted texture");}
    if (!t) {
        //error("attempt to select null texture\n");
        throw runtime_error("attempt to select null texture");
    }
    GLuint handle = 0;
    //render_sys_data_texture rdata;
    if (t->bmp) {
        if (!bitmap_GPU_handles.count(t->bmp)) {
            //error("image not uploaded: \"%s\"/%p\n",t->name.c_str(),t->bmp);
            stringstream ss;
            ss << "image not uploaded: \"" << t->name << "\"/" << (void*)(t->bmp);
            throw runtime_error(ss.str());
        }
        handle = bitmap_GPU_handles[t->bmp];
    }
    else {
        if (!texture_GPU_handles.count(t)) {
            //error("texture not uploaded: %s\n",t->name.c_str());
            stringstream ss;
            ss << "texture not uploaded: " << t->name;
            throw runtime_error(ss.str());
        }
        handle = texture_GPU_handles[t];
    }
    if (debug) { printf("texture select %s (%d)\n", t->name.c_str(), handle); }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
    glUniform1i(locations["Tex"], 0);//wat
    vec2 offset = vec2(t->UV.start.x, t->UV.start.y);
    vec2 size = vec2(t->UV.size.x, t->UV.size.y);
    //offset = vec2(0,0);
    //size = vec2(1,1);
    //printf("tex offset: (%f x %f), size: (%f x %f)\n",offset.x,offset.y,size.x,size.y);
    glUniform2fv(locations["TexOffset"], 1, (GLfloat*)&offset);
    glUniform2fv(locations["TexSize"], 1, (GLfloat*)&size);
    
}
void GPUdriverKind::command_texture_pos(vec2 v) {glUniform2fv(locations["TexPos"], 1, (GLfloat*)&v);}
void GPUdriverKind::command_texture_scale(vec2 v) {glUniform2fv(locations["TexScale"], 1, (GLfloat*)&v);}
void GPUdriverKind::command_rmodel_upload(rmodel* rm) {
    //rmodel* rm = rcmd.rm;
    //if(wasRecentlyDeleted(rm)){
    //    //error("accessing a deleted rmodel");
    //    throw runtime_error("accessing a deleted rmodel");
    //}
    if (!rm) {
        //error("attempt to upload null rmodel\n");
        throw runtime_error("attempt to upload null rmodel");
    }
    //if(debug){printf("uploading rmodel %p\n",rm);}
    if (
        (!rm->vertices) || (!rm->normals) || (!rm->colors) || (!rm->uvs) ||
        (rm->normals->size() != rm->vertices->size()) ||
        (rm->colors->size() != rm->vertices->size()) ||
        (rm->uvs->size() != rm->vertices->size())
        ) {
        //error("trying to upload incomplete rmodel");
        throw runtime_error("trying to upload incomplete rmodel");
    }
    num_rmodels++;
    bytes_rmodels += rm->vertices->size() * 4 * 11;
    while (glGetError() != GL_NO_ERROR) {}
    GLuint handles[4];
    glGenBuffers(4, handles); //fills handle[0,1,2,3].


    //verts
    glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size() * sizeof(vec3), rm->vertices->data(), GL_STATIC_DRAW);

    //colors
    glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size() * sizeof(vec3), rm->colors->data(), GL_STATIC_DRAW);

    //normals
    glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size() * sizeof(vec3), rm->normals->data(), GL_STATIC_DRAW);

    //uvs
    glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size() * sizeof(vec2), rm->uvs->data(), GL_STATIC_DRAW);

    for (int i = 0; i < 4; i++) { rmodel_GPU_handles[rm][i] = handles[i]; }

    int err = glGetError();
    if (err) {
        if (err == GL_OUT_OF_MEMORY) {
            //error("Error: GL_OUT_OF_MEMORY");
            throw runtime_error("Error: GL_OUT_OF_MEMORY");
        }
        //string errstr = string()+"GL Error: "+err;
        //error(errstr.c_str());
        stringstream ss;
        ss << "GL Error: " << err;
        throw runtime_error(ss.str());
    }

}

void GPUdriverKind::command_rmodel_render(rmodel* rm) {
    //rmodel* rm = rcmd.rm;
    //if(wasRecentlyDeleted(rm)){
    //    //error("accessing a deleted rmodel");
    //    throw runtime_error("accessing a deleted rmodel");
    //}
    if (!rm) {
        //error("attempt to render null rmodel\n");
        throw runtime_error("attempt to render null rmodel");
    }
    //if(debug){printf("rendering rmodel %p\n",rm);}
    GLuint handles[4];
    for (int i = 0; i < 4; i++) { handles[i] = rmodel_GPU_handles[rm][i]; }
    //verts
    glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    //colors
    glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    //normals
    glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    //uvs
    glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    switch (renderState.renderMode) {
    case(1):
        glDrawArrays(GL_POINTS, 0, rm->vertices->size());
        break;
    case(2):
        glDrawArrays(GL_LINES, 0, rm->vertices->size());
        break;
    case(3):
        glDrawArrays(GL_TRIANGLES, 0, rm->vertices->size());
        break;
    default:
        // error("unknown render mode '%d'\n",renderState.renderMode);
        stringstream ss;
        ss << "unknwon render mode '" << renderState.renderMode << "'";
        throw runtime_error(ss.str());
        break;
    }
}
void GPUdriverKind::command_rmodel_delete(rmodel* rm) {
    //rmodel* rm = rcmd.rm;
    //if(wasRecentlyDeleted(rm)){
    //    //error("accessing a deleted rmodel");
    //    throw runtime_error("accessing a deleted rmodel");
    //}
    if (!rm) {
        //error("attempt to delete null rmodel\n");
        throw runtime_error("attempt to delete null rmodel");
    }
    //if(debug){printf("deleting rmodel %p\n",rm);}
    num_rmodels--;
    bytes_rmodels -= rm->vertices->size() * 4 * 11;
    GLuint handles[4];
    for (int i = 0; i < 4; i++) { handles[i] = rmodel_GPU_handles[rm][i]; }
    glDeleteBuffers(4, handles);
    rmodel_GPU_handles.erase(rm);
    //delete rm; owned by command now
}
void GPUdriverKind::command_clear_screen() {
    glClearColor(125 / 255.0f, 206 / 255.0f, 250 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GPUdriverKind::command_scissoring(bool b) {
    glUniform1i(locations["scissoringOn"], b);
}
void GPUdriverKind::command_color(vec3 v) {
    v.x /= 255.0f;
    v.y /= 255.0f;
    v.z /= 255.0f;
    glUniform3fv(locations["globalColor"], 1, (const GLfloat*)&v);
}
void GPUdriverKind::command_alpha(float f) {
    float val2 = f / 255.0f;
    glUniform1fv(locations["globalAlpha"], 1, (const GLfloat*)&val2);
}
void GPUdriverKind::command_font_select(font* fnt) {
    if (!fnt) {
        //error("attempt to select null font\n");
        throw runtime_error("attempt to select null font");
    }
    renderState.fnt = fnt;
}
void GPUdriverKind::command_mode_select(int i) {
    renderState.renderMode = i;
}
void GPUdriverKind::command_text_pos(vec2 v) {
    renderState.textPos = v;
}
void GPUdriverKind::command_scissor(rect r) {
    //if(debug){printf("set scissor to %dx%d\n",val.size.x,val.size.y);}
    float R[4];
    //R[0] = val.start.x;
    //R[1] = val.start.y;
    //R[2] = val.end.x;
    //R[3] = val.end.y;
    float x1 = ((float)r.start.x) / renderState.width;
    float y1 = ((float)(r.start.y)) / renderState.height;
    float x2 = ((float)r.end.x) / renderState.width;
    float y2 = ((float)(r.end.y)) / renderState.height;
    R[0] = (float)(2 * (x1 - 0.5));//(2*(x2)-1);
    R[1] = (float)(2 * (0.5 - y2));//(2*(1-y2)-1);
    R[2] = (float)(2 * (x2 - 0.5));
    R[3] = (float)(2 * (0.5 - y1));

    glUniform4fv(locations["scissor"], 1, (const GLfloat*)&R);
}
void GPUdriverKind::command_pointsize(float f) {
    glPointSize(f);
}
void GPUdriverKind::command_linewidth(float f) {
    glLineWidth(f);
}
void GPUdriverKind::command_print_text(string *S) {
    if (!renderState.fnt) {
        //error("no font selected\n");
        throw runtime_error("no font selected");
    }
    //for convenience, ignores
    auto m = renderState.renderMode;
    renderState.renderMode = 3;
    printText2D(S->c_str(), renderState.fnt, renderState.textPos);
    renderState.renderMode = m;
    //delete rcmd.s; owned by command now.
    if (debug) { printf("done printing\n"); }
}
void GPUdriverKind::command_depth_test(bool b) {
    if (b) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
}
void GPUdriverKind::command_lighting(bool b) {
    glUniform1i(locations["lightingOn"], b);
}
void GPUdriverKind::command_sun_pos(vec3 v) {
    vec3 v3 = normalize(v);
    glUniform3fv(locations["sunPos"], 1, (const GLfloat*)&v3);
}
void GPUdriverKind::command_sun_light_color(vec3 v) {
    glUniform3fv(locations["sunColor"], 1, (const GLfloat*)&v);
}
void GPUdriverKind::command_ambient_light_color(vec3 v){
    glUniform3fv(locations["ambientColor"], 1, (const GLfloat*)&v);
}
void GPUdriverKind::command_read_pixels(void* buff) {
    int width = renderState.width;
    int height = renderState.height;
    //void* buff = rcmd.buff;
    printf("GPUdriver: read_pixels called: (w %d, h %d, b %p)\n", width, height, buff);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buff);
}
void GPUdriverKind::command_viewport(rect r) {
    //printf("glViewport: %d %d %d %d\n", rcmd.r.start.x, rcmd.r.start.y, rcmd.r.size.x, rcmd.r.size.y);
    //cout << "GPUdriver.cpp: glViewport: rect= " << rcmd.r << endl;
    glViewport((GLint)r.start.x, (GLint)r.start.y, (GLsizei)r.size.x, (GLsizei)r.size.y);
}
void GPUdriverKind::command_face_culling(bool b) {
    if (b) {glEnable(GL_CULL_FACE);}
    else {glDisable(GL_CULL_FACE);}
}
void GPUdriverKind::command_face_cull_ccw() {
    glFrontFace(GL_CCW);
}
void GPUdriverKind::command_push_options() {
    renderStateStack.push_back(renderState);
}
void GPUdriverKind::command_pop_options() {
    renderState = renderStateStack.back();
    renderStateStack.pop_back();
    /// now we need to apply the renderState
}

void GPUdriverKind::parseCommand(const renderCommand3 &rcmd){
    switch(rcmd.type){
        //----------------------------------------------------
        //----------------------------------------------------
        //----------------------------------------------------

            case(LAYER):
                //if(rcmd.layer){rcmd.layer->render();}
                command_layer(rcmd.layer);
            break;

            case(COLORING):
                //glUniform1i(locations["coloringOn"],rcmd.b);
                command_coloring(rcmd.b);
            break;

            case(UVCOLORING):
                //glUniform1i(locations["uvColoringOn"],rcmd.b);
                command_uvcoloring(rcmd.b);
            break;

            case(NORMALCOLORING):
                //glUniform1i(locations["normalColoringOn"],rcmd.b);
                command_normalcoloring(rcmd.b);
            break;

            case(TRANSPARENCY):
                //if (rcmd.b) {
                //    glEnable(GL_BLEND);
                //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                //}
                //else {
                //    glDisable(GL_BLEND);
                //}
                //glUniform1i(locations["transparencyOn"], rcmd.b);
                command_transparency(rcmd.b);
            break;

            case(DEPTHMASK):
                //if(rcmd.b){
                //    glDepthMask(GL_TRUE);
                //}else{
                //    glDepthMask(GL_FALSE);
                //}
                command_depthmask(rcmd.b);
            break;

            case(TEXTURING):
                //glUniform1i(locations["texturingOn"],rcmd.b);
                command_texturing(rcmd.b);
            break;

            case(DEBUG):
                //glUniform1i(locations["debuggingOn"],rcmd.b);
                command_debugging(rcmd.b);
            break;

            case(PROJECTION):
                //renderState.VP = rcmd.cam->MVP;
                //renderState.camPos = rcmd.cam->pos;
                // //delete rcmd.cam; owned by command now.
                //reproject();
                command_projection(rcmd.cam);
            break;

            case(POSITION):
                //renderState.pos = rcmd.v3;
                //reproject();
                command_position(rcmd.v3);
            break;

            case(ROTATION):
                //renderState.rot = rcmd.v3;
                //reproject();
                command_rotation(rcmd.v3);
            break;

            case(SCALE):
                //renderState.scale = rcmd.v3;
                //reproject();
                command_scale(rcmd.v3);
            break;

            case(TEXTURE_UPLOAD):
     //       {
     //           texture *t = rcmd.t;
     //           //if(wasRecentlyDeleted(t)){error("accessing a deleted texture");}
     //           if(!t){
     //               //error("attempt to upload null texture\n");
     //               throw runtime_error("attempt to upload a null texture");
     //           }

     //           if(texture_GPU_handles.count(t)){break;}//texture already uploaded.
     //           if(bitmap_GPU_handles.count(t->bmp)){break;}//texture already uploaded.

     //           if(t->bmp){
     //               while(glGetError() != GL_NO_ERROR){}
					//printf("GPUdriver: TEX_UPLOAD for (%s)\n",toCString(t));
     //               num_textures++;
     //               bytes_textures += t->bmp->width*t->bmp->height*4;
     //               GLuint textureID;
     //               glGenTextures(1, &textureID);
     //               glBindTexture(GL_TEXTURE_2D,textureID);
					//vector<unsigned char> *V = &*(t->bmp->data);
					//unsigned char *buff = V->data();
     //               glTexImage2D(GL_TEXTURE_2D,0,pixelFormatToGL(t->bmp->format), t->bmp->width, t->bmp->height, 0, pixelFormatToGL(t->bmp->format), GL_UNSIGNED_BYTE, buff);
     //               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     //               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     //               bitmap_GPU_handles[t->bmp] = textureID;
     //               printf("texture '%s' uploaded: %d (%dx%d)\n",t->name.c_str(),textureID,t->bmp->height,t->bmp->width);

     //               int err = glGetError();
     //               if(err){
     //                   if(err == GL_OUT_OF_MEMORY){
     //                       //error("Error: GL_OUT_OF_MEMORY");
     //                       throw runtime_error("Error: GL_OUT_OF_MEMORY");
     //                   }
     //                   stringstream ss;
     //                   ss << "GL Error: " << err;
     //                   //error(ss.str().c_str());
     //                   throw runtime_error(ss.str());
     //               }
     //           }else{
     //              //error("no texture data to upload (%s)\n",t->name.c_str());
     //               stringstream ss;
     //               ss << "no texture data to upload (" << t->name << ")";
     //               throw runtime_error(ss.str());
     //           }
     //       } 
                command_texture_upload(rcmd.t);
            break;
            case(TEXTURE_SELECT):
            //{
            //    texture *t = rcmd.t;
            //    //if(wasRecentlyDeleted(t)){error("accessing a deleted texture");}
            //    if(!t){
            //        //error("attempt to select null texture\n");
            //        throw runtime_error("attempt to select null texture");
            //    }
            //    GLuint handle = 0;
            //    //render_sys_data_texture rdata;
            //    if(t->bmp){
            //        if(!bitmap_GPU_handles.count(t->bmp)){
            //            //error("image not uploaded: \"%s\"/%p\n",t->name.c_str(),t->bmp);
            //            stringstream ss;
            //            ss << "image not uploaded: \"" << t->name << "\"/" << (void*)(t->bmp);
            //            throw runtime_error(ss.str());
            //        }
            //        handle = bitmap_GPU_handles[t->bmp];
            //    }else{
            //        if(!texture_GPU_handles.count(t)){
            //            //error("texture not uploaded: %s\n",t->name.c_str());
            //            stringstream ss;
            //            ss << "texture not uploaded: " << t->name;
            //            throw runtime_error(ss.str());
            //        }
            //        handle = texture_GPU_handles[t];
            //    }
            //    if(debug){printf("texture select %s (%d)\n",t->name.c_str(), handle);}
            //    glActiveTexture(GL_TEXTURE0);
            //    glBindTexture(GL_TEXTURE_2D, handle);
            //    glUniform1i(locations["Tex"],0);//wat
            //    vec2 offset = vec2(t->UV.start.x,t->UV.start.y);
            //    vec2 size = vec2(t->UV.size.x,t->UV.size.y);
            //    //offset = vec2(0,0);
            //    //size = vec2(1,1);
            //    //printf("tex offset: (%f x %f), size: (%f x %f)\n",offset.x,offset.y,size.x,size.y);
            //    glUniform2fv(locations["TexOffset"],1,(GLfloat*)&offset);
            //    glUniform2fv(locations["TexSize"],1,(GLfloat*)&size);
            //} 
                command_texture_select(rcmd.t);
            break;

			case(TEXTURE_POS):
            //{
			//	vec2 v = rcmd.v2;
			//	glUniform2fv(locations["TexPos"],1,(GLfloat*)&v);
			//}
                command_texture_pos(rcmd.v2);
            break;

			case(TEXTURE_SCALE):
            //{
				//vec2 s = rcmd.v2;
				//glUniform2fv(locations["TexScale"],1,(GLfloat*)&s);
			//}
                command_texture_scale(rcmd.v2);
            break;

            case(RMODEL_UPLOAD):
            //{
            //    rmodel *rm = rcmd.rm;
            //    //if(wasRecentlyDeleted(rm)){
            //    //    //error("accessing a deleted rmodel");
            //    //    throw runtime_error("accessing a deleted rmodel");
            //    //}
            //    if(!rm){
            //        //error("attempt to upload null rmodel\n");
            //        throw runtime_error("attempt to upload null rmodel");
            //    }
            //    //if(debug){printf("uploading rmodel %p\n",rm);}
            //    if(
            //        (!rm->vertices)||(!rm->normals)||(!rm->colors)||(!rm->uvs)||
            //        (rm->normals->size() != rm->vertices->size())||
            //        (rm->colors->size() != rm->vertices->size())||
            //        (rm->uvs->size() != rm->vertices->size())
            //    ){
            //        //error("trying to upload incomplete rmodel");
            //        throw runtime_error("trying to upload incomplete rmodel");
            //    }
            //    num_rmodels++;
            //    bytes_rmodels += rm->vertices->size()*4*11;
            //    while(glGetError() != GL_NO_ERROR){}
            //    GLuint handles[4];
            //    glGenBuffers(4,handles); //fills handle[0,1,2,3].


            //    //verts
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
            //    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->vertices->data(), GL_STATIC_DRAW);

            //    //colors
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
            //    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->colors->data(), GL_STATIC_DRAW);

            //    //normals
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
            //    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec3), rm->normals->data(), GL_STATIC_DRAW);

            //    //uvs
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
            //    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    glBufferData(GL_ARRAY_BUFFER, rm->vertices->size()*sizeof(vec2), rm->uvs->data(), GL_STATIC_DRAW);

            //    for(int i = 0; i<4; i++){rmodel_GPU_handles[rm][i] = handles[i];}

            //    int err = glGetError();
            //    if(err){
            //        if(err == GL_OUT_OF_MEMORY){
            //            //error("Error: GL_OUT_OF_MEMORY");
            //            throw runtime_error("Error: GL_OUT_OF_MEMORY");
            //        }
            //        //string errstr = string()+"GL Error: "+err;
            //        //error(errstr.c_str());
            //        stringstream ss;
            //        ss << "GL Error: " << err;
            //        throw runtime_error(ss.str());
            //    }
            //} 
                command_rmodel_upload(rcmd.rm);
            break;

            case(RMODEL_RENDER):
            //{
            //    rmodel *rm = rcmd.rm;
            //    //if(wasRecentlyDeleted(rm)){
            //    //    //error("accessing a deleted rmodel");
            //    //    throw runtime_error("accessing a deleted rmodel");
            //    //}
            //    if(!rm){
            //        //error("attempt to render null rmodel\n");
            //        throw runtime_error("attempt to render null rmodel");
            //    }
            //    //if(debug){printf("rendering rmodel %p\n",rm);}
            //    GLuint handles[4];
            //    for(int i = 0; i<4; i++){handles[i] = rmodel_GPU_handles[rm][i];}
            //    //verts
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
            //    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    //colors
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
            //    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    //normals
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[2]);
            //    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
            //    //uvs
            //    glBindBuffer(GL_ARRAY_BUFFER, handles[3]);
            //    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

            //    switch(renderState.renderMode){
            //        case(1):
            //            glDrawArrays(GL_POINTS,0,rm->vertices->size());
            //            break;
            //        case(2):
            //            glDrawArrays(GL_LINES,0,rm->vertices->size());
            //            break;
            //        case(3):
            //            glDrawArrays(GL_TRIANGLES,0,rm->vertices->size());
            //            break;
            //        default:
            //           // error("unknown render mode '%d'\n",renderState.renderMode);
            //            stringstream ss;
            //            ss << "unknwon render mode '" << renderState.renderMode << "'";
            //            throw runtime_error(ss.str());
            //            break;
            //    }
            //} 
                command_rmodel_render(rcmd.rm);
            break;

            case(RMODEL_DELETE):
            //{
            //    rmodel *rm = rcmd.rm;
            //    //if(wasRecentlyDeleted(rm)){
            //    //    //error("accessing a deleted rmodel");
            //    //    throw runtime_error("accessing a deleted rmodel");
            //    //}
            //    if(!rm){
            //        //error("attempt to delete null rmodel\n");
            //        throw runtime_error("attempt to delete null rmodel");
            //    }
            //    //if(debug){printf("deleting rmodel %p\n",rm);}
            //    num_rmodels--;
            //    bytes_rmodels -= rm->vertices->size()*4*11;
            //    GLuint handles[4];
            //    for(int i = 0; i<4; i++){handles[i] = rmodel_GPU_handles[rm][i];}
            //    glDeleteBuffers(4,handles);
            //    rmodel_GPU_handles.erase(rm);
            //    //delete rm; owned by command now
            //} 
                command_rmodel_delete(rcmd.rm);
            break;

            case(CLEAR_SCREEN):
                //glClearColor(125 / 255.0f, 206 / 255.0f, 250 / 255.0f, 1.0f);
                //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                command_clear_screen();
            break;

            case(SCISSORING):
                //glUniform1i(locations["scissoringOn"],rcmd.b);
                command_scissoring(rcmd.b);
            break;

            case(COLOR):
            //{
            //    vec3 val2 = rcmd.v3;
            //    val2.x /= 255.0f;
            //    val2.y /= 255.0f;
            //    val2.z /= 255.0f;
            //    glUniform3fv(locations["globalColor"],1,(const GLfloat*)&val2);
            //} 
                command_color(rcmd.v3);
            break;

            case(ALPHA):
            //{
            //    float val2 = rcmd.f/255.0f;
            //    glUniform1fv(locations["globalAlpha"],1,(const GLfloat*)&val2);
            //} 
                command_alpha(rcmd.f);
            break;

            case(FONT_SELECT):
                //if(!rcmd.fnt){
                //    //error("attempt to select null font\n");
                //    throw runtime_error("attempt to select null font");
                //}
                //renderState.fnt = rcmd.fnt;
                command_font_select(rcmd.fnt);
            break;

            case(MODE_SELECT):
                //renderState.renderMode = rcmd.i;
                command_mode_select(rcmd.i);
            break;

            case(TEXT_POS):
                //renderState.textPos = rcmd.v2;
                command_text_pos(rcmd.v2);
            break;

            case(SCISSOR):
            //{
            //    //if(debug){printf("set scissor to %dx%d\n",val.size.x,val.size.y);}
            //    float R[4];
            //    //R[0] = val.start.x;
            //    //R[1] = val.start.y;
            //    //R[2] = val.end.x;
            //    //R[3] = val.end.y;
            //    float x1 = ((float)rcmd.r.start.x)/renderState.width;
            //    float y1 = ((float)(rcmd.r.start.y))/renderState.height;
            //    float x2 = ((float)rcmd.r.end.x)/renderState.width;
            //    float y2 = ((float)(rcmd.r.end.y))/renderState.height;
            //    R[0] = (float)(2*(x1-0.5));//(2*(x2)-1);
            //    R[1] = (float)(2*(0.5-y2));//(2*(1-y2)-1);
            //    R[2] = (float)(2*(x2-0.5));
            //    R[3] = (float)(2*(0.5-y1));

            //    glUniform4fv(locations["scissor"],1,(const GLfloat*)&R);
            //} 
                command_scissor(rcmd.r);
            break;

            case(POINTSIZE):
                //glPointSize(rcmd.f);
                command_pointsize(rcmd.f);
            break;

            case(LINEWIDTH):
                //glLineWidth(rcmd.f);
                command_linewidth(rcmd.f);
            break;

            case(PRINT_TEXT):
            //{
            //    if(!renderState.fnt){
            //        //error("no font selected\n");
            //        throw runtime_error("no font selected");
            //    }
            //    //for convenience, ignores
            //    auto m = renderState.renderMode;
            //    renderState.renderMode = 3;
            //    printText2D(rcmd.s->c_str(),renderState.fnt,renderState.textPos);
            //    renderState.renderMode = m;
            //    //delete rcmd.s; owned by command now.
            //    if(debug){printf("done printing\n");}
            //} 
                command_print_text(rcmd.s);
            break;

            //case(COMMENT): handled by higher level
            //    if(debug){printf("%s", rcmd.s->c_str());}
            //break;

            case(DEPTH_TEST):
                //if(rcmd.b){glEnable(GL_DEPTH_TEST);}
                //else{glDisable(GL_DEPTH_TEST);}
                command_depth_test(rcmd.b);
            break;

            case(LIGHTING):
                //glUniform1i(locations["lightingOn"],rcmd.b);
                command_lighting(rcmd.b);
            break;
            case(SUN_POS):
            //{
            //    vec3 v3 = normalize(rcmd.v3);
            //    glUniform3fv(locations["sunPos"],1,(const GLfloat*)&v3);
            //}
                command_sun_pos(rcmd.v3);
            break;

            case(SUN_LIGHT_COLOR):
                //glUniform3fv(locations["sunColor"],    1,(const GLfloat*)&rcmd.v3);
                command_sun_light_color(rcmd.v3);
            break;

            case(AMBIENT_LIGHT_COLOR):
                //glUniform3fv(locations["ambientColor"],1,(const GLfloat*)&rcmd.v3);
                command_ambient_light_color(rcmd.v3);
            break;

            case(READ_PIXELS):
            //{
            //    int width = renderState.width;
            //    int height = renderState.height;
            //    void* buff = rcmd.buff;
            //    printf("GPUdriver: read_pixels called: (w %d, h %d, b %p)\n", width, height, buff);
            //    glPixelStorei(GL_PACK_ALIGNMENT, 1);
            //    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buff);
            //}
                command_read_pixels(rcmd.buff);
            break;

            case(VIEWPORT):
             //   //printf("glViewport: %d %d %d %d\n", rcmd.r.start.x, rcmd.r.start.y, rcmd.r.size.x, rcmd.r.size.y);
             //   //cout << "GPUdriver.cpp: glViewport: rect= " << rcmd.r << endl;
             //   glViewport((GLint)rcmd.r.start.x, (GLint)rcmd.r.start.y, (GLsizei)rcmd.r.size.x, (GLsizei)rcmd.r.size.y);
                command_viewport(rcmd.r);
            break;

            case(FACE_CULLING):
                //if (rcmd.b) {
                //    glEnable(GL_CULL_FACE);
                //}
                //else {
                //    glDisable(GL_CULL_FACE);
                //}
                command_face_culling(rcmd.b);
            break;
            
            case(FACE_CULL_CCW):
                //glFrontFace(GL_CCW);
                command_face_cull_ccw();
            break;

            case(PUSH_OPTIONS):
                //renderStateStack.push_back(renderState);
                command_push_options();
            break;

            case(POP_OPTIONS):
                //renderState = renderStateStack.back();
                //renderStateStack.pop_back();
                ///// now we need to apply the renderState
                command_pop_options();
            break;
            //----------------------------------------------------
            //----------------------------------------------------
            //----------------------------------------------------
        }

}

void applyRenderState(renderStateKind state) {

}


#ifndef NO_GLEW
GLenum pixelFormatToGL(pixelFormat F){
	switch(F){
	case(TL_ALPHA):
		return GL_ALPHA;
	break;
	case(TL_RGB):
		return GL_RGB;
	break;
	case(TL_RGBA):
		return GL_RGBA;
	break;
	case(TL_LUMINANCE):
		return GL_LUMINANCE;
	break;
	case(TL_LUMINANCE_ALPHA):
		return GL_LUMINANCE_ALPHA;
	break;
	}
	return (int)F;
}
#endif