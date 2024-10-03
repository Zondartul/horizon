#ifndef GUIBASE_GUARD
#define GUIBASE_GUARD
#include "util/globals_gui.h"

class DLLAPI_GUI GUIbase:public eventListener, public elastic_ptr_anchor{
	public:
	string name;	
	GUIbase *parent;
	vector<GUIbase*> children;
	typedef typename vector<GUIbase*>::iterator ChI;
	eventChannel clientChannel; 
	eventChannel partChannel;
	rect area;
	rect clientArea;
	bool isClient;		 
	bool mouseover;	
	bool suppressInvalidate;
	bool hidden;	
	bool blockChildInput;
	bool deletePending;
	GUIrenderer *renderer;
	bool render_debug;	   
	bool render_noscissor; 
	GUIbase();
	virtual ~GUIbase(); 
	virtual GUIbase *addChild(GUIbase *child);
	virtual ChI addChild(GUIbase *child, ChI iter);
	virtual GUIbase *removeChild(GUIbase *child);
	virtual GUIbase *moveChildToTheTop(GUIbase *child);
	virtual GUIbase *makePart(GUIbase *child);
	virtual GUIbase *makeClient(GUIbase *child);
	virtual GUIbase *setSize(vec2 newSize);
	virtual GUIbase *moveTo(vec2 newStart);
	virtual GUIbase *setClientArea(rect newclientarea);
	virtual GUIbase *setHidden(bool newHidden);
	virtual GUIbase *sizeToContents();
	GUIbase *root();
	GUIbase *getMouseoverElement();
	bool isAncestorOf(GUIbase *child);
	bool isParentOf(GUIbase *child);
	GUIbase *getByName(string name);
	GUIbase *getByType(string type);
	GUIbase *getByHelper(string askname, string asktype);
	virtual void tickLogic();
	virtual void tick();
	virtual void renderLogic();
	virtual void render();
	virtual void onEvent(eventKind event);
	virtual void invalidate();
	virtual void invalidateTree();
	virtual void invalidateDown();
	virtual void recalcMouseover();
	virtual void recalcMouseoverTree();
	virtual void close();
	virtual void checkCloseTree();
	virtual vec2 thisToWorld(vec2 L) const;
	virtual rect thisToWorld(rect L) const;
	virtual vec2 clientToWorld(vec2 L) const;
	virtual rect clientToWorld(rect L) const;
	virtual rect worldArea() const;
	virtual rect worldClientArea() const;
	virtual rect visibleArea() const;
	virtual rect visibleClientArea() const;
	GUI_border_rects getBorders(GUI_border_size border = GUI_border_size()) const;
	GUIe_border testBorders(vec2 pos, GUI_border_size border =  GUI_border_size()) const;
	virtual string getType();
	virtual GUIpropertyTable getPropertyTable();
	virtual void setPropertyTable(GUIpropertyTable table);
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key); 
	virtual void setProperty(string key, string val);
	virtual GUIcompoundProperty getCompoundProperty();
	virtual void setCompoundProperty(const GUIcompoundProperty prop);
	vector<GUIbase*> getChildren(bool own=false);
	vector<const GUIbase*> getChildren(bool own=false) const;
	virtual bool hasParent(GUIbase *node) const;
};

#endif
