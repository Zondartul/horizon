#ifndef RESOLUTION_GUARD
#define RESOLUTION_GUARD

float frictionFormula(float f1, float f2);
float restitutionFormula(float r1, float r2);
void resolveCollision(collisioninfo *col);
void separateCollision(collisioninfo *col);


#endif