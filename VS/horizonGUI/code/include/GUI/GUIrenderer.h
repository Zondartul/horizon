#ifndef GUIRENDERER_GUARD
#define GUIRENDERER_GUARD
#include "util/globals_gui.h"
#include <string>
using std::string;

class GUIbase;

class DLLAPI_GUI GUIrenderer{
	public:
	string name;
	GUIrenderer();
	virtual ~GUIrenderer();
	virtual void render(GUIbase *el, string type = "") = 0;
};

#endif