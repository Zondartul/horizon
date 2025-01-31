#ifndef RENDERLAYER_GUARD
#define RENDERLAYER_GUARD
#include <string>
#include <vector>
#include "vec.h"
#include "stdint.h"
#include "camera.h"
#include "stringUtils.h"
#include "renderCommand.h"
#include "util.h"
using std::vector;
using std::string;


typedef vector<renderCommand3*> renderQueue3;

class renderLayer {
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
string toString(renderLayer *l);

struct gs_renderLayerKind {
	vector<renderLayer*> g_all_layers;
	vector<renderLayer*> g_layers;
};

#endif
