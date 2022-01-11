#ifndef GUIRENDERER_DEFAULT_GUARD
#define GUIRENDERER_DEFUALT_GUARD

class GUIrenderer_default:public GUIrenderer{
	public:
	GUIrenderer_default();
	virtual ~GUIrenderer_default();
	virtual void render(GUIbase *el, string type = "");
};

extern GUIrenderer_default GUIR_default;

#endif