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
using glm::mat4;
using glm::vec3;
using glm::vec2;
#include "model.h"
extern model *mBox;
#include "bitmap.h"
#include <map>
using std::map;
GLuint vaoHandle;
//renderCommandBuffer cmdbuff;
renderQueue *rqueue;
GLuint positionBufferHandle;
GLuint colorBufferHandle;
GLuint normalBufferHandle;
GLuint uvBufferHandle;
int numVerts = 0;
map<string,int> locations;
//#define renderCmd(t,d) { renderCommand cmd; cmd.type = t; cmd.d; cmdbuff.push_back(cmd); }
map<bitmap*,GLuint> bitmap_GPU_handles;
map<texture*,GLuint> texture_GPU_handles;
map<rmodel*,GLuint[4]> rmodel_GPU_handles;
#include "fonts.h"

struct renderStateKind{
	public:
	int renderMode = 0; //1 - points, 2 - edges, 3 - triangles
	font *f = 0;
	vec2 textPos = vec2(0,0);
	vec3 pos = vec3(0,0,0);
	vec3 rot = vec3(0,0,0);
	vec3 scale = vec3(1,1,1);
	mat4 VP;
	vec3 camPos = vec3(0,0,0);
	//bool lighting = false;
	//vec3 sun_pos = vec3(1,1,1);
	//vec3 sun_light_color = 0.9f*vec3(1,1,1);
	//vec3 ambient_light_color = 0.1f*vec3(1,1,1);
	/*
	renderStateKind(){
		renderMode = 1;
		f = 0;
		textPos = vec2(0,0);
	}*/
} renderState;

GLuint uploadShader(GLuint shaderType, const char *filepath){
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

GLuint uploadShaderProgram(const char *filepath_vertex, const char *filepath_fragment){
	GLuint vertShader = uploadShader(GL_VERTEX_SHADER, filepath_vertex);
	GLuint fragShader = uploadShader(GL_FRAGMENT_SHADER, filepath_fragment);
	
	GLuint programHandle = glCreateProgram();
	if(0 == programHandle){
		error("error creating program object\n");
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


void renderLowInit(){
	GLenum err = glewInit();
	if(GLEW_OK != err){
		error("Error initializing GLEW: %s\n", glewGetErrorString(err));
	}
	
	GLuint programHandle = uploadShaderProgram("shaders/vertShader.vert","shaders/fragShader.frag");
	
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
	for(auto I = locations.begin(); I != locations.end(); I++){
		I->second = glGetUniformLocation(programHandle,I->first.c_str());
		printf("uniform '%s' at '%d'\n",I->first.c_str(),I->second);
	}
	rqueue = new renderQueue();
	
	printf("renderLow: init OK\n");
}

void projectionToCamera(){
	//renderCmd(RCMD::PROJECTION, m = camera.mProjection*camera.mView);
	rqueue->push_back(new rcmd_projection(camera.getProjection()));
}

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


void renderParseQueue(renderQueue *rqueue){
	if(debug){
		printf("----render queue:\n");
		int i=1;
		for(auto I = rqueue->begin(); I != rqueue->end(); I++){
			printf("%d: %s\n",i,(*I)->type);
			i++;
		}	
	}
	for(auto I = rqueue->begin(); I != rqueue->end(); I++){
		if(debug){printf("executing %s\n",(*I)->type);}
		(*I)->execute();
		//delete *I;
	}
	if(debug){printf("----queue rendered\n");}
	//rqueue->clear();
}

void rcmd_layer::execute(){
	if(val){
		renderLayer *L = (renderLayer*)val;
		L->render();
	}
}

void rcmd_coloring::execute(){
	//if(debug){printf("coloring %d\n",val);}
	glUniform1i(locations["coloringOn"],val);
}

void rcmd_transparency::execute(){
	//if(debug){printf("transparency %d\n",val);}
	if(val){
		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else{
		glDisable(GL_BLEND); 
	}
	glUniform1i(locations["transparencyOn"],val);
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
	glUniform1i(locations["texturingOn"],val);
}

void rcmd_debug::execute(){
	//if(debug){printf("debugging %d\n",val);}
	glUniform1i(locations["debuggingOn"],val);
}

void reproject(){
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
	
	glUniformMatrix4fv(locations["VP"],1,GL_FALSE,(const GLfloat*)&VP);
	glUniformMatrix4fv(locations["M"],1,GL_FALSE,(const GLfloat*)&M);
	glUniformMatrix4fv(locations["Mrot"],1,GL_FALSE,(const GLfloat*)&Mrot);
	glUniform3fv(locations["WorldCamPos"],1,(const GLfloat*)&campos);
}

void rcmd_projection::execute(){
	//if(debug){printf("projection\n");}
	renderState.VP = val.MVP;
	renderState.camPos = val.pos;
	reproject();
	//glUniformMatrix4fv(locations["MVP"],1,GL_FALSE,(const GLfloat*)&val);
}

void rcmd_position::execute(){
	//if(debug){printf("position\n");}
	renderState.pos = val;
	reproject();
	
}

void rcmd_rotation::execute(){
	renderState.rot = val;
	reproject();
}

void rcmd_scale::execute(){
	//if(debug){printf("scale\n");}
	renderState.scale = val;
	reproject();
}

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
int num_textures = 0;
long bytes_textures = 0;
void rcmd_texture_upload::execute(){
	texture *t = val;
	if(!val){error("attempt to upload null texture\n");}
	if(texture_GPU_handles.count(t)){return;}//texture already uploaded.
	if(bitmap_GPU_handles.count(t->bmp)){return;}//texture already uploaded.
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
}

void rcmd_texture_select::execute(){
	texture *t = val;
	if(!val){error("attempt to select null texture\n");}
	GLuint handle = 0;
	//render_sys_data_texture rdata;
	if(t->bmp){
		if(!bitmap_GPU_handles.count(t->bmp)){error("image not uploaded: %p\n",t->bmp);}
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

	for(int i = 0; i<4; i++){rmodel_GPU_handles[rm][i] = handles[i];}
	
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
}

void rcmd_rmodel_delete::execute(){
	rmodel *rm = val;
	if(!val){error("attempt to delete null rmodel\n");}
	//if(debug){printf("deleting rmodel %p\n",rm);}
	num_rmodels--;
	bytes_rmodels -= rm->vertices->size()*4*11;
	GLuint handles[4];
	for(int i = 0; i<4; i++){handles[i] = rmodel_GPU_handles[rm][i];}
	glDeleteBuffers(4,handles);
	rmodel_GPU_handles.erase(rm);
}

void rcmd_clear_screen::execute(){
	//if(debug){printf("clearing screen\n");}
	glClearColor(125/255.0f,206/255.0f,250/255.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void rcmd_scissoring::execute(){
	//if(debug){printf("scissoring: %d\n",val);}
	glUniform1i(locations["scissoringOn"],val);
}

void rcmd_color::execute(){
	//if(debug){printf("set color to %d %d %d\n",val.x,val.y,val.z);}
	vec3 val2 = val;
	val2.x /= 255.0f;
	val2.y /= 255.0f;
	val2.z /= 255.0f;
	glUniform3fv(locations["globalColor"],1,(const GLfloat*)&val2);
}

void rcmd_alpha::execute(){
	//if(debug){printf("set alpha to %f\n",val);}
	float val2 = val/255.0f;
	glUniform1fv(locations["globalAlpha"],1,(const GLfloat*)&val2);
}

void rcmd_font_select::execute(){
	if(!val){error("attempt to select null font\n");}
	//if(debug){printf("selecting font %s (%p)\n",val->name.c_str(),val);}
	renderState.f = val;
}

void rcmd_mode_select::execute(){
	//if(debug){printf("selecting render mode %d\n",val);}
	renderState.renderMode = val;
}

void rcmd_text_pos::execute(){
	//if(debug){printf("set text pos to %f,%f\n",val.x,val.y);}
	renderState.textPos = val;
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
	
	glUniform4fv(locations["scissor"],1,(const GLfloat*)&R);
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
	if(!renderState.f){error("no font selected\n");}
	//for convenience, ignores 
	auto m = renderState.renderMode;
	renderState.renderMode = 3;
	printText2D(val.c_str(),renderState.f,renderState.textPos);
	renderState.renderMode = m;
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

void rcmd_lighting::execute()			{glUniform1i( locations["lightingOn"],val);}//{renderState.lighting = val;}
void rcmd_sun_pos::execute(){
	//vec4 vp = vec4(val,0)*renderState.VP;
	//vp /= vp.w;
	//val = vp;
	val = normalize(val);
	
	glUniform3fv(locations["sunPos"],1,(const GLfloat*)&val);
}//{renderState.sun_pos = val;}
void rcmd_sun_light_color::execute()	{glUniform3fv(locations["sunColor"],    1,(const GLfloat*)&val);}//{renderState.sun_brightness = val;}
void rcmd_ambient_light_color::execute(){glUniform3fv(locations["ambientColor"],1,(const GLfloat*)&val);}//{renderState.ambient_brightness = val;}
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
