#ifndef NARROWPHASE_GUARD
#define NARROWPHASE_GUARD
#include "collision.h"

collisioninfo *checkCollisionAABB_AABB(collisionbodyAABB *body1, collisionbodyAABB *body2);
collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB);
collisioninfo *collisionCheckDispatch(collisionbody *body1, collisionbody *body2);
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2, collisionOptions &options);

#endif
