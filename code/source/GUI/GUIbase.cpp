#include "GUI_internal.h"

//GUIbase
GUIbase::GUIbase(){
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

//int gbd_indent = 0;
//#define printind() for(int I = 0; I < gbd_indent; I++){printf(" ");}
/*
void stackBreaker(int a){
	int *x = &a;
	for(int I = -10; I < 10; I++){
		x[I] = 123;
	}
}
*/
GUIbase::~GUIbase(){
	if(!deletePending){error("GUI widget deleted without close()\n");}
	//gbd_indent++;
	//printind(); printf("destructor of (%s): 1. remove from parent\n",getType().c_str());
	if(parent){parent->removeChild(this);}
	//printind(); printf("destructor of (%s): 2. delete children\n",getType().c_str());
	
	//dafuq, children werent being deleted?
	//make a separate immutable vector because children will remove themselves from the main list
	
	// ah shit, this causes a weird segfault later for no reason
	// it worked before tho, wtf? and we can't leave it commented or we leak memory & other bad things
	// happen like double event stuff
	// update: apparrently it's not the culprit but it's the fastest way to poke at the bug
	// the bug happens without it too. WHAT THE FUCK IS HAPPENING.
	
	{
		stackSentinel SS;
		//printf("testing stack\n");
		//stackBreaker(5);
		printf("deleting children\n");
		vector<GUIbase*> children2 = children;
		for(auto I = children2.begin(); I != children2.end(); I++){
			GUIbase *ch = *I;
			delete ch;
		}
	}
	
	//printind(); printf("desturctor of (%s) done\n", getType().c_str());
	//gbd_indent--;
}

GUIbase *GUIbase::addChild(GUIbase *child){
	addChild(child, children.end());
	return this;
}

//this change may break something in other objects
GUIbase::ChI GUIbase::addChild(GUIbase *child, ChI iter){
	child->parent = this;
	//children.push_back(child); //somehow, adding them to the front screws everything up
	ChI iter2 = children.insert(iter, child);
	//channel.addListenerFront(child);
	if(child->isClient){clientChannel.addListenerFront(child);}
	else{partChannel.addListenerFront(child);}
	invalidateTree();
	return iter2;
}

void GUIbase_debugChildren(GUIbase *b){
	if(!b){error("'this' is null");}
	printf("printing %p's %d children:\n",b,b->children.size());
	if(!b->children.size()){printf("%p has no children\n");return;}
	int i = 0;
	for(auto I = b->children.begin(); I != b->children.end(); I++){
		GUIbase *b2 = *I;
		string type = typeid(*b2).name();
		printf("%d: %p (%s)\n",i,*I,type.c_str());
		i++;
	}
}

GUIbase *GUIbase::removeChild(GUIbase *child){
	if(!isParentOf(child)){error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));}
	//printf("removeChild called\n");
	//GUIbase_debugChildren(this);
	bool found = false;
	int i = 0;
	for(auto I = children.begin(); I != children.end();){ //no I++ here
		if(*I == child){
			//printf("erased %p, %d\n",*I,i++);
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
	//printf("invalidated\n");
	return this;
}

GUIbase *GUIbase::moveChildToTheTop(GUIbase *child){
	if(!isParentOf(child)){error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));}
	bool found= false;
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
	if(!isParentOf(child)){error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));}
	child->isClient = false;
	clientChannel.removeListener(child);
	partChannel.addListenerFront(child);
	return this;
}

GUIbase *GUIbase::makeClient(GUIbase *child){
	if(!isAncestorOf(child)){error("GUI widget (%s) is not the parent of (%s)\n",toCString(this), toCString(child));}
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
	//printf("moveTo: new area = %s\n",toString(area).c_str());
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
	for(int I = 0; I < children.size(); I++){
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
	if(!parent){GUIsetFavoriteRenderOptions();}
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			}else{
				setScissoring(true);
				//setScissor(children[I]->visibleArea());
				setScissor(visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	//cropScissor(worldClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{128,0,0});
			}else{
				setScissoring(true);
				//setScissor(children[I]->visibleArea());
				setScissor(visibleClientArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	//popRenderOptions();
}

void GUIbase::render(){ if(renderer){renderer->render(this);} }



void GUIbase::onEvent(eventKind event){
	//if(event.type == EVENT_MOUSE_MOVE){
	//	recalcMouseover();	//this check needs to be already
	//						//done by the time children start handling events
	//}
	//event.maskEvent(channel.publishEventParallelMaskable(event));
	
	//root can initiate mouseover events
	bool transmit = true;
	if(!parent){
		if(event.type == EVENT_MOUSE_MOVE){
			recalcMouseoverTree();
		}
		if(event.type == EVENT_TICK){
			tickLogic();
		}
		if(event.type == EVENT_CLEANUP){
			//printf("event_cleanup\n");
			checkCloseTree();
		}
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
				//do nothing
			break;
		}
	}
	//1. publish to parts
	partChannel.publishEventParallelMaskable(event);
	//2. publish to clients
	if(transmit){
		//1. publish event to children
		int shouldMask = clientChannel.publishEventParallelMaskable(event);
		//2. possibly mask it
		//printf("widget (%s), ", getType().c_str());
		//printf((parent)? "non-root":"root");
		//printf((shouldMask)? " masking (%d)":" not masking (%d)", shouldMask);
		//printf(" event (%s)\n",toCString(event.type));
		
		if(event.mask){
			event.maskEvent(shouldMask);
		}
	}else{
		//1. do not transmit.
		//2. event is not captured - do not mask.
		//printf("widget (%s), ", getType().c_str());
		//printf((parent)? "non-root":"root");
		//printf(" blocking ");
		//printf(" event (%s)\n",toCString(event.type));
	}
}

GUIbase *GUIbase::root(){
	GUIbase *p = this;
	for(int I = 0; I < 500; I++){
		if(p->parent){p = p->parent;}
		else{return p;}
	}
	error("GUIbase::root: recursion detected\n");
	return 0;
//	if(parent){
//		GUIbase *p = parent;
//		while(p->parent){p = p->parent;}
//		return p;
//	}else{
//		return this;
//	}
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
	//depth first search, because it's harder to do breadth first
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
	//printf("invalidate %p (%s)\n",this,typeid(this).name());
	clientArea = rect().setStart({1,1}).setEnd(area.size-vec2{1,1});//rect(newarea.size);
}

void GUIbase::invalidateTree(){
	if(suppressInvalidate){return;}
	//set invalidateSupress and invalidate children (inv downwards)
	//btw, when an external change is done to an element, it probably supresses invalidations and invalidates upwards and downwards...
	//or invalidates from top.
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

//in the future, this could be used to classify mouse
//as "over border/title/other parts"
void GUIbase::recalcMouseover(){
	vec2 pos = getMousePos();
	//not mouseover if mouse is somewhere else entirely
	if(!visibleArea().contains(pos)){mouseover = false; return;}
	if(parent){
		//not mouseover if parent doesn't have mouseover
		if(!parent->mouseover){mouseover = false; return;}
		//not mouseover if any of higher siblings have mouseover
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
	//printf("(%s).checkCloseTree()\n",getType().c_str());
	vector<GUIbase*> children2 = children; //probably expensive to do it every frame, (or not - the vector is only like 5 entries big)
											//it's better to update the destructor to not mess with the parent
	//then the for-loop will need I = vector.erase thingy and such.
	for(auto I = children2.begin(); I != children2.end(); I++){
		GUIbase *el = *I;
		if(el->deletePending){delete el;}
		else{el->checkCloseTree();}
	}
}

//if something has coordinates L within
//the Area of this element,
//it was coordinates W in the world.
vec2 GUIbase::thisToWorld(vec2 L){
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

rect GUIbase::thisToWorld(rect L){
	return L.setStart(thisToWorld(L.start)).setEnd(thisToWorld(L.end));
}

//if something has coordinates L within
//the clientArea of this element,
//it was coordinates W in the world.
vec2 GUIbase::clientToWorld(vec2 L){
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

rect GUIbase::clientToWorld(rect L){
	return L.setStart(clientToWorld(L.start)).setEnd(clientToWorld(L.end));
}

rect GUIbase::worldArea(){
	if(parent){
		return thisToWorld(rect(area.size));
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::worldClientArea(){
	if(parent){
		return thisToWorld(clientArea);
	}else{
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::visibleArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldArea());
		}else{
			return parent->visibleArea().clamp(worldArea());
		}
	}else{
		return worldArea();
	}
}
rect GUIbase::visibleClientArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldClientArea());
		}else{
			return parent->visibleArea().clamp(worldClientArea());
		}
	}else{
		return worldClientArea();
	}
}

GUI_border_rects GUIbase::getBorders(GUI_border_size bsize){
	/*
	AA----------AB------------------------------BB----------BA
	|  corner	| 			  top				|  corner    |
	|  top-left AC------------------------------BC top-right |
	|			|								|			 |
	AD---AE----AF								BF-----BE---BD
	|    |											   |     |
	|	 |                                             |     |
	|left|                                             |right|
	|    |                                             |     |
	|    |                                             |     |
	CD---CE----CF								DF-----DE---DD
	|			|								|            |
	| corner   CC-------------------------------DC corner    |
	| btm-left  |            bottom             |  btm-right |
	CA---------CB-------------------------------DB----------DA
	*/

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
	
/*	//------------------- safety measure----
	printf("Rtop = %s, Rbtm = %s, Rleft = %s, Rright = %s\n", 
		toCString(border.Rtop), 
		toCString(border.Rbottom), 
		toCString(border.Rleft), 
		toCString(border.Rright));
	printf("Rctl = %s, Rctr = %s, Rcbl = %s, Rcbr = %s\n",
		toCString(border.Rctl),
		toCString(border.Rctr),
		toCString(border.Rcbl), 
		toCString(border.Rcbr));
	//top half
	assert(AA.x < AB.x); assert(AB.x < BB.x); assert(BB.x < BA.x);
	assert(AB.y < AC.y); assert(BB.y < BC.y);
	assert(AC.x < BC.x);
	assert(AC.y < AD.y); assert(AC.y < AE.y); assert(AC.y < AF.y);
	assert(BC.y < BF.y); assert(BC.y < BE.y); assert(BC.y < BD.y);
	assert(AD.x < AE.x); assert(AE.x < AF.x); assert(AF.x < BF.x); assert(BF.x < BE.x); assert(BE.x < BD.x);
	//bottom half
	assert(AD.y < CD.y); assert(AE.y < CE.y); assert(AF.y < CF.y); assert(BF.y < DF.y); assert(BE.y < DE.y); assert(BD.y < DD.y);
	assert(CD.x < CE.x); assert(CE.x < CF.x); assert(CF.x < DF.x); assert(DF.x < DE.x); assert(DE.x < DD.x);
	assert(CF.y < CC.y); assert(DF.y < DC.y);
	assert(CC.x < DC.x);
	assert(CC.y < CA.y); assert(CC.y < CB.y); assert(DC.y < DB.y); assert(DC.y < DA.y);
	assert(CA.x < CB.x); assert(CB.x < DB.x); assert(DB.x < DA.x);
*/	//---------------------------------------
	
	
	return border;
}

GUIe_border GUIbase::testBorders(vec2 pos, GUI_border_size borderSize){
	
	GUI_border_rects border = getBorders(borderSize);
	
	//int condition = 0;
	GUIe_border condition = GUIb::None;
	//printf("border condition:\n");
	if(border.Rctl.contains(pos))	{/*printf("TL|");*/condition |= GUIb::Corner_TL;}
	if(border.Rtop.contains(pos))	{/*printf("T|");*/condition |= GUIb::Top;}
	if(border.Rctr.contains(pos))	{/*printf("TR|");*/condition |= GUIb::Corner_TR;}
	if(border.Rleft.contains(pos))	{/*printf("L|");*/condition |= GUIb::Left;}
	if(border.Rright.contains(pos))	{/*printf("R|");*/condition |= GUIb::Right;}
	if(border.Rcbl.contains(pos))	{/*printf("BL|");*/condition |= GUIb::Corner_BL;}
	if(border.Rbottom.contains(pos)){/*printf("B|");*/condition |= GUIb::Bottom;}
	if(border.Rcbr.contains(pos))	{/*printf("BR|");*/condition |= GUIb::Corner_BR;}
	
	return condition;//static_cast<GUI_border_type>(condition);
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
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

		 if(key == "name")		{name		= val;}
	else if(key == "type")		{if(getType() != val){error("attmpt to change GUI widget type\n");}}
	else if(key == "isClient")	{isClient 	= fromString<bool>(val);}
	else if(key == "hidden")	{hidden 	= fromString<bool>(val);}
	else if(key == "area")		{area		= fromString<rect>(val);}
}

string GUIbase::getType(){return "GUIbase";}

GUIcompoundProperty GUIbase::getCompoundProperty(){
	//GUIcompoundProperty *prop = new GUIcompoundProperty();
	GUIcompoundProperty prop;
	prop.name = getType();
	prop.table = getPropertyTable();
	for(auto I = children.begin(); I != children.end(); I++){
		GUIbase *C = *I;
		//GUIcompoundProperty *prop2 = C->getCompoundProperty();
		//prop->children.push_back(prop2);
		prop.children.push_back(C->getCompoundProperty());
	}
	return prop;
}

//note: non-client widgets (parts) should
//be already constructed by the owner before they get properties assigned.
void GUIbase::setCompoundProperty(const GUIcompoundProperty prop){
	//printf("compound set of (%s) on (%s)\n",prop->name.c_str(), getType().c_str());
	if(prop.name == getType()){
		//printf("setting prop table\n");
		setPropertyTable(prop.table);
	}
	auto Ic = children.begin(); 
	auto Ice = children.end();
	auto Ip = prop.children.begin(); 
	auto Ipe = prop.children.end();
	//printf("setting children tables (%d ch, %d tab)\n",children.size(), prop->children.size());
	for(;(Ic != Ice) && (Ip != Ipe);){
		GUIbase *C = *Ic;
		const GUIcompoundProperty &P = *Ip;
		//printf("compound name = (%s)\n",P->name.c_str());
		if(C->getType() == P.name){
			//printf("using set\n");
			C->setCompoundProperty(P);
		}else{
			//printf("using instant\n");
			GUIbase *C2 = P.instantiate();
			if(C2){Ic = addChild(C2,Ic);}//children.insert(Ic,C2);
		}
		Ic++; Ip++;
		//if(Ic == Ice){printf("ran out of children\n");}
		//if(Ip == Ipe){printf("ran out of tables\n");}
	}
	while(Ip != Ipe){
		const GUIcompoundProperty &P = *Ip;
		//printf("using instant2\n");
		GUIbase *C2 = P.instantiate();
		if(C2){Ic = addChild(C2,Ic);}
		Ip++;
	}
}






