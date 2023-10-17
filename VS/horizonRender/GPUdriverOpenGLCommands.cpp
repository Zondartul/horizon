#include <sstream>
#include <stdexcept>
#include <iostream>
#include "program/file.h"
#include "render/GPUdriverOpenGL.h"
#include "resource/texture.h"
#include "render/printw.h"
#include "util/util.h"
//#include "util/debug.h"
#include "util/globals_render.h"
//#include "util/stringUtils.h" /// to print vec2 and stuff
#include "render/rmodel.h"
using namespace std;

#define rs stateStack.state

void GPUdriverOpenGL::command_layer(renderLayer* layer) { if (layer) { layer->render(); } }
void GPUdriverOpenGL::command_coloring(bool b) {
    rs.coloring = b;
    glUniform1i(locations["coloringOn"], b); 
}
void GPUdriverOpenGL::command_uvcoloring(bool b) {
    rs.uvcoloring = b;
    glUniform1i(locations["uvColoringOn"], b); 
}
void GPUdriverOpenGL::command_normalcoloring(bool b) {
    rs.normalcoloring = b;
    glUniform1i(locations["normalColoringOn"], b); 
}
void GPUdriverOpenGL::command_transparency(bool b) {
    rs.transparency = b;
    if (b) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else { glDisable(GL_BLEND); }
    glUniform1i(locations["transparencyOn"], b);
}
void GPUdriverOpenGL::command_depthmask(bool b) {
    rs.depthmask = b;
    if (b) { glDepthMask(GL_TRUE); }
    else { glDepthMask(GL_FALSE); }
}
void GPUdriverOpenGL::command_texturing(bool b) {
    rs.texturing = b;
    glUniform1i(locations["texturingOn"], b); 
}
void GPUdriverOpenGL::command_debugging(bool b) {
    rs.debugging = b;
    glUniform1i(locations["debuggingOn"], b); 
}
void GPUdriverOpenGL::command_projection(camprojection* cam) {
    rs.VP = cam->MVP;
    rs.camPos = cam->pos;
    //delete rcmd.cam; owned by command now.
    reproject();
}
void GPUdriverOpenGL::command_position(vec3 v) {
    rs.pos = v;
    reproject();
}
void GPUdriverOpenGL::command_rotation(vec3 v) {
    rs.rot = v;
    reproject();
}
void GPUdriverOpenGL::command_scale(vec3 v) {
    rs.scale = v;
    reproject();
}
void GPUdriverOpenGL::command_texture_upload(texture* t) {
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
void GPUdriverOpenGL::command_texture_select(texture* t) {
    rs.cur_texture = t;
    //texture* t = rcmd.t;
    //if(wasRecentlyDeleted(t)){error("accessing a deleted texture");}
    if (!t) {
        //error("attempt to select null texture\n");
        //throw runtime_error("attempt to select null texture");
        return;
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
void GPUdriverOpenGL::command_texture_pos(vec2 v) {
    rs.texture_pos = v;
    glUniform2fv(locations["TexPos"], 1, (GLfloat*)&v); 
}
void GPUdriverOpenGL::command_texture_scale(vec2 v) {
    rs.texture_scale = v;
    glUniform2fv(locations["TexScale"], 1, (GLfloat*)&v); 
}
void GPUdriverOpenGL::command_rmodel_upload(rmodel* rm) {
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

void GPUdriverOpenGL::command_rmodel_render(rmodel* rm) {
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

    switch (rs.renderMode) {
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
        // error("unknown render mode '%d'\n",rs.renderMode);
        stringstream ss;
        ss << "unknwon render mode '" << rs.renderMode << "'";
        throw runtime_error(ss.str());
        break;
    }
}
void GPUdriverOpenGL::command_rmodel_delete(rmodel* rm) {
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
void GPUdriverOpenGL::command_clear_screen() {
    glClearColor(125 / 255.0f, 206 / 255.0f, 250 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GPUdriverOpenGL::command_scissoring(bool b) {
    rs.scissoring = b;
    glUniform1i(locations["scissoringOn"], b);
}
void GPUdriverOpenGL::command_color(vec3 v) {
    rs.color = v;
    v.x /= 255.0f;
    v.y /= 255.0f;
    v.z /= 255.0f;
    glUniform3fv(locations["globalColor"], 1, (const GLfloat*)&v);
}
void GPUdriverOpenGL::command_alpha(float f) {
    rs.alpha = f;
    float val2 = f / 255.0f;
    glUniform1fv(locations["globalAlpha"], 1, (const GLfloat*)&val2);
}
//void GPUdriverOpenGL::command_font_select(font* fnt) {
//    //if (!fnt) {
//    //    //error("attempt to select null font\n");
//    //    throw runtime_error("attempt to select null font");
//    //}
//    rs.fnt = fnt;
//}
void GPUdriverOpenGL::command_mode_select(int i) {
    rs.renderMode = i;
}
//void GPUdriverOpenGL::command_text_pos(vec2 v) {
//    rs.textPos = v;
//}
void GPUdriverOpenGL::command_scissor(rect r) {
    rs.scissor = r;
    //if(debug){printf("set scissor to %dx%d\n",val.size.x,val.size.y);}
    float R[4];
    //R[0] = val.start.x;
    //R[1] = val.start.y;
    //R[2] = val.end.x;
    //R[3] = val.end.y;
    float x1 = ((float)r.start.x) / rs.width;
    float y1 = ((float)(r.start.y)) / rs.height;
    float x2 = ((float)r.end.x) / rs.width;
    float y2 = ((float)(r.end.y)) / rs.height;
    R[0] = (float)(2 * (x1 - 0.5));//(2*(x2)-1);
    R[1] = (float)(2 * (0.5 - y2));//(2*(1-y2)-1);
    R[2] = (float)(2 * (x2 - 0.5));
    R[3] = (float)(2 * (0.5 - y1));

    glUniform4fv(locations["scissor"], 1, (const GLfloat*)&R);
}
void GPUdriverOpenGL::command_pointsize(float f) {
    rs.pointsize = f;
    glPointSize(f);
}
void GPUdriverOpenGL::command_linewidth(float f) {
    rs.linewidth = f;
    glLineWidth(f);
}
void GPUdriverOpenGL::command_print_text(string* S) {
    //if (!rs.fnt) {
    //    //error("no font selected\n");
    //    throw runtime_error("no font selected");
    //}
    ////for convenience, ignores
    //auto m = rs.renderMode;
    //rs.renderMode = 3;
    //printText2D(S->c_str(), rs.fnt, rs.textPos);
    //rs.renderMode = m;
    ////delete rcmd.s; owned by command now.
    //if (debug) { printf("done printing\n"); }
}
void GPUdriverOpenGL::command_depth_test(bool b) {
    rs.depth_test = b;
    if (b) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
}
void GPUdriverOpenGL::command_lighting(bool b) {
    rs.lighting = b;
    glUniform1i(locations["lightingOn"], b);
}
void GPUdriverOpenGL::command_sun_pos(vec3 v) {
    rs.sun_pos = v;
    vec3 v3 = normalize(v);
    glUniform3fv(locations["sunPos"], 1, (const GLfloat*)&v3);
}
void GPUdriverOpenGL::command_sun_light_color(vec3 v) {
    rs.sun_light_color = v;
    glUniform3fv(locations["sunColor"], 1, (const GLfloat*)&v);
}
void GPUdriverOpenGL::command_ambient_light_color(vec3 v) {
    rs.ambient_light_color = v;
    glUniform3fv(locations["ambientColor"], 1, (const GLfloat*)&v);
}
void GPUdriverOpenGL::command_read_pixels(void* buff) {
    int width = rs.width;
    int height = rs.height;
    //void* buff = rcmd.buff;
    printf("GPUdriver: read_pixels called: (w %d, h %d, b %p)\n", width, height, buff);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buff);
}
void GPUdriverOpenGL::command_viewport(rect r) {
    rs.viewport = r;
    //printf("glViewport: %d %d %d %d\n", rcmd.r.start.x, rcmd.r.start.y, rcmd.r.size.x, rcmd.r.size.y);
    //cout << "GPUdriver.cpp: glViewport: rect= " << rcmd.r << endl;
    glViewport((GLint)r.start.x, (GLint)r.start.y, (GLsizei)r.size.x, (GLsizei)r.size.y);
}
void GPUdriverOpenGL::command_face_culling(bool b) {
    rs.face_culling = b;
    if (b) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
}
void GPUdriverOpenGL::command_face_cull_ccw() {
    /// this one doesn't have an off switch
    /// but it should
    glFrontFace(GL_CCW);
}

void GPUdriverOpenGL::command_push_options() {
    //stateStack.push_back(rs);
    stateStack.push();
}
void GPUdriverOpenGL::command_pop_options() {
    //auto rs = stateStack.back();
    //stateStack.pop_back();
    /// now we need to apply the rs
    //rs.applyRenderState(rs);
    stateStack.pop(*this);
}

void GPUdriverOpenGL::command_reset_options() {
    //resetrs();
    stateStack.reset(*this);
}

#undef rs


void GPUdriverOpenGL::command_print_options() {
    stateStack.print();
}
