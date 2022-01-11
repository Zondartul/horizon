#ifndef OCTREE_GUARD
#define OCTREE_GUARD
#include "vec.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
class collisionbody;
struct octree_visitor;

//only stores internal divisions
class octree_node{
	public:
	octree_node *parent = 0;
	int curoctant = 0;	//which octant of the parent node is this one?
	bool isLeaf = 1;	//are there any children?
	octree_node *children[8] = {0,0,0,0,0,0,0,0};
	vec3 planes = vec3(0,0,0);		//corresponds to the positions of the three splitting planes
	AABB volume;
	bool infinite = 0;	//unbounded volume
	vector<octree_visitor*> visitors;
	octree_node();
	int getOctant(AABB vaabb);
	bool canSplit();
	bool canMerge();
	void split();
	void merge();
	void calcPlanes();	//calculates the plane positions based on volume
	void addVisitor(octree_visitor *ov);
	void removeVisitor(octree_visitor *ov);
	string getName();
};

string toString(octree_node*);

//a two-way link between an object and an octree node that contains it
struct octree_visitor{
	octree_node *curNode;
	collisionbody *body;
	vec3 pos;
	octree_visitor(octree_node *node, collisionbody *body, vec3 pos);
	~octree_visitor();
	void moveTo(vec3 pos);	//updates the octree according to the new position of the object
};

void octreeRender(octree_node *N);	//visualize the octree for debugging
void octreePrint(octree_node *N);					//print octree debug info

#endif