#include "program/file.h"
#include "render/GPUdriver.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "resource/texture.h"
#include "render/printw.h"
#include "util/util.h"
//#include "util/debug.h"
#include "util/globals_render.h"
#include "render/GPUdriverOpenGL.h"
#ifdef USE_GLEW
	#include "Ext/GL/glew.h"
#endif

using namespace std;


void GPUdriverOpenGL::renderLowInit(){
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
GLuint GPUdriverOpenGL::uploadShader(GLuint shaderType, const char *filepath){
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

GLuint GPUdriverOpenGL::uploadShaderProgram(const char *filepath_vertex, const char *filepath_fragment){
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

#define rs stateStack.state

void GPUdriverOpenGL::reproject(){
	vec3 mpos = rs.pos;
	float mrot_ang = length(rs.rot);
	vec3 mrot_axis = normalizeSafe(rs.rot);
	vec3 mscale = rs.scale;
	mat4 VP = mat4();
	mat4 M = mat4();
	mat4 Mrot = mat4();

	VP = rs.VP;
	M = glm::translate(mat4(), mpos);
	if(mrot_ang){M = glm::rotate(M,mrot_ang,mrot_axis);}
	M = glm::scale(M,mscale);

	if(mrot_ang){Mrot = glm::rotate(mat4(),mrot_ang,mrot_axis);}

	vec3 campos = rs.camPos;
#ifndef NO_GLEW
	glUniformMatrix4fv(locations["VP"],1,GL_FALSE,(const GLfloat*)&VP);
	glUniformMatrix4fv(locations["M"],1,GL_FALSE,(const GLfloat*)&M);
	glUniformMatrix4fv(locations["Mrot"],1,GL_FALSE,(const GLfloat*)&Mrot);
	glUniform3fv(locations["WorldCamPos"],1,(const GLfloat*)&campos);
#endif
}

void GPUdriverOpenGL::parseQueue(renderQueue3 *rqueue){
    for(auto I = rqueue->begin(); I != rqueue->end(); I++){
        renderCommand3 &rcmd = **I;
        parseCommand(rcmd);

    }
}


std::ostream& operator<<(std::ostream& stream, rect R) {
    stream << "(" << R.start.x << ", " << R.start.y <<" x " << R.size.x << ", " << R.size.y << ")";
    return stream;
}



void GPUdriverOpenGL::parseCommand(const renderCommand3 &rcmd){
    /// pst: we can mute commands if instead of switches, use a map
    switch(rcmd.type){
            case(LAYER):            command_layer(rcmd.layer);      break;
            case(COLORING):         command_coloring(rcmd.b);       break;
            case(UVCOLORING):       command_uvcoloring(rcmd.b);     break;
            case(NORMALCOLORING):   command_normalcoloring(rcmd.b); break;
            case(TRANSPARENCY):     command_transparency(rcmd.b);   break;
            case(DEPTHMASK):        command_depthmask(rcmd.b);      break;
            case(TEXTURING):        command_texturing(rcmd.b);      break;
            case(DEBUG):            command_debugging(rcmd.b);      break;
            case(PROJECTION):       command_projection(rcmd.cam);   break;
            case(POSITION):         command_position(rcmd.v3);      break;
            case(ROTATION):         command_rotation(rcmd.v3);      break;
            case(SCALE):            command_scale(rcmd.v3);         break;
            case(TEXTURE_UPLOAD):   command_texture_upload(rcmd.t); break;
            case(TEXTURE_SELECT):   command_texture_select(rcmd.t); break;
			case(TEXTURE_POS):      command_texture_pos(rcmd.v2);   break;
			case(TEXTURE_SCALE):    command_texture_scale(rcmd.v2); break;
            case(RMODEL_UPLOAD):    command_rmodel_upload(rcmd.rm); break;
            case(RMODEL_RENDER):    command_rmodel_render(rcmd.rm); break;
            case(RMODEL_DELETE):    command_rmodel_delete(rcmd.rm); break;
            case(CLEAR_SCREEN):     command_clear_screen();         break;
            case(SCISSORING):       command_scissoring(rcmd.b);     break;
            case(COLOR):            command_color(rcmd.v3);         break;
            case(ALPHA):            command_alpha(rcmd.f);          break;
            case(FONT_SELECT):      command_font_select(rcmd.fnt);  break;
            case(MODE_SELECT):      command_mode_select(rcmd.i);    break;
            case(TEXT_POS):         command_text_pos(rcmd.v2);      break;
            case(SCISSOR):          command_scissor(rcmd.r);        break;
            case(POINTSIZE):        command_pointsize(rcmd.f);      break;
            case(LINEWIDTH):        command_linewidth(rcmd.f);      break;
            case(PRINT_TEXT):       command_print_text(rcmd.s);     break;
            case(DEPTH_TEST):       command_depth_test(rcmd.b);     break;
            case(LIGHTING):         command_lighting(rcmd.b);       break;
            case(SUN_POS):          command_sun_pos(rcmd.v3);       break;
            case(SUN_LIGHT_COLOR):      command_sun_light_color(rcmd.v3);break;
            case(AMBIENT_LIGHT_COLOR):  command_ambient_light_color(rcmd.v3);break;
            case(READ_PIXELS):      command_read_pixels(rcmd.buff); break;
            case(VIEWPORT):         command_viewport(rcmd.r);       break;
            case(FACE_CULLING):     command_face_culling(rcmd.b);   break;
            case(FACE_CULL_CCW):    command_face_cull_ccw();        break;
            case(PUSH_OPTIONS):     command_push_options();         break;
            case(POP_OPTIONS):      command_pop_options();break;
			case(RESET_OPTIONS):	command_reset_options(); break;
			case(PRINT_OPTIONS):	command_print_options(); break;
		}
}

void GPUdriverOpenGL::setViewportSize(int width, int height) {
	rs.width = width;
	rs.height = height;
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