#include "GUI/GUI_internal.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
using namespace std;
//using std::stringstream;

GUIbase::GUIbase(){
	auto& GUIR_default = Gg->gs_GUIrenderer_default->GUIR_default;
	parent = NULL;
	isClient = true;
	hidden = false;
	suppressInvalidate = false;
	area = defaultarea;
	clientArea = defaultclientarea;
	renderer = &GUIR_default;
	blockChildInput = false;
	deletePending = false;
	render_debug = false;
	render_noscissor = false;
	mouseover = false;
}
GUIbase::~GUIbase(){
	if (!deletePending){ cout << "GUI widget deleted without close()" << endl; } //{ throw std::runtime_error("GUI widget deleted without close()\n"); }//{error("GUI widget deleted without close()\n");}
	if(parent){parent->removeChild(this);}
	{
		//stackSentinel SS;
		//printf("deleting children\n");
		vector<GUIbase*> children2 = children;
		for(auto I = children2.begin(); I != children2.end(); I++){
			GUIbase *ch = *I;
			delete ch;
		}
	}
}
GUIbase *GUIbase::addChild(GUIbase *child){
	addChild(child, children.end());
	return this;
}
GUIbase::ChI GUIbase::addChild(GUIbase *child, ChI iter){
	child->parent = this;
	ChI iter2 = children.insert(iter, child);
	if(child->isClient){clientChannel.addListenerFront(child);}
	else{partChannel.addListenerFront(child);}
	invalidateTree();
	return iter2;
}
void GUIbase_debugChildren(GUIbase *b){
	if (!b) { throw std::runtime_error("'this' is null"); }//{ error("'this' is null"); }
	printf("printing %p's %d children:\n",b,b->children.size());
	if(!b->children.size()){printf("%p has no children\n", b);return;}
	int i = 0;
	for(auto I = b->children.begin(); I != b->children.end(); I++){
		GUIbase *b2 = *I;
		string type = typeid(*b2).name();
		printf("%d: %p (%s)\n",i,*I,type.c_str());
		i++;
	}
}
GUIbase *GUIbase::removeChild(GUIbase *child){
	if(!isParentOf(child)){
		//error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));
		stringstream ss;
		ss << "GUI widget (" << toString(this) << ") is not the parent of (" << toString(child) << ")\n";
		throw std::runtime_error(ss.str());
	}
	bool found = false;
	//int i = 0;
	for(auto I = children.begin(); I != children.end();){ 
		if(*I == child){
			if(!child->isClient){partChannel.removeListener(child);}
			else{clientChannel.removeListener(child);}
			I = children.erase(I);
			found = true;
		}else{
			I++;
		}
	}
	if(!found){printf("couldn't erase %p: not found\n",child);}
	invalidateTree();
	return this;
}
GUIbase *GUIbase::moveChildToTheTop(GUIbase *child){
	if(!isParentOf(child)){
		//error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));
		stringstream ss;
		ss << "GUI widget (" << toString(this) << ") is not the parent of (" << toString(child) << ")\n";
		throw std::runtime_error(ss.str());
	}
	//bool found= false;
	for(auto I = children.begin(); I != children.end(); I++){
		if(*I == child){
			I = children.erase(I);
			children.push_back(child);
		}
	}
	if(!child->isClient){partChannel.moveListenerToFront(child);}
	else{clientChannel.moveListenerToFront(child);}
	return this;
}
GUIbase *GUIbase::makePart(GUIbase *child){
	if(!isParentOf(child)){
		//error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));
		stringstream ss;
		ss << "GUI widget (" << toString(this) << ") is not the parent of (" << toString(child) << ")\n";
		throw std::runtime_error(ss.str());
	}
	child->isClient = false;
	clientChannel.removeListener(child);
	partChannel.addListenerFront(child);
	return this;
}
GUIbase *GUIbase::makeClient(GUIbase *child){
	if(!isAncestorOf(child)){
		//error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));
		stringstream ss;
		ss << "GUI widget (" << toString(this) << ") is not the parent of (" << toString(child) << ")\n";
		throw std::runtime_error(ss.str());
	}
	child->isClient = true;
	partChannel.removeListener(child);
	clientChannel.addListenerFront(child);
	return this;
}
GUIbase *GUIbase::setSize(vec2 newSize){
	area = area.setSize(newSize);
	invalidate();
	return this;
}
GUIbase *GUIbase::moveTo(vec2 newstart){
	area = area.moveTo(newstart);
	invalidate();
	return this;
}
GUIbase *GUIbase::setClientArea(rect newclientarea){
	clientArea = newclientarea;
	return this;
}
GUIbase *GUIbase::setHidden(bool newHidden){
	hidden = newHidden;
	return this;
}
GUIbase *GUIbase::sizeToContents(){
	float maxx = 0;
	float maxy = 0;
	for(unsigned int I = 0; I < children.size(); I++){
		maxx = max(maxx, children[I]->area.end.x);
		maxy = max(maxy, children[I]->area.end.y);
	}
	vec2 newsize = {maxx,maxy};
	vec2 diff = newsize - clientArea.size;
	clientArea = clientArea.setSize(newsize);
	area = area.setSize(area.size+diff);
	return this;
}
void GUIbase::tickLogic(){
	tick();
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->tickLogic();
	}
}
void GUIbase::tick(){}
void GUIbase::renderLogic(){
	auto& GUIoptions = Gg->gs_GUI_internal->g_GUIoptions;
	if(!parent){GUIsetFavoriteRenderOptions();}
	for(unsigned int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			}else{
				setScissoring(true);
				setScissor(visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	for(unsigned int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{128,0,0});
			}else{
				setScissoring(true);
				setScissor(visibleClientArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}
}
void GUIbase::render(){ if(renderer){renderer->render(this);} }
void GUIbase::onEvent(eventKind event){
	bool transmit = true;
	if(!parent){
		if(event.type == EVENT_MOUSE_MOVE){
			recalcMouseoverTree();
		}
		if(event.type == EVENT_TICK){
			tickLogic();
		}
		if(event.type == EVENT_CLEANUP){
			checkCloseTree();
		}
		//if (event.type == EVENT_WINDOW_RESIZE) {
		//	invalidateTree();
		//}
	}
	if(blockChildInput){
		switch(event.type){
			case EVENT_KEY_UP: 				
			case EVENT_KEY_DOWN: 			
			case EVENT_MOUSE_BUTTON_UP: 	
			case EVENT_MOUSE_BUTTON_DOWN:
			case EVENT_MOUSE_WHEEL:
				transmit = false;
			break;
			case EVENT_FRAME: 		
			case EVENT_TICK:
			case EVENT_CLEANUP:
			case EVENT_MOUSE_MOVE: 			
			default:
			break;
		}
	}
	partChannel.publishEventParallelMaskable(event);
	if(transmit){
		int shouldMask = clientChannel.publishEventParallelMaskable(event);
		if(event.mask){
			event.maskEvent(shouldMask);
		}
	}else{
	}
}
GUIbase *GUIbase::root(){
	GUIbase *p = this;
	for(int I = 0; I < 500; I++){
		if(p->parent){p = p->parent;}
		else{return p;}
	}
	throw std::runtime_error("GUIbase::root: recursion detected\n");//error("GUIbase::root: recursion detected\n");
	return 0;
}
GUIbase *GUIbase::getMouseoverElement(){
	if(!mouseover){return 0;}
	for(auto I = children.begin(); I != children.end(); I++){
		if((*I)->mouseover){
			return (*I)->getMouseoverElement();
		}
	}
	return this;
}
bool GUIbase::isAncestorOf(GUIbase *child){
	while(child && child->parent){
		if(child->parent == this){return true;}
		child = child->parent;
	}
	return false;
}
bool GUIbase::isParentOf(GUIbase *child){
	return (child && (child->parent == this));
}
GUIbase *GUIbase::getByHelper(string askname, string asktype){
	if((askname != "") && (name == askname)){return this;}
	if((asktype != "") && (getType() == asktype)){return this;}
	for(auto I = children.begin(); I != children.end(); I++){
		GUIbase *B = (*I)->getByHelper(askname,asktype);
		if(B){return B;}
	}
	return 0;
}
GUIbase *GUIbase::getByName(string name){
	GUIbase *B = getByHelper(name,"");
	if(!B){printf("can't find widget by name: [%s]\n",name.c_str());}
	else{printf("found widget by name: [%s]\n", name.c_str());}
	return B;
}
GUIbase *GUIbase::getByType(string type){
	GUIbase *B = getByHelper("",type);
	if(!B){printf("can't find widget by type: [%s]\n",type.c_str());}
	else{printf("found widget by type: [%s]\n",type.c_str());}
	return B;
}
void GUIbase::invalidate(){
	clientArea = rect().setStart({1,1}).setEnd(area.size-vec2{1,1});
}
void GUIbase::invalidateTree(){
	if(suppressInvalidate){return;}
	if(parent){
		root()->invalidateTree();
	}else{
		invalidateDown();
	}
}
void GUIbase::invalidateDown(){
	invalidate();
	bool prevsuppress = suppressInvalidate;
	suppressInvalidate = true;
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->invalidateDown();
	}
	suppressInvalidate = prevsuppress;
}
void GUIbase::recalcMouseover(){
	vec2 pos = getMousePos();
	if(!visibleArea().contains(pos)){mouseover = false; return;}
	if(parent){
		if(!parent->mouseover){mouseover = false; return;}
		auto I = parent->children.begin();
		while(*I != this){I++;}
		I++;
		for(;I != parent->children.end(); I++){
			if((*I)->mouseover){mouseover = false; return;}
		}
	}
	mouseover = true;
}
void GUIbase::recalcMouseoverTree(){
	recalcMouseover();
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->recalcMouseoverTree();
	}
}
void GUIbase::close(){
	deletePending = true;
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->close();
	}
}
void GUIbase::checkCloseTree(){
	vector<GUIbase*> children2 = children; 
	for(auto I = children2.begin(); I != children2.end(); I++){
		GUIbase *el = *I;
		if(el->deletePending){delete el;}
		else{el->checkCloseTree();}
	}
}
vec2 GUIbase::thisToWorld(vec2 L) const{
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start;
		}else{
			return (parent->thisToWorld(L))+area.start;
		}
	}else{
		return L+area.start;
	}
}
rect GUIbase::thisToWorld(rect L) const{
	return L.setStart(thisToWorld(L.start)).setEnd(thisToWorld(L.end));
}
vec2 GUIbase::clientToWorld(vec2 L) const{
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start+clientArea.start;
		}else{
			return (parent->thisToWorld(L))+area.start+clientArea.start;
		}
	}else{
		return L+area.start+clientArea.start;
	}
}
rect GUIbase::clientToWorld(rect L) const{
	return L.setStart(clientToWorld(L.start)).setEnd(clientToWorld(L.end));
}
rect GUIbase::worldArea() const{
	if(parent){
		return thisToWorld(rect(area.size));
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::worldClientArea() const{
	if(parent){
		return thisToWorld(clientArea);
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::visibleArea() const{
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			return parent->visibleClientArea().clamp(worldArea());
		}else{
			return parent->visibleArea().clamp(worldArea());
		}
	}else{
		return worldArea();
	}
}
rect GUIbase::visibleClientArea() const{
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			return parent->visibleClientArea().clamp(worldClientArea());
		}else{
			return parent->visibleArea().clamp(worldClientArea());
		}
	}else{
		return worldClientArea();
	}
}
GUI_border_rects GUIbase::getBorders(GUI_border_size bsize) const{
	rect warea = worldArea();
	vec2 AA = warea.start;
	vec2 AB = AA + vec2(bsize.corner,0);
	vec2 AC = AB + vec2(0,bsize.top);
	vec2 AD = AA + vec2(0,bsize.corner);
	vec2 AE = AD + vec2(bsize.left,0);
	vec2 AF = vec2(AB.x,AD.y);
	vec2 BA = vec2(warea.end.x, warea.start.y);
	vec2 BB = BA + vec2(-bsize.corner,0);
	vec2 BC = BB + vec2(0,bsize.top);
	vec2 BD = BA + vec2(0,bsize.corner);
	vec2 BE = BD + vec2(-bsize.right,0);
	vec2 BF = vec2(BB.x,BD.y);
	vec2 CA = vec2(warea.start.x, warea.end.y);
	vec2 CB = CA + vec2(bsize.corner,0);
	vec2 CC = CB + vec2(0,-bsize.bottom);
	vec2 CD = CA + vec2(0,-bsize.corner);
	vec2 CE = CD + vec2(bsize.left,0);
	vec2 CF = vec2(CB.x,CD.y);
	vec2 DA = warea.end;
	vec2 DB = DA + vec2(-bsize.corner,0);
	vec2 DC = DB + vec2(0,-bsize.bottom);
	vec2 DD = DA + vec2(0,-bsize.corner);
	vec2 DE = DD + vec2(-bsize.right,0);
	vec2 DF = vec2(DB.x, DD.y);
	GUI_border_rects border;
	border.Rtop		= rect(AB,BC);
	border.Rbottom	= rect(CC,DB);
	border.Rleft	= rect(AD,CE);
	border.Rright	= rect(BE,DD);
	border.Rctl		= rect(AA,AF);
	border.Rctr		= rect(BB,BD);
	border.Rcbl		= rect(CD,CB);
	border.Rcbr		= rect(DF,DA);
	return border;
}
GUIe_border GUIbase::testBorders(vec2 pos, GUI_border_size borderSize) const{
	GUI_border_rects border = getBorders(borderSize);
	GUIe_border condition = GUIb::None;
	if(border.Rctl.contains(pos))	{condition |= GUIb::Corner_TL;}
	if(border.Rtop.contains(pos))	{condition |= GUIb::Top;}
	if(border.Rctr.contains(pos))	{condition |= GUIb::Corner_TR;}
	if(border.Rleft.contains(pos))	{condition |= GUIb::Left;}
	if(border.Rright.contains(pos))	{condition |= GUIb::Right;}
	if(border.Rcbl.contains(pos))	{condition |= GUIb::Corner_BL;}
	if(border.Rbottom.contains(pos)){condition |= GUIb::Bottom;}
	if(border.Rcbr.contains(pos))	{condition |= GUIb::Corner_BR;}
	return condition;
}
GUIpropertyTable GUIbase::getDefaultPropertyTable(){
	GUIpropertyTable table;
	table.table["name"] 		= name;
	table.table["type"]			= getType();
	table.table["isClient"] 	= toString(bool(false));
	table.table["hidden"] 	= toString(bool(false));
	table.table["area"] 		= toString(rect(defaultarea));
	return table;
}
GUIpropertyTable GUIbase::getPropertyTable(){
	GUIpropertyTable table = getDefaultPropertyTable();
	for(auto I = table.table.begin(); I != table.table.end(); I++){
		auto key = I->first;
		auto val = I->second;
		table.table[key] = getProperty(key);
	}
	return table;
}
void GUIbase::setPropertyTable(GUIpropertyTable table){
	for(auto I = table.table.begin(); I != table.table.end(); I++){
		auto key = I->first;
		auto val = I->second;
		setProperty(key,val);
	}
}
string GUIbase::getProperty(string key){
		 if(key == "name")		{return name;}
	else if(key == "type")		{return getType();}
	else if(key == "isClient")	{return toString(isClient);}
	else if(key == "hidden")	{return toString(hidden);}
	else if(key == "area")		{return toString(area);}
	return "";
}

void GUIbase::setProperty(string key, string val){
	/// NOTE: parse errors here are fatal
	/// because we are probably constructing new GUI stuff
		 if(key == "name")		{name		= val;}
		 else if (key == "type") {
			 if (getType() != val) {
				 //error("attmpt to change GUI widget type\n");}
				 throw std::runtime_error("attempt to change GUI widget type\n");
			 }
		 }
	else if(key == "isClient")	{isClient 	= require(fromString<bool>(val));}
	else if(key == "hidden")	{hidden 	= require(fromString<bool>(val));}
	else if(key == "area")		{area		= require(fromString<rect>(val));}
}
string GUIbase::getType(){return "GUIbase";}
GUIcompoundProperty GUIbase::getCompoundProperty(){
	GUIcompoundProperty prop;
	prop.name = getType();
	prop.table = getPropertyTable();
	for(auto I = children.begin(); I != children.end(); I++){
		GUIbase *C = *I;
		prop.children.push_back(C->getCompoundProperty());
	}
	return prop;
}
void GUIbase::setCompoundProperty(const GUIcompoundProperty prop){
	if(prop.name == getType()){
		setPropertyTable(prop.table);
	}
	auto Ic = children.begin();
	auto Ip = prop.children.begin();
	for(;
		(Ic != children.end()) && (Ip != prop.children.end());){
		GUIbase *C = *Ic;
		const GUIcompoundProperty &P = *Ip;
		if(C->getType() == P.name){
			C->setCompoundProperty(P);
		}else{
			GUIbase *C2 = P.instantiate();
			if(C2){Ic = addChild(C2,Ic);}
		}
		Ic++; Ip++;
	}
	while(Ip != prop.children.end()){
		const GUIcompoundProperty &P = *Ip;
		GUIbase *C2 = P.instantiate();
		if(C2){Ic = addChild(C2,Ic);}
		Ip++;
	}
}

vector<GUIbase*> GUIbase::getChildren(bool own){
	vector<GUIbase*> res;
	for(auto ch:children){
		if(ch->isClient != own){res.push_back(ch);}
	}
	return res;
}
vector<const GUIbase*> GUIbase::getChildren(bool own) const{
	vector<const GUIbase*> res;
	for(auto ch:children){
		if(ch->isClient != own){res.push_back(ch);}
	}
	return res;
}

bool GUIbase::hasParent(GUIbase *node) const{
	GUIbase *node2 = parent;
	while(node2){
		if(node2 == node){return true;}
		node2 = node2->parent;
	}
	return false;
}
