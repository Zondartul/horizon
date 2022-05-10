#ifndef PAINT_GUARD
#define PAINT_GUARD
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "printw.h"
#include "vec.h"
#include "camera.h"
using std::string;
using std::vector;
class renderLayer;
struct font;
struct texture;
class rmodel;
class model;



void setLayer(renderLayer *L);			
void addLayer(renderLayer *L);			
void addLayer(renderLayer *L1, renderLayer *L2);	
renderLayer *addNewLayer(string name = "", bool persistent = true, bool special = false);				
renderLayer *duplicateLayer(renderLayer *L); 
void clearLayer(renderLayer *L);		
void resetLayer(renderLayer *L);		

void removeLayer(renderLayer *L);		
void renderAllLayers();					
void printAllLayers();					
void printAllLayersNextRender();		
void frameReport();						
void frameReportNextRender();			

void setColoring(bool b);				
void setUvColoring(bool b);				
void setNormalColoring(bool b);			
void setTransparency(bool b);			
void setDepthMask(bool b);				
void setTexturing(bool b);				
void setDebug(bool b);					
void setScissoring(bool b);				
void setDepthTest(bool b);				
void setLighting(bool b);				

void drawLayer(renderLayer *L);			
void setColor(vec3 col);				
void setAlpha(float a);					
void setTexture(texture *t);			
void setFont(font *f);					
void setRenderMode(int mode);			
void setTextPos(vec2 textPos); 			
void setScissor(rect S);				
void setPointSize(float size);			
void setLineWidth(float width);			
void setSunPos(vec3 pos);				
void setSunColor(vec3 val);				
void setAmbientColor(vec3 val);			

void uploadTexture(texture *t);			
void uploadRmodel(rmodel *rm);			
void deleteRmodel(rmodel *rm);			

void setProjection(camprojection cpj);	
void setProjectionToCamera(cameraKind *camera);
void setPosition(vec3 pos);				
void setRotation(vec3 rot);				
void setScale(vec3 scale);				

void setTexturePosition(vec2 pos);		
void setTextureScale(vec2 scale);		


void clearScreen();						
void drawRmodel(rmodel *rm);			
void drawRmodelStd(rmodel *rm);			
										
void printText(string text);		
void pushRenderOptions();                
void popRenderOptions();                 


void setFaceCulling(bool b);
void setFaceCullCCW();
void readPixels(int x, int y, int w, int h, void* buff);
void setViewport(int x, int y, int w, int h);


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
void drawRectOutlineColored(rect R, vec3 col); 
void drawImage(texture *t, rect R);

void debugFloatingText(vec3 p, string S);

void drawModel(vec3 pos, vec3 rot, vec3 scale, model *m);


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



void renderComment(string str);







void initLayers();

struct gs_paintKind {
	renderLayer* g_loadLayer;		
	renderLayer* g_layer3D;		
	renderLayer* g_layerDebug;	
	renderLayer* g_layer2D;		
	renderLayer* g_layerDebug2D;	
	renderLayer* g_deleteLayer;	

	renderLayer* g_currentLayer = 0;

	rmodel* g_rm_unitboxWF = 0;
	rmodel* g_rm_unitcyllinderWF = 0;
	rmodel* g_rm_unitconeWF = 0;
	rmodel* g_rm_unitsphereWF = 0;

	rmodel* g_rm_unitbox = 0;
	rmodel* g_rm_unitcyllinder = 0;
	rmodel* g_rm_unitcone = 0;
	rmodel* g_rm_unitsphere = 0;

	bool g_printAllPending = false;
	bool g_frameReportPending = false;
};

#endif
