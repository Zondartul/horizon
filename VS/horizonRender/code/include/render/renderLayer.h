#ifndef RENDERLAYER_GUARD
#define RENDERLAYER_GUARD
#include "math/vec.h"
#include "render/camera.h"
#include "render/renderCommand.h"
#include "util/stringUtils.h"
#include "util/util.h"
#include "util/globals_render.h"
#include <string>
#include <vector>
#include <cstdint>
using std::vector;
using std::string;


typedef vector<renderCommand3*> renderQueue3;

class DLLAPI_RENDER renderLayer {
public:
	string name;
	bool persistent = true;
	bool special = false;
	renderLayer* resetLayer = 0;
	renderLayer() = default;
	renderLayer(string name, bool persistent = true, bool special = false);
	void render();
	void clear();
	void reset();	//add a call to the resetLayer
	void print();
	string report3();   //same but with rcmd3 queue
	void push(renderCommand3* cmd);
	const renderCommand3* get(unsigned int num);
	renderLayer* duplicate();
private:
	renderQueue3 queue3;
};
string DLLAPI_RENDER toString(renderLayer *l);
void DLLAPI_RENDER setupLayer3D();
void DLLAPI_RENDER setupLayer2D();
void DLLAPI_RENDER setupLayers();

struct DLLAPI_RENDER gs_renderLayerKind {
	vector<renderLayer*> g_all_layers;
	vector<renderLayer*> g_layers;
};

#endif
