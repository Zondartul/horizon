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
	//vec3 dp = body2->pos - body1->pos; //dp: vector from body1 to body2
	vec3 n = col->c_to_c.normal; //normal of body2's surface penetrated by body1
	vec3 dv = col->body1->vel - col->body2->vel; //dv: velocity of body1 relative to body2 
	bool sep = (dot(dv,n) > 0);
	vec3 dp = col->body2->pos - col->body1->pos;
	//printf("bS: body1 = %s / %s, body2 = %s / %s\n", 
	//		col->body1->bodyname.c_str(), col->body1->type == BODY_DYNAMIC? "DYNAMIC" : "STATIC",
	//		col->body2->bodyname.c_str(), col->body2->type == BODY_DYNAMIC? "DYNAMIC" : "STATIC");
	//printf("bS: n = %s, dv = %s, dp = %f, sep = %d\n",toCString(n), toCString(dv), length(dp), (int)sep);
	return sep;
}


//resolve collision by putting a force on the dynbody
void resolveDynamicStatic(collisioninfo *col){
	//printCollision(col);
	//printf("Resolving Static-Dynamic collision\n");
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
	//printf("f = %f, r = %f\n",f,r);
	vec3 dv = v1-v2;
	//printf("len(v2) = %f, len(v1) = %f, len(v2-v1) = %f\n",length(v2),length(v1),length(dv));
	
	vec3 n = col->c_to_c.normal;			//normal direction
	//p is dv - projection(dv on n)
	vec3 p = normalizeSafe(dv - project(dv,n));//normalizeSafe(normalizeSafe(dv) - n);//parralel direction
	//printf("parralel vel = %s, len = %f\n", toCString(p), length(p));
	setLayer(layerDebug);
	drawArrow(body1->pos, body1->pos+p, vec3(128,0,128)); //purple arrow = velocity parralel to collision
	float dvn = glm::dot(dv,n); 	//delta-v normal
 	float dvp = glm::dot(dv,p);		//delta-v parralel
	
	float v1n = glm::dot(v1,n);	//print(v1n);
	float v1p = glm::dot(v1,p);	//print(v1p);
	
	float v2n = glm::dot(v2,n);	//print(v2n);
	float v2p = glm::dot(v2,p);	//print(v2p);
	//printf("v1n = %f, v1p = %f\n",v1n,v1p);
	vec3 v1f = v1;
	vec3 v2f = v2;
	//body1 is static, body2 is dynamic
	//todo: normal rigid-bounce, normal spring-bounce?
	
	//normal velocity after collision:
	//when restitution = 1 (fully elastic collision) final dv = - initial dv (final v1 = 2*v2-v1)
	//when restitution = 0 (fully inelastic collision) final dv = 0			 (final v1 = v2)
	float v1nf = v2n+r*(v2n-v1n);	//print(v2nf);	//final normal velocity
	
	if(separating){v1nf = v1n;}
	float nF = (v1nf-v1n)/m1;		//print(nF);		//normal force
	float fA = min(nF*f/m1,dvp);	//print(fA);		//friction acceleration
	float v1pf = v1p-fA; 			//print(v2pf);	//final parralel velocity

	
	v1f = n*v1nf + p*v1pf;			//final v = v.normal + v.parralel
	//if(!separating){printf("resolve: v1nf = %f, nF = %f, fA = %f, v1pf = %f, n1f = %f\n",v1nf, nF, fA, v1pf, v1f);}
	//else{printf("resolve: sep\n");}
	//printf("v2f = %s\n",toCString(v2f));
	
	body1->vel = v1f;
	//printf("final b2->vel = %s\n", toCString(body2->vel));
	//printf("----------------------\n");
}

void resolveDynamicDynamic(collisioninfo *col){
	
	bool separating = bodiesSeparating(col);
	//printCollision(col);
	//printf("Resolving Dynamic-Dynamic collision\n");
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
	vec3 n = col->c_to_c.normal;			//normal direction
	vec3 p = normalizeSafe(dv - project(dv,n));//normalizeSafe(normalizeSafe(dv) - n);//parralel direction
	float dvn = glm::dot(dv,n); 	//delta-v normal
 	float dvp = glm::dot(dv,p);		//delta-v parralel
	
	float v1n = glm::dot(v1,n);	//print(v1n);
	float v1p = glm::dot(v1,p);	//print(v1p);
	
	float v2n = glm::dot(v2,n);	//print(v2n);
	float v2p = glm::dot(v2,p);	//print(v2p);
	
	vec3 v1f = v1;
	vec3 v2f = v2;
	//both bodies dynamic
	float v1nf = (m1*v1n+m2*v2n-r*m2*(v1n-v2n))/(m1+m2);	//print(v1nf);	//final normal velocity
	float v2nf = (m1*v1n+m2*v2n+r*m1*(v1n-v2n))/(m1+m2);	//print(v2nf);	//final normal velocity
	if(separating){
		v1nf = v1n;
		v2nf = v2n;
	}
	float nF = ((v2nf-v2n)+(v1nf-v1n))/m2;					//print(nF);		//normal force
	
	
	float sF = nF*f;
	float fA1 = 0, fA2 = 0;
	if((sF/m1 > dvp/2.f) || (sF/m2 > dvp/2.f)){
		//static friction
		fA1 = dvp/2.f;
		fA2 = dvp/2.f;
	}else{
		//dynamic friction
		fA1 = sF/m1;
		fA2 = sF/m2;
	}
	//print(fA1);		//friction acceleration
	//print(fA2);		//friction acceleration
	
	float v1pf = v1p-fA1;			//print(v1pf);	//final parralel velocity
	float v2pf = v2p-fA2; 			//print(v2pf);	//final parralel velocity
	
	v1f = n*v1nf + p*v1pf;
	v2f = n*v2nf + p*v2pf;			//final v = v.normal + v.parralel
	//printf("v2f = %s\n",toCString(v2f));
	
	
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
	/*
		v1 = body1->vel; //idea: use change from prev. position instead?
		v2 = body2->vel;
		dv = v1-v2;					//relative velocity
		pen = col->c_to_c.penetration;
		dvpen = dot(dv,normalize(pen));	//rel.vel. in the penetration direction
		v1n = glm::proj(v1,pen);
		v2n = glm::proj(v2,pen);
		lpen = length(pen);				//penetration distance
		t = lpen / dvpen;					//time the penetration took
		if(t > tcrit){
			//time too long - proposed separation would cause instability
			//(i.e. things flying off into space)
			velocitySeparation = false;
		}
	*/
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
	
	/*
	if(velocitySeparation){
		//we preserve parralel speed
		if(d1 && d2){//dynamic-dynamic
			body1->pos -= v1n*t*bias;	//time travel!
			body2->pos -= v2n*t*bias;
		}else if(d1){//dynamic-static
			body1->pos -= dv*t*bias;
		}else if(d2){//static-dynamic
			body2->pos += dv*t*bias;
		}else{//static-static
			body1->pos -= pen*bias;
			body2->pos += pen*bias;
		}
	}else{
		if(d1){body1->pos -= pen*bias;}
		if(d2){body2->pos += pen*bias;}
	}
	*/
	if(velocitySeparation){
		//preserve parralel speed
		//printf("<separate_vel>\n");
		if(!d1 && !d2){d1 = true; d2 = true;}
		if(d1){body1->pos -= v1n*t*bias;}
		if(d2){body2->pos -= v2n*t*bias;}
	}else{
		//stability failsafe: just un-penetrate
		//printf("<separate_unpen (pen = %f), d1 = %d, d2 = %d>\n",length(pen),(int)d1,(int)d2);
		//printf("<pen = %s, towards %s>\n", toCString(pen),(dot(pen, (body1->pos - body2->pos)) > 0)? "BODY_1" : "BODY_2");
		if(d1 == d2){bias = bias / 2.f;} //if both bodies are to be separated, each gets a half of the separation
		if(d1){body1->pos -= pen*bias;}
		if(d2){body2->pos += pen*bias;}
	}
	//penetration vector should point from body1 to body2
		
	//GREEN ARROW: normal of each body
	//BLUE ARROW: penetration vector of each body
	//AQUA ARROW: relative velocity
	setLayer(layerDebug);
	drawPoint(col->c_to_c.pos,vec3(0,0,255));
	vec3 O1 = body1->pos;//col->c_to_c.pos;
	vec3 O2 = body2->pos;
	vec3 C = col->c_to_c.pos;
	vec3 N = col->c_to_c.normal;
	vec3 P = col->c_to_c.penetration;
	vec3 D = dv;
	//setLineWidth(2.f);
	drawArrow(O1,O1+N,vec3(0,255,0));
	drawArrow(O1,O1+P,vec3(0,0,255));
	drawArrow(O1,O1+D,vec3(0,255,255));
	//setLineWidth(1.f);
	//drawArrow(O2,O2+N,vec3(0,255,0));
	//drawArrow(O2,O2+P,vec3(0,0,255));
	//drawArrow(O2,O2-D,vec3(0,255,255));
	//drawPoint(C,vec3(255,0,255));
	for(int I = 0; I < col->cpts.size(); I++){
		collisionpoint cp = col->cpts[I];
		drawPoint(cp.pos,vec3(128,0,128));
	}
	//printf("[res drawarrow] O = %s, N = %s\n",toCString(O),toCString(N));
	
	//printf("(bodies separated)\n");
}
//for e->name
#include "entity/entity.h"

void resolveCollision(collisioninfo *col){
	if(!col){error("no collision info\n");}
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	if(!body1){error("no body1\n");}
	if(!body2){error("no body2\n");}
	//string name1 = "<n/e>";if(col->body1->E){name1 = col->body1->E->name;}
	//string name2 = "<n/e>";if(col->body2->E){name2 = col->body2->E->name;}
	//printf("collision: %s vs %s\n",name1.c_str(), name2.c_str());
	
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

//olde code:
/* 
	if(mass1 and mass2){printf("collision: m1 = %f, m2 = %f, friction = %f, rest = %f\n",mass1,mass2,friction,restitution);}
	//we treat mass = 0 as infinite mass
	vec3 n = col->c_to_c.normal;
	if(length(n) == 0){
		printCollision(col->body1,col->body2);
		printf("no normal\n"); 
		return;
	}
	//vec3 v1 = vel1-vel2;//*vel1;
	if(length(vel1-vel2) == 0){
		printCollision(col->body1,col->body2);
		printf("no-v penetration\n");
		return;
	}
	//vec3 v2 = vec3(0,0,0);//*vel2;
	
	//float friction = 0.005f;
	
	//vec3 v1_perp_comp = glm::proj(v1,n);
	vec3 v1_perp_comp = glm::proj(vel1,n);
	//float v1_norm_length = dot(v1_perp_comp,n);
	//if(v1_norm_length > 0){return;}
	
	vec3 v1_par_comp = vel1-v1_perp_comp;
	//vec3 v2_perp_comp = -v1_perp_comp;
	vec3 v2_perp_comp = glm::proj(vel2,n);
	//float v2_norm_length = 0;
	//vec3 v2_par_comp = vec3(0,0,0);
	vec3 v2_par_comp = vel2-v2_perp_comp;
	
	vec3 dv_perp_comp = v1_perp_comp - v2_perp_comp;
	vec3 dv_par_comp = v1_par_comp - v2_par_comp;
	
	vec3 v1_par_friction = vec3(0,0,0);
	vec3 v2_par_friction = vec3(0,0,0);
	
	float v1_norm_length = length(v1_perp_comp);
	float v2_norm_length = -length(v2_perp_comp);
	float v1_perp_comp_result, v2_perp_comp_result;
	
	resolve1Dcollision(v1_norm_length,mass1,v2_norm_length,mass2,&v1_perp_comp_result,&v2_perp_comp_result,restitution);
	v2_perp_comp_result = -v2_perp_comp_result; //due to -> -> velocity convention
	float dv1abs = fabs(fabs(v1_perp_comp_result)-length(v1_perp_comp));
	float dv2abs = fabs(fabs(v2_perp_comp_result)-length(v2_perp_comp));
	float normalForce = dv1abs+dv2abs;
	normalForce = normalForce / (mass1+mass2);
	//0 mass is treated as inf mass later
	
	if(length(dv_par_comp)){
		vec3 v1_par_dir = normalize(v1_par_comp);
		vec3 v2_par_dir = -v1_par_dir;
		float v1_par_len = length(v1_par_comp);
		float v2_par_len = length(v2_par_comp);
		float dv_par_len = length(dv_par_comp);
		if(col->body1->type == BODY_DYNAMIC){
			//v1_par_friction = -v1_par_dir*min(friction/mass1,v1_par_len);
			v1_par_friction = (-v1_par_dir)*min(friction*normalForce,dv_par_len);
		}
		if(col->body2->type == BODY_DYNAMIC){
			v2_par_friction = (-v2_par_dir)*min(friction*normalForce,dv_par_len);
			//v2_par_friction = v1_par_dir*min(friction/mass2,v1_par_len);
		}
	}
	//vec3 v2_perp_comp = proj(v2,n);
	//float v2_norm_length = dot(v2_perp_comp,n);
	//vec3 v2_par_comp = v2-v2_perp_comp;
	
	
	
	vel1 = (v1_par_comp+v1_par_friction)+
		normalize(v1_perp_comp)*v1_perp_comp_result;
	vel2 = (v2_par_comp+v2_par_friction)+
		normalize(v2_perp_comp)*v2_perp_comp_result;
	
	
		printf("dv = %f, nF = %f, v1f = %f, v2f = %f\n",length(dv_par_comp),normalForce,vel1,vel2);
	
	if(col->body1->type == BODY_DYNAMIC){col->body1->vel = vel1;}
	if(col->body2->type == BODY_DYNAMIC){col->body2->vel = vel2;} */

/* 
void resolve1Dcollision(float vel1, float mass1, float vel2, float mass2, float *vel1_result, float *vel2_result,float restitution){
	float vel1_new,vel2_new;
	//float restitution = 0.0f;//0.5f;
	float m1 = mass1;
	float m2 = mass2;
	float v1 = vel1;
	float v2 = vel2;
	if((mass1 == 0) and (mass2 == 0)){printf("warning: resolving collisions, both objects massless\n"); return;}
	if(mass1 == 0){ //mass1 actually infinite
		//the limit of elastic equations for infinite mass:
		vel1_new = vel1;
		//vel2_new = -vel2+2*vel1;
		
		vel2_new = vel1+restitution*(vel1-vel2);
		vel1_new = v1;
		//vel2_new = 2*v1 - v2;
	}else if(mass2 == 0){
		//vel1_new = -vel1+2*vel2;
		
		vel1_new = vel2+restitution*(vel2-vel1);
		//vel1_new = 2*v2 - v1;
		vel2_new = vel2;
	}else{
		//vel1_new = (vel1*(mass1-mass2)+2*mass2*vel2)/(mass1+mass2);
		//vel2_new = (vel2*(mass2-mass1)+2*mass1*vel1)/(mass1+mass2);
		
		vel1_new = (mass1*vel1+mass2*vel2+restitution*mass2*(vel2-vel1))/(mass1+mass2);
		vel2_new = (mass1*vel1+mass2*vel2+restitution*mass1*(vel1-vel2))/(mass1+mass2);
		
		//vel1_new = ((m1-m2)/(m1+m2))*v1+(2*m2/(m1+m2))*v2;
		//vel2_new = (2*m1/(m1+m2))*v1 - ((m1-m2)/(m1+m2))*v2;
	}
	*vel1_result = vel1_new;
	*vel2_result = vel2_new;
} */