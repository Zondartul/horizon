#ifndef BROADPHASE_GUARD
#define BROADPHASE_GUARD
#include <utility>
using std::pair;

bool canCollide(collisionbody *body1, collisionbody *body2);

typedef pair<collisionbody*,collisionbody*> collisionpair;

struct broadphaseinfo{
	vector<collisionbody*> bodies;
	vector<collisionpair> pairs;
	void add(broadphaseinfo *bp2[8]);	   //add the entities and collisions from another bp
	void addJoin(broadphaseinfo *bp2[8]); //add and make pairs between the two sets
};

broadphaseinfo *getBroadphaseNodeOnly(octree_node *node);
broadphaseinfo *checkCollisionBroadphase(octree_node *node);
void broadphaseRender(broadphaseinfo *bp);

#endif