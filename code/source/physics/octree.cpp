#include "octree.h"
#include "collision.h"
#include "globals.h"
#include "stringUtils.h"

octree_node::octree_node(){}

//gets the octant that the body is in
//-1 - out of this node
//0 - none (crosses one or more separating planes)
//1,2,3,4 - upper octants
//5,6,7,8 - lower octants
int octree_node::getOctant(AABB vaabb){
	bool xon,xop,yon,yop,zon,zop,xn,xp,yn,yp,zn,zp; //x outside positive/negative
	bool pxbs,pybs,pzbs,pxae,pyae,pzae; //plane x/y/z before start / after end
	pxbs = planes.x		< volume.start.x;
	pxae = planes.x 	> volume.end.x;
	pybs = planes.y		< volume.start.y;
	pyae = planes.y		> volume.end.y;
	pzbs = planes.z		< volume.start.z;
	pzae = planes.z		> volume.end.z;

	int res;
	xon = vaabb.start.x		< volume.start.x;
	xop = vaabb.end.x 		> volume.end.x;
	yon = vaabb.start.y		< volume.start.y;
	yop = vaabb.end.y		> volume.end.y;
	zon = vaabb.start.z		< volume.start.z;
	zop = vaabb.end.z		> volume.end.z;
	res = 0;

	if(xon || xop || yon || yop || zon || zop){res = -1; goto getoctExit;}	//gone to parent node
	xn = vaabb.start.x 		< planes.x;
	xp = vaabb.end.x 		> planes.x;
	yn = vaabb.start.y 		< planes.y;
	yp = vaabb.end.y 		> planes.y;
	zn = vaabb.start.z 		< planes.z;
	zp = vaabb.end.z 		> planes.z;

	if((xn && xp) || (yn && yp) || (zn && zp)){res = 0; goto getoctExit;}
	if(xn && yn && zn){res = 1; goto getoctExit;}
	if(xp && yn && zn){res = 2; goto getoctExit;}
	if(xn && yp && zn){res = 3; goto getoctExit;}
	if(xp && yp && zn){res = 4; goto getoctExit;}
	if(xn && yn && zp){res = 5; goto getoctExit;}
	if(xp && yn && zp){res = 6; goto getoctExit;}
	if(xn && yp && zp){res = 7; goto getoctExit;}
	if(xp && yp && zp){res = 8; goto getoctExit;}


	res = 0;
getoctExit:
	return res;
}

bool octree_node::canSplit(){
	return (volume.size.x > 1.f)
		&& (volume.size.y > 1.f)
		&& (volume.size.z > 1.f);
}

bool octree_node::canMerge(){
	if(isLeaf){return true;}
	for(int i = 0; i < 8; i++){
		octree_node *n = children[i];
		if(n->visitors.size()){return false;}
		if(!n->canMerge()){return false;}
	}
	return true;
}

void octree_node::merge(){
	if(isLeaf){
		return;
	}
	for(int I = 0; I < 8; I++){
		octree_node *N = children[I];
		N->merge();
		for(auto J = N->visitors.begin(); J != N->visitors.end();){
			visitors.push_back(*J);
			J = N->visitors.erase(J);
		}
		delete N;
		children[I] = 0;
	}
	isLeaf = true;
}

// view from bottom:
//
//  r g b
//  x y z
//
//  bottom:
//      |
//	 1  r  2
//      |
//	-g--+--g-
//	    |
//	 3  r  4
//      |
//  top:
//      |
//	 5  r  6
//      |
//	-g--+--g-
//	    |
//	 7  r  8
//      |
//


void octree_node::split(){
	isLeaf = false;
	calcPlanes();
	for(int I = 0; I < 8; I++){
		octree_node *N = new octree_node();
		N->parent = this;
		N->isLeaf = true;
		N->curoctant = I;
		children[I] = N;
	}
	float x0 = volume.start.x;
	float x1 = planes.x;
	float x2 = volume.end.x;

	float y0 = volume.start.y;
	float y1 = planes.y;
	float y2 = volume.end.y;

	float z0 = volume.start.z;
	float z1 = planes.z;
	float z2 = volume.end.z;
	vec3 vs; vec3 ve;
	vs = vec3(x0,y0,z0); ve = vec3(x1,y1,z1);
	children[0]->volume = AABB(vs,ve);
	children[0]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x1,y0,z0); ve = vec3(x2,y1,z1);
	children[1]->volume = AABB(vs,ve);
	children[1]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x0,y1,z0);ve = vec3(x1,y2,z1);
	children[2]->volume = AABB(vs,ve);
	children[2]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x1,y1,z0); ve = vec3(x2,y2,z1);
	children[3]->volume = AABB(vs,ve);
	children[3]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x0,y0,z1); ve = vec3(x1,y1,z2);
	children[4]->volume = AABB(vs,ve);
	children[4]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x1,y0,z1); ve = vec3(x2,y1,z2);
	children[5]->volume = AABB(vs,ve);
	children[5]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x0,y1,z1); ve = vec3(x1,y2,z2);
	children[6]->volume = AABB(vs,ve);
	children[6]->planes = vs + (ve-vs)/2.f;

	vs = vec3(x1,y1,z1); ve = vec3(x2,y2,z2);
	children[7]->volume = AABB(vs,ve);
	children[7]->planes = vs + (ve-vs)/2.f;
}


void octree_node::calcPlanes(){
	planes = (volume.start+volume.end)/2.f;
}

void octree_node::addVisitor(octree_visitor *ov){
	visitors.push_back(ov);
}
void octree_node::removeVisitor(octree_visitor *ov){
	for(auto I = visitors.begin(); I != visitors.end();){
		if(*I == ov){
			I = visitors.erase(I);
		}else{I++;}
	}
}

string octree_node::getName(){
	string selfname = itoa(curoctant);
	string pname = "";
	if(parent){pname = parent->getName() + ".";}
	return pname+selfname;
}

string toString(octree_node *N){
	string S = "";
	S += fstring("node %p\n",N);
	S += fstring("parent = %p\n",N->parent);
	S += fstring("curoctant = %d\n",N->curoctant);
	S += fstring("isLeaf = %d\n",N->isLeaf);
	S += fstring("children = %p %p %p %p | %p %p %p %p\n",
		N->children[0],
		N->children[1],
		N->children[2],
		N->children[3],
		N->children[4],
		N->children[5],
		N->children[6],
		N->children[7]);
	S += fstring("planes = %s\n",toCString(N->planes));
	S += fstring("volume = %s\n",toCString(N->volume));
	S += fstring("infinite = %d\n",N->infinite);
	S += fstring("visitors.size = %d\n",N->visitors.size());
	return S;
}

////-----------------------------

octree_visitor::octree_visitor(octree_node *node, collisionbody *body, vec3 pos){
	this->curNode = node;
	this->body = body;
	this->pos = pos;
	node->addVisitor(this);
	moveTo(pos);
}

octree_visitor::~octree_visitor(){
	if(curNode){curNode->removeVisitor(this);}
}

void octree_visitor::moveTo(vec3 pos){
	if(!curNode){error("no curnode\n");}
	if(!body){error("no body\n");}
	this->pos = pos;

	AABB aabb = body->getAABB().moveBy(pos);
	int octant = curNode->getOctant(aabb);
	bool insertNeeded = false;
	bool removeNeeded = false;
	if(octant == -1){
		if(curNode->parent){
			curNode->removeVisitor(this);
			insertNeeded = true;
			while((octant == -1) && curNode->parent){
				if(curNode->canMerge()){
					curNode->merge();
				}
				curNode = curNode->parent;
				if(curNode->canMerge()){
					curNode->merge();
				}
				octant = curNode->getOctant(aabb);
			}
		}
	}
	if(octant != 0){
		removeNeeded = true;
	}
	while((octant != 0) && (octant != -1)){
		if(curNode->isLeaf && curNode->canSplit()){
			curNode->split();
		}
		if(!curNode->isLeaf){
			if(removeNeeded){ //spaghetti code
				removeNeeded = false;
				curNode->removeVisitor(this);
				insertNeeded = true;
			}
			curNode = curNode->children[octant-1];
			octant = curNode->getOctant(aabb);
		}else{
			break;
		}
	}
	if(insertNeeded){
		curNode->addVisitor(this);
	}
}
#include "paint.h"

void octreeRender(octree_node *N){
	if(!N){return;}
	setColor(vec3(0,255,0));
	setColoring(false);
	setTexturing(false);
	setLighting(false);
	vec3 rot = vec3(0,0,0);
	drawBoxWireframe(N->volume);
	vec3 vmin = N->volume.start;
	vec3 vmax = N->volume.end;
	vec3 planes = N->planes;
	vec3 newstart;
	vec3 newend;
	vec3 center = vmin+(vmax-vmin)/2.f;
	if(N->visitors.size()){
		setColor(vec3(255,0,0));
		drawBoxWireframe(N->volume.setSize(N->volume.size*0.99f));
	}
	bool renderPlanes = false;
	if(renderPlanes){
		setAlpha(128);
		setTransparency(true);
		float n = 5;
		setColor(vec3(128,0,0));
		newstart	= vec3(planes.x,vmin.y,vmin.z);
		newend		= vec3(planes.x,vmax.y,vmax.z);
		vec3 pos = (newstart+newend)/2.f;
		vec3 scale = newend-newstart;
		drawBox(pos,rot,scale);

		//y-plane
		setColor(vec3(0,128,0));
		newstart	= vec3(vmin.x,planes.y,vmin.z);
		newend		= vec3(vmax.x,planes.y,vmax.z);
		pos = (newstart+newend)/2.f;
		scale = newend-newstart;
		drawBox(pos,rot,scale);

		//z-plane
		setColor(vec3(0,0,128));
		newstart	= vec3(vmin.x,vmin.y,planes.z);
		newend		= vec3(vmax.x,vmax.y,planes.z);
		pos = (newstart+newend)/2.f;
		scale = newend-newstart;
		drawBox(pos,rot,scale);

		setTransparency(false);
		setColor(vec3(0,0,0));
	}
	if(!N->isLeaf){
		for(int I = 0; I < 8; I++){
			octreeRender(N->children[I]);
		}
	}
}

#include "GUI.h"
extern GUIbase* g_GUI;
#include "stringUtils.h"

void octreePrint(octree_node *N){
	static GUIbase* owin = 0;
	static GUIlabel* lbl = 0;
	if(!owin){
		owin = new GUIwindow();
		owin->setSize(vec2(200,300));
		dynamic_cast<GUIwindow*>(owin)->setTitle(fstring("octree_node %p",N));

		lbl = new GUIlabel();
		owin->addChild(lbl);
		g_GUI->addChild(owin);
	}
	string text;
	if(!N){
		text += "(null)";
	}else{
		text += fstring("AABB: %s\n",toCString(N->volume));
		text += fstring("visitors: %d\n",N->visitors.size());
	}

	lbl->setText(text);
	lbl->sizeToContents();
	owin->sizeToContents();
	owin->invalidate();
}


