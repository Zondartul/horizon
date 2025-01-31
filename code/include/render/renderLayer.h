#ifndef RENDERLAYER_GUARD
#define RENDERLAYER_GUARD
#include <vector>
using std::vector;
#include "vec.h"
#include "stdint.h"
#include <string>
using std::string;
#include "camera.h"
#include "stringUtils.h"
#include "renderCommand.h"
#include "util.h"
//texture has pimpl: textureRenderHandle *handle;
//is usually GLuint;
//rmodel has pimpl: rmodelRenderHandle *handle;
//is usually GLuint[4];

typedef vector<renderCommand3*> renderQueue3;

class renderLayer{
	public:
	string name;
	bool persistent = true;
	bool special = false;
	renderLayer *resetLayer = 0;
	renderLayer() = default;
	renderLayer(string name,bool persistent=true, bool special=false);
	void render();
	void clear();
	void reset();	//add a call to the resetLayer
	void print();
    string report3();   //same but with rcmd3 queue
	void push(renderCommand3* cmd);
    const renderCommand3* get(unsigned int num);
	renderLayer *duplicate();
	private:
	renderQueue3 queue3;
};
string toString(renderLayer *l);

//extern vector<renderLayer*> g_all_layers;		//all layers (even those not in use)
//extern vector<renderLayer*> g_layers;			//main rendering sequence

void setupLayer3D();
void setupLayer2D();
void setupLayers();

class renderExKind: public exKind{
    public:
    renderExKind(logmessage lmsg_, renderLayer *L, int cmdNum);
};

#define renderEx(L,N,...) renderExKind(logmessage(fstring( __VA_ARGS__ ), __FILE__, __LINE__ ),L,N)

struct gs_renderLayerKind {
	vector<renderLayer*> g_all_layers;
	vector<renderLayer*> g_layers;
};

#endif
