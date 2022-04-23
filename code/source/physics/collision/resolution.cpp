#include "collision.h"
#include "resolution.h"
#include "simplemath.h"
#include "vec.h"
#include "glm/gtx/projection.hpp"
#include "globals.h"
#include "stringUtils.h"
#include "geometry.h"


//for debug drawing
#include "paint.h"

//compute friction for a pair of bodies.
float frictionFormula(float f1, float f2){
	return f1*f2;
}

//compute restitution for a pair of bodies
float restitutionFormula(float r1, float r2){
	return r1*r2;
}

//static-static collisions are not handled right now
void resolveStaticStatic(collisioninfo *col){
	printCollision(col);
	printf("Resolving Static-Static collision\n");
}

bool bodiesSeparating(collisioninfo *col){
	vec3 n = col->c_to_c.normal; //normal of body2's surface penetrated by body1
	vec3 dv = col->body1->vel - col->body2->vel; //dv: velocity of body1 relative to body2 
	bool sep = (dot(dv,n) > 0);
	vec3 dp = col->body2->pos - col->body1->pos;
	return sep;
}


//resolve collision by putting a force on the dynbody
void resolveDynamicStatic(collisioninfo *col){
	bool separating = bodiesSeparating(col);
	
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	
	vec3 v1 = body1->vel;
	float m1 = body1->mass;
	float r1 = body1->restitution;
	float f1 = body1->friction;
	vec3 v2 = body2->vel;
	
	
	float m2 = body2->mass;
	float r2 = body2->restitution;
	float f2 = body2->friction;
	float f = frictionFormula(f1,f2);
	float r = restitutionFormula(r1,r2);
	vec3 dv = v1-v2;
	
	vec3 n = col->c_to_c.normal;			//normal direction
	vec3 p = normalizeSafe(dv - project(dv,n));
	setLayer(g_layerDebug);
	drawArrow(body1->pos, body1->pos+p, vec3(128,0,128)); //purple arrow = velocity parralel to collision
	float dvn = glm::dot(dv,n); 	//delta-v normal
 	float dvp = glm::dot(dv,p);		//delta-v parralel
	
	float v1n = glm::dot(v1,n);
	float v1p = glm::dot(v1,p);
	
	float v2n = glm::dot(v2,n);
	float v2p = glm::dot(v2,p);
	vec3 v1f = v1;
	vec3 v2f = v2;
	//body1 is static, body2 is dynamic
	//todo: normal rigid-bounce, normal spring-bounce?
	
	//normal velocity after collision:
	//when restitution = 1 (fully elastic collision) final dv = - initial dv (final v1 = 2*v2-v1)
	//when restitution = 0 (fully inelastic collision) final dv = 0			 (final v1 = v2)
	float v1nf = v2n+r*(v2n-v1n);	//print(v2nf);	//final normal velocity
	
	if(separating){v1nf = v1n;}
	float nF = (v1nf-v1n)/m1;			//normal force
	float fA = min(nF*f/m1,dvp);		//friction acceleration
	float v1pf = v1p-fA; 				//final parralel velocity

	
	v1f = n*v1nf + p*v1pf;			//final v = v.normal + v.parralel

	body1->vel = v1f;
}

void resolveDynamicDynamic(collisioninfo *col){
	bool separating = bodiesSeparating(col);
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	
	vec3 v1 = body1->vel;
	float m1 = body1->mass;
	float r1 = body1->restitution;
	float f1 = body1->friction;
	vec3 v2 = body2->vel;
	float m2 = body2->mass;
	float r2 = body2->restitution;
	float f2 = body2->friction;
	float f = frictionFormula(f1,f2);
	float r = restitutionFormula(r1,r2);
	vec3 dv = v2-v1;
	vec3 n = col->c_to_c.normal;	//normal direction
	vec3 p = normalizeSafe(dv - project(dv,n));//parralel direction
	float dvn = glm::dot(dv,n); 	//delta-v normal
 	float dvp = glm::dot(dv,p);		//delta-v parralel
	
	float v1n = glm::dot(v1,n);	
	float v1p = glm::dot(v1,p);
	
	float v2n = glm::dot(v2,n);
	float v2p = glm::dot(v2,p);
	
	vec3 v1f = v1;
	vec3 v2f = v2;
	//both bodies dynamic
	float v1nf = (m1*v1n+m2*v2n-r*m2*(v1n-v2n))/(m1+m2);	//final normal velocity
	float v2nf = (m1*v1n+m2*v2n+r*m1*(v1n-v2n))/(m1+m2);	//final normal velocity
	if(separating){
		v1nf = v1n;
		v2nf = v2n;
	}
	float nF = ((v2nf-v2n)+(v1nf-v1n))/m2;			//normal force
	
	
	float sF = nF*f;
	float fA1 = 0, fA2 = 0; //friction acceleration
	if((sF/m1 > dvp/2.f) || (sF/m2 > dvp/2.f)){
		//static friction
		fA1 = dvp/2.f;
		fA2 = dvp/2.f;
	}else{
		//dynamic friction
		fA1 = sF/m1;
		fA2 = sF/m2;
	}
	
	float v1pf = v1p-fA1;			//final parralel velocity
	float v2pf = v2p-fA2; 			//final parralel velocity
	
	v1f = n*v1nf + p*v1pf;
	v2f = n*v2nf + p*v2pf;			//final v = v.normal + v.parralel
	
	body1->vel = v1f;
	body2->vel = v2f;
}

void separateCollision(collisioninfo *col){
	if(bodiesSeparating(col)){return;}
	float bias = 1.01f; //extra separation
	bool velocitySeparation = false;//true;
	float tcrit = 1.1f; //maximum duration penetration to compensate with using velocity
							//t > 1 tick -> things go flying into space
							//t = 0 	 -> old no-velocity behavior (things fall through floor)
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	bool d1 = (col->body1->type == BODY_DYNAMIC);
	bool d2 = (col->body2->type == BODY_DYNAMIC);
	vec3 v1,v2,dv,pen,v1n,v2n;
	v1 = body1->vel;
	v2 = body2->vel;
	pen = col->c_to_c.penetration;
	float dvpen,lpen,t;
	if(velocitySeparation){
		//experimental velocity-based separation.
		//assuming constant velocity, calculate where the bodies were
		//when the distance between them was 0+bias
	//since penetration isn't always against normal (it can be sideways)
	//we need to calculate the normal-penetration
		v1 = body1->vel;
		v2 = body2->vel;
		dv = v1-v2;
		
		pen = col->c_to_c.penetration;
		vec3 norm = col->c_to_c.normal;
		vec3 normpen = glm::proj(pen,norm);
		dvpen = dot(dv,normalizeSafe(normpen));
		v1n = glm::proj(v1,normpen);
		v2n = glm::proj(v2,normpen);
		
		float lnormpen = length(normpen);
		t = lnormpen / dvpen;
		if(t > tcrit){
			velocitySeparation = false;
		}
		if(dot(v1,v2) < 0){
			//bodies are already separating
			velocitySeparation = false;
		}
	}
	
	if(velocitySeparation){
		//preserve parralel speed
		if(!d1 && !d2){d1 = true; d2 = true;}
		if(d1){body1->pos -= v1n*t*bias;}
		if(d2){body2->pos -= v2n*t*bias;}
	}else{
		//stability failsafe: just un-penetrate
		if(d1 == d2){bias = bias / 2.f;} //if both bodies are to be separated, each gets a half of the separation
		if(d1){body1->pos -= pen*bias;}
		if(d2){body2->pos += pen*bias;}
	}
	//penetration vector should point from body1 to body2
		
	//GREEN ARROW: normal of each body
	//BLUE ARROW: penetration vector of each body
	//AQUA ARROW: relative velocity
	setLayer(g_layerDebug);
	drawPoint(col->c_to_c.pos,vec3(0,0,255));
	vec3 O1 = body1->pos;//col->c_to_c.pos;
	vec3 O2 = body2->pos;
	vec3 C = col->c_to_c.pos;
	vec3 N = col->c_to_c.normal;
	vec3 P = col->c_to_c.penetration;
	vec3 D = dv;
	drawArrow(O1,O1+N,vec3(0,255,0));
	drawArrow(O1,O1+P,vec3(0,0,255));
	drawArrow(O1,O1+D,vec3(0,255,255));
	for(int I = 0; I < col->cpts.size(); I++){
		collisionpoint cp = col->cpts[I];
		drawPoint(cp.pos,vec3(128,0,128));
	}
}
//for e->name
#include "entity/entity.h"

void resolveCollision(collisioninfo *col){
	if(!col){error("no collision info\n");}
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	if(!body1){error("no body1\n");}
	if(!body2){error("no body2\n");}
	
	if(body1->type == BODY_DYNAMIC && body2->type == BODY_DYNAMIC){
		resolveDynamicDynamic(col);
	}else if(body1->type == BODY_DYNAMIC){
		resolveDynamicStatic(col);
	}else if(body2->type == BODY_DYNAMIC){
		resolveDynamicStatic(reverseCollision(col));
	}else{
		resolveStaticStatic(col);
	}
}
