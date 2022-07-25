#ifndef GUIIMAGE_GUARD
#define GUIIMAGE_GUARD
class GUIimage:public virtual GUIframe{
	public:
	GUIimage();
	texture *image;
	GUIimage *setImage(texture *newimage);
	virtual GUIbase *sizeToContents();
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);
	virtual string getType(); 
};
#endif 
