#include "collision.h"
#include "broadphase.h"
#include "paint.h"

#define print(x) printf(#x ": %f\n", x)
#define print2(x) printf(#x ": %s\n", toString(x).c_str())

void broadphaseinfo::add(broadphaseinfo *bp[8]){
	for(int octant = 0; octant < 8; octant++){
		broadphaseinfo *bp2 = bp[octant];
		if(!bp2){continue;}
		bodies.insert(bodies.end(),bp2->bodies.begin(),bp2->bodies.end());
		pairs.insert(pairs.end(),bp2->pairs.begin(),bp2->pairs.end());
	}
}

void broadphaseinfo::addJoin(broadphaseinfo *bp[8]){
	//pairs between old and new bodies
	for(int octant = 0; octant < 8; octant++){
		broadphaseinfo *bp2 = bp[octant];
		if(!bp2){continue;}
		for(auto I = bodies.begin(); I != bodies.end(); I++){
			for(auto J = bp2->bodies.begin(); J != bp2->bodies.end(); J++){
				if(!canCollide(*I,*J)){continue;}
				pairs.push_back({*I,*J});
			}
		}
	}
	//also concatenate the new bps
	add(bp);
}

bool canCollide(collisionbody *body1, collisionbody *body2){
	return !(	
		body1->type == BODY_NOCOLLIDE ||
		body2->type == BODY_NOCOLLIDE ||
		(body1->type == BODY_STATIC && body2->type == BODY_STATIC)
	);
	//NOCOLLIDE bodies are never checked
	//STATIC bodies are not checked against other static bodies
	//(since they are assumed not to move)
}

class rec_counter{
	public:
	int *val;
	rec_counter(int *newval){
		val = newval;
		(*val)++;
	}
	~rec_counter(){
		(*val)--;
	}
};

void indent(int num,char C){while(num--){printf("%c",C);}}

//int g_bprecs = 0;
broadphaseinfo *getBroadphaseNodeOnly(octree_node *node){
	broadphaseinfo *bp1 = new broadphaseinfo();
	if(!node){return bp1;}
	if(!node->visitors.size()){return bp1;}
	for(auto I = node->visitors.begin(); I != node->visitors.end(); I++){
		bp1->bodies.push_back((*I)->body);
	}
	//and make pairwise pairs of wise
	for(auto I = bp1->bodies.begin(); I != bp1->bodies.end(); I++){
		for(auto J = I+1; J != bp1->bodies.end(); J++){
			if(!canCollide(*I,*J)){continue;}
			//S-D, D-D, T-S, T-D collisions only
			bp1->pairs.push_back({*I,*J});
		}
	}
	return bp1;
}

broadphaseinfo *checkCollisionBroadphase(octree_node *node){
	if(!node){return 0;}
	rec_counter rc(&g_bprecs);
	//first, add bodies from this node
	string nname = node->getName();
	broadphaseinfo *bp1 = getBroadphaseNodeOnly(node);
	//then, add the bodies from sub-nodes
	if(!node->isLeaf){
		broadphaseinfo *bp2[8];
		for(int I = 0; I < 8; I++){
			bp2[I] = checkCollisionBroadphase(node->children[I]);
		}
		bp1->addJoin(bp2);
		for(int I = 0; I < 8; I++){delete bp2[I];}
	}
	return bp1;
}

void broadphaseRender(broadphaseinfo *bp){
	setColor(vec3(0,0,255));
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setPointSize(3.f);
	setColoring(false);
	setTexturing(false);
	setLighting(false);
	int j = 0;
	for(auto I = bp->bodies.begin(); I != bp->bodies.end(); I++){
		vec3 pos = (*I)->ov->pos;
		drawPoint(pos);
		j++;
	}
	for(auto I = bp->pairs.begin(); I != bp->pairs.end(); I++){
		drawLine((*I).first->ov->pos,(*I).second->ov->pos);
	}
}


