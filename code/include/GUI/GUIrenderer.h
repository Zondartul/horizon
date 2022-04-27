#ifndef GUIRENDERER_GUARD
#define GUIRENDERER_GUARD
#include <string>
using std::string;

// The GUIrenderer class is an abstract interface for various renderers (skins)
// of the GUI elements.
// A GUIrenderer can be set on a per-element basis.
// If the renderer has no state, it can be shared
// having multiple instances is fine too
// an element should be able to pretend to be any other element
// color should be set independently from element type

class GUIbase;

class GUIrenderer{
	public:
	string name;
	
	GUIrenderer();
	virtual ~GUIrenderer();
	virtual void render(GUIbase *el, string type = "") = 0;
};

#endif