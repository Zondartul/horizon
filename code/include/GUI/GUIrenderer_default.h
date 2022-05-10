#ifndef GUIRENDERER_DEFAULT_GUARD
#define GUIRENDERER_DEFAULT_GUARD
#include <string>
#include "GUIrenderer.h"
using std::string;
class GUIbase;
class GUIrenderer_default:public GUIrenderer{
	public:
	GUIrenderer_default();
	virtual ~GUIrenderer_default();
	virtual void render(GUIbase *el, string type = "");
};
struct gs_GUIrenderer_defaultKind {
	GUIrenderer_default GUIR_default;
};
#endif