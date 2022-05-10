#ifndef GUIRENDERER_GUARD
#define GUIRENDERER_GUARD
#include <string>
using std::string;









class GUIbase;

class GUIrenderer{
	public:
	string name;
	
	GUIrenderer();
	virtual ~GUIrenderer();
	virtual void render(GUIbase *el, string type = "") = 0;
};

#endif