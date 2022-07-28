#ifndef PAINT_GUARD
#define PAINT_GUARD
#include "Ext/glm/glm.hpp"
#include "render/printw.h"
#include "render/camera.h"
#include "math/vec.h"
#include "util/globals_render.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class renderLayer;
struct font;
struct texture;
class rmodel;
class model;

void DLLAPI_RENDER setLayer(renderLayer *L);			
void DLLAPI_RENDER addLayer(renderLayer *L);
void DLLAPI_RENDER addLayer(renderLayer *L1, renderLayer *L2);
renderLayer DLLAPI_RENDER *addNewLayer(string name = "", bool persistent = true, bool special = false);
renderLayer DLLAPI_RENDER *duplicateLayer(renderLayer *L);
void DLLAPI_RENDER clearLayer(renderLayer *L);
void DLLAPI_RENDER resetLayer(renderLayer *L);		
void DLLAPI_RENDER removeLayer(renderLayer *L);		
void DLLAPI_RENDER renderAllLayers();					
void DLLAPI_RENDER printAllLayers();					
void DLLAPI_RENDER printAllLayersNextRender();		
void DLLAPI_RENDER frameReport();						
void DLLAPI_RENDER frameReportNextRender();			
void DLLAPI_RENDER setColoring(bool b);				
void DLLAPI_RENDER setUvColoring(bool b);				
void DLLAPI_RENDER setNormalColoring(bool b);			
void DLLAPI_RENDER setTransparency(bool b);			
void DLLAPI_RENDER setDepthMask(bool b);				
void DLLAPI_RENDER setTexturing(bool b);				
void DLLAPI_RENDER setDebug(bool b);					
void DLLAPI_RENDER setScissoring(bool b);				
void DLLAPI_RENDER setDepthTest(bool b);				
void DLLAPI_RENDER setLighting(bool b);				
void DLLAPI_RENDER drawLayer(renderLayer *L);			
void DLLAPI_RENDER setColor(vec3 col);				
void DLLAPI_RENDER setAlpha(float a);					
void DLLAPI_RENDER setTexture(texture *t);			
void DLLAPI_RENDER setFont(font *f);					
void DLLAPI_RENDER setRenderMode(int mode);			
void DLLAPI_RENDER setTextPos(vec2 textPos); 			
void DLLAPI_RENDER setScissor(rect S);				
void DLLAPI_RENDER setPointSize(float size);			
void DLLAPI_RENDER setLineWidth(float width);			
void DLLAPI_RENDER setSunPos(vec3 pos);				
void DLLAPI_RENDER setSunColor(vec3 val);				
void DLLAPI_RENDER setAmbientColor(vec3 val);			
void DLLAPI_RENDER uploadTexture(texture *t);			
void DLLAPI_RENDER uploadRmodel(rmodel *rm);			
void DLLAPI_RENDER deleteRmodel(rmodel *rm);			
void DLLAPI_RENDER setProjection(camprojection cpj);	
void DLLAPI_RENDER setProjectionToCamera(cameraKind *camera);
void DLLAPI_RENDER setPosition(vec3 pos);				
void DLLAPI_RENDER setRotation(vec3 rot);				
void DLLAPI_RENDER setScale(vec3 scale);				
void DLLAPI_RENDER setTexturePosition(vec2 pos);		
void DLLAPI_RENDER setTextureScale(vec2 scale);		
void DLLAPI_RENDER clearScreen();						
void DLLAPI_RENDER drawRmodel(rmodel *rm);			
void DLLAPI_RENDER drawRmodelStd(rmodel *rm);			
void DLLAPI_RENDER printText(string text);		
void DLLAPI_RENDER pushRenderOptions();                
void DLLAPI_RENDER popRenderOptions();                 
void DLLAPI_RENDER setFaceCulling(bool b);
void DLLAPI_RENDER setFaceCullCCW();
void DLLAPI_RENDER readPixels(int x, int y, int w, int h, void* buff);
void DLLAPI_RENDER setViewport(int x, int y, int w, int h);
void DLLAPI_RENDER drawPoint(vec3 pos);
void DLLAPI_RENDER drawPoints(const vector<vec3> &points);
void DLLAPI_RENDER drawPoint(vec3 pos, vec3 color);
void DLLAPI_RENDER drawLine(vec3 start, vec3 end);
void DLLAPI_RENDER drawLine(vec3 start, vec3 end, vec3 color);
void DLLAPI_RENDER drawArrow(vec3 start, vec3 end);
void DLLAPI_RENDER drawArrow(vec3 start, vec3 end, vec3 color);
void DLLAPI_RENDER drawTriangle(vec3 A, vec3 B, vec3 C);
void DLLAPI_RENDER drawRect(rect R);
void DLLAPI_RENDER drawRectOutline(rect R);
void DLLAPI_RENDER drawRectOutlineColored(rect R, vec3 col); 
void DLLAPI_RENDER drawImage(texture *t, rect R);
void DLLAPI_RENDER debugFloatingText(vec3 p, string S);
void DLLAPI_RENDER drawModel(vec3 pos, vec3 rot, vec3 scale, model *m);
void DLLAPI_RENDER drawBoxWireframe(AABB aabb);
void DLLAPI_RENDER drawCyllinderWireframe(AABB aabb);
void DLLAPI_RENDER drawConeWireframe(AABB aabb);
void DLLAPI_RENDER drawSphereWireframe(AABB aabb);
void DLLAPI_RENDER drawBoxWireframe(vec3 pos, vec3 rot,  vec3 scale);
void DLLAPI_RENDER drawCyllinderWireframe(vec3 pos, vec3 rot, float r, float h);
void DLLAPI_RENDER drawConeWireframe(vec3 pos, vec3 rot, float r, float h);
void DLLAPI_RENDER drawSphereWireframe(vec3 pos, vec3 rot, float r);
void DLLAPI_RENDER drawBox(vec3 pos, vec3 rot,  vec3 scale);
void DLLAPI_RENDER drawCyllinder(vec3 pos, vec3 rot, float r, float h);
void DLLAPI_RENDER drawCone(vec3 pos, vec3 rot, float r, float h);
void DLLAPI_RENDER drawSphere(vec3 pos, vec3 rot, float r);
void DLLAPI_RENDER renderComment(string str);
void DLLAPI_RENDER initLayers();
void DLLAPI_RENDER uploadFont(font* f);

struct DLLAPI_RENDER gs_paintKind {
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
