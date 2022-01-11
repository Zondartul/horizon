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
extern model mBox;


GLuint vaoHandle;
renderCommandBuffer cmdbuff;
GLuint positionBufferHandle;
GLuint colorBufferHandle;
GLuint normalBufferHandle;
GLuint uvBufferHandle;
int numVerts = 0;
map<string,int> locations;
#define renderCmd(t,d) { renderCommand cmd; cmd.type = t; cmd.d; cmdbuff.push_back(cmd); }


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

	locations["MVP"] = 0;
	locations["Tex"] = 0;
	locations["texturingOn"] = 0;
	locations["coloringOn"] = 0;
	locations["debuggingOn"] = 0;
	locations["transparencyOn"] = 0;
	
	for(auto I = locations.begin(); I != locations.end(); I++){
		I->second = glGetUniformLocation(programHandle,I->first.c_str());
		printf("uniform '%s' at '%d'\n",I->first.c_str(),I->second);
	}
	
	printf("renderLow: init OK\n");
}

void projectionToCamera(){
	renderCmd(RCMD::PROJECTION, m = camera.mProjection*camera.mView);
}

void unloadModel(rmodel *rm){
	//can't delete immediately because it might not have been drawn yet
	renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[0]);
	renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[1]);
	renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[2]);
	renderCmd(RCMD::BUFFER_DELETE,i=rm->handle[3]);
	rm->handle[0] = 0;
	rm->handle[1] = 0;
	rm->handle[2] = 0;
	rm->handle[3] = 0;
}

void renderTriangle(){
	rmodel rm;

	rm.vertices->push_back(vec3(0,0,0));
	rm.vertices->push_back(vec3(0.5,0,0));
	rm.vertices->push_back(vec3(0.5,0.5,0));
	rm.vertices->push_back(vec3(0.5,0,0));
	rm.vertices->push_back(vec3(0.5,0.5,0));
	rm.vertices->push_back(vec3(1,0.5,0));
	
	rm.colors->push_back(vec3(1,0,0));
	rm.colors->push_back(vec3(0,1,0));
	rm.colors->push_back(vec3(0,0,1));
	rm.colors->push_back(vec3(1,0,0));
	rm.colors->push_back(vec3(0,1,0));
	rm.colors->push_back(vec3(0,0,1));
	
	// rm.normals->push_back(vec3(0,0,1));
	// rm.normals->push_back(vec3(0,0,1));
	// rm.normals->push_back(vec3(0,0,1));
	// rm.normals->push_back(vec3(0,0,1));
	// rm.normals->push_back(vec3(0,0,1));
	// rm.normals->push_back(vec3(0,0,1));
	
	// rm.uvs->push_back(vec2(0,0));
	// rm.uvs->push_back(vec2(0,0));
	// rm.uvs->push_back(vec2(0,0));
	// rm.uvs->push_back(vec2(0,0));
	// rm.uvs->push_back(vec2(0,0));
	// rm.uvs->push_back(vec2(0,0));
	
	renderCmd(RCMD::COLORING,b=true);
	renderCmd(RCMD::TEXTURING,b=false);
	uploadModel(&rm);
	renderModel(RCMD::DRAW_TRIANGLES,&rm);
	//unloadModel(&rm); //destructor does it
}

void uploadModel(model *m){
	m->rm = new rmodel(m);
	uploadModel(m->rm);
}

void uploadModel(rmodel *rm){
	
	//the number of primitives in a model is determined
	//by the number of vertices.
	//we need to either use dummy buffers or extend
	//the other attribute data to cover all the vertices
	//(vertices.size == colors.size == normals.size == uvs.size)
	
	while(rm->colors->size() < rm->vertices->size()){rm->colors->push_back(vec3(1.0f,1.0f,1.0f));}//default color is white
	while(rm->normals->size() < rm->vertices->size()){rm->normals->push_back(vec3(0.0f,0.0f,0.0f));}
	while(rm->uvs->size() < rm->vertices->size()){rm->uvs->push_back(vec2(0.0f,0.0f));}
	
	glGenBuffers(1,(GLuint*)&rm->handle[0]);
	renderCmd(RCMD::VERTICES_SELECT,i=rm->handle[0]);
	renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->vertices);
	
	
	glGenBuffers(1,(GLuint*)&rm->handle[1]);
	renderCmd(RCMD::COLORS_SELECT, i=rm->handle[1]);
	renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->colors);
	
	glGenBuffers(1,(GLuint*)&rm->handle[2]);
	renderCmd(RCMD::NORMALS_SELECT, i=rm->handle[2]);
	renderCmd(RCMD::BUFFER_VEC3_LOAD, p=rm->normals);
	
	glGenBuffers(1,(GLuint*)&rm->handle[3]);
	renderCmd(RCMD::UVS_SELECT,i=rm->handle[3]);
	renderCmd(RCMD::BUFFER_VEC2_LOAD,p=rm->uvs);
}

void renderModel(RCMD command, rmodel *rm){
	renderCmd(RCMD::VERTICES_SELECT,i=rm->handle[0]);
	renderCmd(RCMD::COLORS_SELECT,i=rm->handle[1]);
	renderCmd(RCMD::NORMALS_SELECT,i=rm->handle[2]);
	renderCmd(RCMD::UVS_SELECT,i=rm->handle[3]);
	renderCmd(command,i=rm->vertices->size());
}

void renderModel(model *m){
	renderCmd(RCMD::COLORING, b=false);
	renderCmd(RCMD::TEXTURING, b=true);
	renderCmd(RCMD::TEXTURE_SELECT,i=m->t.handle.t);
	renderModel(RCMD::DRAW_TRIANGLES, m->rm);
}

void renderTick(){
	glClearColor(0.3,0.7,0.9,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	
	camera.go3D();
	//camera.go2D();
	projectionToCamera();
	
	//renderCmd(RCMD::TEXTURING,b=true);
	//renderCmd(RCMD::COLORING,b=true);
	renderTriangle();
	renderModel(&mBox);
	//camera.go2D();
	//printw("Hello World!");
	textPos = {25.0f,25.0f};//{100.0f,100.0f};
	//renderCmd(RCMD::DEBUG,b=true);
	printText2D("Hi");
	
	renderParseCommandBuffer(&cmdbuff);
	
	OpenGL_swap();	
}

void renderParseCommandBuffer(renderCommandBuffer *cmdbuff){
	vector<vec3> *v;
	vector<vec2> *v2;
	rmodel *rm;
	bool debug = false;
	for(auto I = cmdbuff->begin(); I != cmdbuff->end(); I++){
		switch(I->type){
			case(RCMD::VERTICES_SELECT):
				if(debug){printf("verts select %d\n",I->i);}
				glBindBuffer(GL_ARRAY_BUFFER, I->i);
				glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				break;
			case(RCMD::COLORS_SELECT):
				if(debug){printf("colors select %d\n",I->i);}
				glBindBuffer(GL_ARRAY_BUFFER, I->i);
				glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				break;
			case(RCMD::NORMALS_SELECT):
				if(debug){printf("normals select %d\n",I->i);}
				glBindBuffer(GL_ARRAY_BUFFER, I->i);
				glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				break;
			case(RCMD::UVS_SELECT):
				if(debug){printf("uvs select %d\n",I->i);}
				glBindBuffer(GL_ARRAY_BUFFER, I->i);
				glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
				break;
			case(RCMD::BUFFER_VEC3_LOAD):
				if(debug){printf("buffer vec3 load %p\n",I->p);}
				v = (vector<vec3> *)I->p;
				//if(debug){for(int i = 0; i < v->size(); i++){printf("v[%d] = (%f,%f,%f)\n",i,(*v)[i].x,(*v)[i].y,(*v)[i].z);}}
				glBufferData(GL_ARRAY_BUFFER, v->size()*sizeof(vec3), v->data(), GL_STATIC_DRAW);
				numVerts = v->size();
				break;
			case(RCMD::BUFFER_VEC2_LOAD):
				if(debug){printf("buffer vec2 load %p\n",I->p);}
				v2 = (vector<vec2> *)I->p;
				glBufferData(GL_ARRAY_BUFFER, v2->size()*sizeof(vec2), v2->data(), GL_STATIC_DRAW);
				numVerts = v->size();
				break;
			case(RCMD::BUFFER_DELETE):
				if(debug){printf("buffer delete %i\n",I->i);}
				glDeleteBuffers(1,(GLuint*)&I->i);
				break;
			case(RCMD::RMODEL_DELETE):
				if(debug){printf("rmodel delete %p\n",I->p);}
				rm = (rmodel*)I->p;
				delete rm->colors; //go fast, blobassus
				delete rm->vertices; //karate lessons
				delete rm->uvs;		//sunscreen
				delete rm->normals;	//pants that fit
				rm->colors = 0;
				rm->vertices = 0;
				rm->uvs = 0;
				rm->normals = 0;
				delete rm; //it's all yours
				break;
			case(RCMD::DRAW_TRIANGLES):
				if(debug){printf("draw triangles %d =============\n",I->i);}
				glBindVertexArray(vaoHandle);
				glDrawArrays(GL_TRIANGLES,0,I->i);
				//printf("+");
				break;
			case(RCMD::DRAW_LINES):
				if(debug){printf("draw triangles %d =============\n",I->i);}
				glBindVertexArray(vaoHandle);
				glDrawArrays(GL_LINES,0,I->i);
				//printf("+");
				break;
			case(RCMD::DRAW_POINTS):
				if(debug){printf("draw triangles %d =============\n",I->i);}
				glBindVertexArray(vaoHandle);
				glDrawArrays(GL_POINTS,0,I->i);
				//printf("+");
				break;
			case(RCMD::PROJECTION):
				if(debug){printf("projection\n");}
				glUniformMatrix4fv(locations["MVP"],1,GL_FALSE,(const GLfloat*)&(I->m));
				//printf("p");
				break;
			case(RCMD::TEXTURE_SELECT):
				if(debug){printf("texture select %d\n",I->i);}
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, I->i);
				glUniform1i(locations["Tex"],0);//I->i);
				//printf("t");
				break;
			case(RCMD::TRANSPARENCY):
				if(debug){printf("transparency %d\n",I->b);}
				if(I->b){
					glEnable(GL_BLEND); 
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}else{
					glDisable(GL_BLEND); 
				}
				glUniform1i(locations["transparencyOn"],I->b);
				break;
			case(RCMD::TEXTURING):
				if(debug){printf("texturing %d\n",I->b);}
				glUniform1i(locations["texturingOn"],I->b);
				break;
			case(RCMD::COLORING):
				if(debug){printf("coloring %d\n",I->b);}
				glUniform1i(locations["coloringOn"],I->b);
				break;
			case(RCMD::DEBUG):
				if(debug){printf("debugging %d\n",I->b);}
				glUniform1i(locations["debuggingOn"],I->b);
				break;
			default:
				error("unknown command!");
				break;
		}
	}
	cmdbuff->clear();
}


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
