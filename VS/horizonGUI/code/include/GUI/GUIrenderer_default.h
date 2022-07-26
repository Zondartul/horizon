#ifndef GUIRENDERER_DEFAULT_GUARD
#define GUIRENDERER_DEFAULT_GUARD
#include "GUI/GUIrenderer.h"
#include "util/globals_gui.h"
#include <string>
using std::string;

class GUIbase;

class DLLAPI_GUI GUIrenderer_default:public GUIrenderer{
	public:
	GUIrenderer_default();
	virtual ~GUIrenderer_default();
	virtual void render(GUIbase *el, string type = "");
};

struct DLLAPI_GUI gs_GUIrenderer_defaultKind {
	GUIrenderer_default GUIR_default;
};

#endif