#ifndef PAINT_GUARD
#define PAINT_GUARD
#include "printw.h"
//#include "renderLayer.h"
struct renderLayer;
struct font;
struct texture;
struct rmodel;
struct model;
#include "vec.h"
#include "glm/glm.hpp"
#include <string>
using std::string;
//class renderLayer;
//core functions
void setLayer(renderLayer *L);			//chooses the renderlayer to which drawing is done
void addLayer(renderLayer *L);			//adds the specified renderlayer after the current one
void addLayer(renderLayer *L1, renderLayer *L2);	//adds L2 after existing L1
renderLayer *addNewLayer();				//adds a new renderlayer after the current one

void removeLayer(renderLayer *L);		//removes the specified renderlayer
void renderAllLayers();					//executes all the render queues
void printAllLayers();					//(debug) prints the commands currently in queues
void printAllLayersNextRender();		//(debug) same, but waits until a 'renderAll' command is issued

void setColoring(bool b);				//enables per-vertex coloring (no color = white)
void setTransparency(bool b);			//enables transparency (no transparency = alpha 1)
void setTexturing(bool b);				//enables texturing (no texture = colors only)
void setDebug(bool b);					//enables debug rendering mode
void setScissoring(bool b);				//enable scissor test
void setDepthTest(bool b);				//enable depth test

void setColor(vec3f col);				//sets global color (default = 1,1,1)
void setAlpha(float a);					//sets global alpha (default = 1), needs transparency
void setTexture(texture *t);			//selects current texture, needs upload
void setFont(font *f);					//selects current font, needs upload
void setRenderMode(int mode);			//1 - points, 2 - lines, 3 - triangles, not implemented: 4 - triangles (wireframe)
void setTextPos(vec2 textPos); 			//textPos is advanced automatically after each print
void setScissor(rect S);				//limits the drawn-to area
void setPointSize(float size);			//sets size of single points (not lines)
void setLineWidth(float width);			//sets the width of lines

void uploadTexture(texture *t);			//uploads texture to GPU
void uploadRmodel(rmodel *rm);			//uploads rmodel to GPU
void deleteRmodel(rmodel *rm);			//deletes rmodel from GPU

void setProjection(mat4 proj);			//changes current projection matrix
void setPosition(vec3 pos);				//changes current world pos
void setScale(vec3 scale);				//changes current object scale 

//void clearDepthBuffer();				//clears only the depth buffer
void clearScreen();						//clears screen with current color
void drawRmodel(rmodel *rm);			//draws rendermodel
void printText(string text);		//draws text at current textPos

//simple drawing functions:
void drawPoint(vec3 pos);
void drawPoint(vec3 pos, vec3f color);
void drawLine(vec3 start, vec3 end);
void drawLine(vec3 start, vec3 end, vec3f color);
void drawTriangle(vec3 A, vec3 B, vec3 C);
void drawRect(rect R);
void drawRectOutline(rect R);
void drawRectOutlineColored(rect R, vec3f col); //disables scissor, sets color, draws rect outline
//void drawTexturedRect(rect R);
//void drawTexturedRectUV(rect R, rectf UV);

void drawModel(vec3 pos, vec3 rot, vec3 scale, model *m);
//util
void renderComment(string str);

#endif