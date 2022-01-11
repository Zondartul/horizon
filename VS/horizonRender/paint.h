#ifndef PAINT_GUARD
#define PAINT_GUARD
#include "printw.h"
//#include "renderLayer.h"
class renderLayer;
struct font;
struct texture;
class rmodel;
class model;
#include "vec.h"
#include "glm/glm.hpp"
#include <string>
#include "camera.h"
#include <vector>
using std::string;
using std::vector;
//class renderLayer;
extern renderLayer *currentLayer;
//core functions
void setLayer(renderLayer *L);			//chooses the renderlayer to which drawing is done
void addLayer(renderLayer *L);			//adds the specified renderlayer after the current one
void addLayer(renderLayer *L1, renderLayer *L2);	//adds L2 after existing L1
renderLayer *addNewLayer(string name = "", bool persistent = true, bool special = false);				//adds a new renderlayer after the current one
renderLayer *duplicateLayer(renderLayer *L); //creates a copy of layer L and returns it
void clearLayer(renderLayer *L);		//clears a layer
void resetLayer(renderLayer *L);		//adds a call to the layer's reset layer.

void removeLayer(renderLayer *L);		//removes the specified renderlayer
void renderAllLayers();					//executes all the render queues
void printAllLayers();					//(debug) prints the commands currently in queues
void printAllLayersNextRender();		//(debug) same, but waits until a 'renderAll' command is issued
void frameReport();						//(debug) write the commands in queues to file
void frameReportNextRender();			//(debug) same, but waits until a 'renderAll' command is issued

void setColoring(bool b);				//enables per-vertex coloring (no color = white)
void setUvColoring(bool b);				//(debug) add color based on UV-coordinates
void setNormalColoring(bool b);			//(debug) add color based on the normal vector
void setTransparency(bool b);			//enables transparency (no transparency = alpha 1)
void setDepthMask(bool b);				//enables or disables depth buffer writing
void setTexturing(bool b);				//enables texturing (no texture = colors only)
void setDebug(bool b);					//enables debug rendering mode
void setScissoring(bool b);				//enable scissor test
void setDepthTest(bool b);				//enable depth test
void setLighting(bool b);				//enable lighting calc

void drawLayer(renderLayer *L);			//render another layer while rendering this one
void setColor(vec3 col);				//sets global color (default = 1,1,1)
void setAlpha(float a);					//sets global alpha (default = 1), needs transparency
void setTexture(texture *t);			//selects current texture, needs upload
void setFont(font *f);					//selects current font, needs upload
void setRenderMode(int mode);			//1 - points, 2 - lines, 3 - triangles, not implemented: 4 - triangles (wireframe)
void setTextPos(vec2 textPos); 			//textPos is advanced automatically after each print
void setScissor(rect S);				//limits the drawn-to area
void setPointSize(float size);			//sets size of single points (not lines)
void setLineWidth(float width);			//sets the width of lines
void setSunPos(vec3 pos);				//sets the directional light position
void setSunColor(vec3 val);				//sets the directional light brightness
void setAmbientColor(vec3 val);			//sets the ambient light brightness

void uploadTexture(texture *t);			//uploads texture to GPU
void uploadRmodel(rmodel *rm);			//uploads rmodel to GPU
void deleteRmodel(rmodel *rm);			//deletes rmodel from GPU

void setProjection(camprojection cpj);	//changes current projection matrix
void setProjectionToCamera(cameraKind *camera);
void setPosition(vec3 pos);				//changes current world position
void setRotation(vec3 rot);				//changes current world rotation
void setScale(vec3 scale);				//changes current object scale

void setTexturePosition(vec2 pos);		//changes global texture shift
void setTextureScale(vec2 scale);		//changes global texture scale

//void clearDepthBuffer();				//clears only the depth buffer
void clearScreen();						//clears screen with current color
void drawRmodel(rmodel *rm);			//draws rendermodel
void drawRmodelStd(rmodel *rm);			//draws rendermodel at standard position,
										//	rotation, and scale
void printText(string text);		//draws text at current textPos
void pushRenderOptions();                //save current render options
void popRenderOptions();                 //restore render options

//simple drawing functions:
void drawPoint(vec3 pos);
void drawPoints(const vector<vec3> &points);
void drawPoint(vec3 pos, vec3 color);
void drawLine(vec3 start, vec3 end);
void drawLine(vec3 start, vec3 end, vec3 color);
void drawArrow(vec3 start, vec3 end);
void drawArrow(vec3 start, vec3 end, vec3 color);
void drawTriangle(vec3 A, vec3 B, vec3 C);
void drawRect(rect R);
void drawRectOutline(rect R);
void drawRectOutlineColored(rect R, vec3 col); //disables scissor, sets color, draws rect outline
void drawImage(texture *t, rect R);

void debugFloatingText(vec3 p, string S);
//void drawTexturedRect(rect R);
//void drawTexturedRectUV(rect R, rectf UV);

void drawModel(vec3 pos, vec3 rot, vec3 scale, model *m);

//draws primitive shapes by scaling a prefab model.
void drawBoxWireframe(AABB aabb);
void drawCyllinderWireframe(AABB aabb);
void drawConeWireframe(AABB aabb);
void drawSphereWireframe(AABB aabb);

void drawBoxWireframe(vec3 pos, vec3 rot,  vec3 scale);
void drawCyllinderWireframe(vec3 pos, vec3 rot, float r, float h);
void drawConeWireframe(vec3 pos, vec3 rot, float r, float h);
void drawSphereWireframe(vec3 pos, vec3 rot, float r);

void drawBox(vec3 pos, vec3 rot,  vec3 scale);
void drawCyllinder(vec3 pos, vec3 rot, float r, float h);
void drawCone(vec3 pos, vec3 rot, float r, float h);
void drawSphere(vec3 pos, vec3 rot, float r);


//util
void renderComment(string str);


extern renderLayer *loadLayer;		//data load commands go here
extern renderLayer *layer3D;		//3D drawing commands go here
extern renderLayer *layerDebug;			//3D debug indication goes here
extern renderLayer *layer2D;		//2D drawing commands go here
extern renderLayer *layerDebug2D;	//2D debug indication goes here
extern renderLayer *deleteLayer;	//data delete commands go here
void initLayers();


#endif
