#ifndef RESOLUTION_GUARD
#define RESOLUTION_GUARD

//void resolve1Dcollision(float vel1, float mass1, float vel2, float mass2, float *vel1_result, float *vel2_result,float restitution);
float frictionFormula(float f1, float f2);
float restitutionFormula(float r1, float r2);
void resolveCollision(collisioninfo *col);
void separateCollision(collisioninfo *col);


#endif