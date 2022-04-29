#ifndef EDITOR1_GUARD
#define EDITOR1_GUARD
#include "vec.h"
#include "editmodel.h"
#include "global_vars.h"
#include "event.h"
class renderLayer;

void openEditor2();
class editor2Kind :public eventListener {
public:
	editor2Kind();

	struct {
		renderLayer* l3D;
		renderLayer* l3Dimmediate;
		renderLayer* l2D;
		renderLayer* l2Dimmediate;
	} layers;
	void setupLayers();
	void resetLayer(renderLayer* L);

	e_model EM;
	void constructTestModel();
	e_selection sel;
	void redraw();
	void printselection();

	void think();
	void onEvent(eventKind event);

	bool hasLastPoint = false;
	bool showLine = false;
	vec3 lastPoint;

	bool boxSelectOn = false;
	bool hasBox = false;
	vec2 boxStart;
	vec2 boxEnd;
	void boxSelect(vec2 boxStart, vec2 boxEnd);
};

struct gs_editor2Kind {
	/*static*/ vec3 g_plane[3] = { vec3(0,0,0),vec3(1,0,0),vec3(0,1,0) };
	editor2Kind* g_editor2;
};

#endif
