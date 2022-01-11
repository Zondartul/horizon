#include "collision.h"
#include "math.h"
#include "glm/gtx/projection.hpp"
#include "stdio.h"
#include "stringUtils.h"
#include "stdlib.h"
#include "simplemath.h"
#include "paint.h"
#include "globals.h"
#include "entity.h"
#include "octree.h"

extern octree_node *octree_root;

string toString(collisionbodytype type){
	switch(type){
		case BODY_STATIC: return "BODY_STATIC"; break;
		case BODY_DYNAMIC: return "BODY_DYNAMIC"; break;
		case BODY_TRIGGER: return "BODY_TRIGGER"; break;
		case BODY_NOCOLLIDE: return "BODY_NOCOLLIDE"; break;
	}
}

collisionbody::collisionbody(){
	ov = new octree_visitor(octree_root, this, pos);
	//ov->curNode = octree_root; //todo: make ov a RAII object
	//ov->body = this;
	//octree_root->addVisitor(ov);
	//ov->moveTo(pos);
}
collisionbody::~collisionbody(){
	delete ov;
}
collisionbodyAABB::collisionbodyAABB(AABB aabb){this->aabb = aabb;}
collisionbodyRay::collisionbodyRay(vec3 from, vec3 dir){this->from = from; this->dir = dir;}




#define setcandidate(x,n,z)	{		    \
	resolutions[x].pos = centerA;       \
	resolutions[x].normal = n;          \
	resolutions[x].penetration = n*z;   \
	resolutions[x].depth = z;		    \
}

#define print(x) printf(#x ": %f\n", x)
#define print2(x) printf(#x ": %s\n", toString(x).c_str())


// https://tavianator.com/fast-branchless-raybounding-box-intersections/
collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB){
	//collisionbodyRay *bodyRay = dynamic_cast<collisionbodyRay*>(cpRay.body);
	//collisionbodyAABB *bodyAABB = dynamic_cast<collisionbodyAABB*>(cpAABB.body);
	vec3 from = bodyRay->from;
	vec3 dir = bodyRay->dir;
	AABB aabb = bodyAABB->aabb.moveBy(bodyAABB->pos);
	
	float t1,t2,tmin,tmax;
	tmin = -1.f/0.f; //-inf
	tmax = 1.f/0.f;  //+inf
	if(dir.x != 0){
		float t1 = (aabb.start.x - from.x)/dir.x;
		float t2 = (aabb.end.x - from.x)/dir.x;
		tmin = max(tmin, min(t1,t2));
		tmax = min(tmax, max(t1,t2));
	}
	if(dir.y != 0){
		float t1 = (aabb.start.y - from.y)/dir.y;
		float t2 = (aabb.end.y - from.y)/dir.y;
		tmin = max(tmin, min(t1,t2));
		tmax = min(tmax, max(t1,t2));
	}
	if(dir.z != 0){
		float t1 = (aabb.start.z - from.z)/dir.z;
		float t2 = (aabb.end.z - from.z)/dir.z;
		tmin = max(tmin, min(t1,t2));
		tmax = min(tmax, max(t1,t2));
	}
	
	if((tmax >= 0) && (tmax >= tmin)){
		collisioninfo *col = new collisioninfo();
		col->body1 = bodyRay;
		col->body2 = bodyAABB;
		collisionpoint cpt1;
		cpt1.pos = from+dir*tmin;
		cpt1.depth = length(dir)*tmin;
		
		if(tmin >= 0){
			collisionpoint cpt2;
			cpt2.pos = from+dir*tmax;
			cpt2.depth = length(dir)*tmax;
			col->cpts.push_back(cpt2);
		}else{
			cpt1.pos = from+dir*tmax;
			cpt1.depth = length(dir)*tmax;
		}
		col->c_to_c = cpt1;
		return col;
	}else{
		return 0;
	}
}

void resolve1Dcollision(float vel1, float mass1, float vel2, float mass2, float *vel1_result, float *vel2_result,float restitution){
	float vel1_new,vel2_new;
	//float restitution = 0.0f;//0.5f;
	if((mass1 == 0) and (mass2 == 0)){printf("warning: resolving collisions, both objects massless\n"); return;}
	if(mass1 == 0){
		//the limit of elastic equations for infinite mass:
		vel1_new = vel1;
		//vel2_new = -vel2+2*vel1;
		vel2_new = vel1+restitution*(vel1-vel2);
	}else if(mass2 == 0){
		//vel1_new = -vel1+2*vel2;
		vel1_new = vel2+restitution*(vel2-vel1);
		vel2_new = vel2;
	}else{
		//vel1_new = (vel1*(mass1-mass2)+2*mass2*vel2)/(mass1+mass2);
		//vel2_new = (vel2*(mass2-mass1)+2*mass1*vel1)/(mass1+mass2);
		vel1_new = (mass1*vel1+mass2*vel2+restitution*mass2*(vel2-vel1))/(mass1+mass2);
		vel2_new = (mass1*vel1+mass2*vel2+restitution*mass1*(vel1-vel2))/(mass1+mass2);
	}
	*vel1_result = vel1_new;
	*vel2_result = vel2_new;
}

//compute friction for a pair of bodies.
float frictionFormula(float f1, float f2){
	return f1*f2;
}

//compute restitution for a pair of bodies
float restitutionFormula(float r1, float r2){
	return r1*r2;
}

//void resolveCollision(collisioninfo col, vec3 *vel1, float mass1, vec3 *vel2, float mass2,float friction,float restitution){
void resolveCollision(collisioninfo *col){
	if(!col){error("no collision info\n");}
	if(!col->body1){error("no body1\n");}
	if(!col->body2){error("no body2\n");}
	vec3 vel1 			= col->body1->vel;
	float mass1 		= col->body1->mass;
	float friction1 	= col->body1->friction;
	float restitution1 	= col->body1->restitution;
	vec3 vel2 			= col->body2->vel;
	float mass2 		= col->body2->mass;
	float friction2 	= col->body2->friction;
	float restitution2 	= col->body2->restitution;
	
	float friction = frictionFormula(friction1, friction2);
	float restitution = restitutionFormula(restitution1, restitution2);
	
	//if(mass1 and mass2){printf("collision: m1 = %f, m2 = %f, friction = %f, rest = %f\n",mass1,mass2,friction,restitution);}
	//we treat mass = 0 as infinite mass
	vec3 n = col->c_to_c.normal;
	if(length(n) == 0){printf("no normal\n"); return;}
	vec3 v1 = vel1-vel2;//*vel1;
	if(length(v1) == 0){/*printf("penetration\n");*/ return;}
	vec3 v2 = vec3(0,0,0);//*vel2;
	
	//float friction = 0.005f;
	
	vec3 v1_perp_comp = glm::proj(v1,n);
	float v1_norm_length = dot(v1_perp_comp,n);
	if(v1_norm_length > 0){/*printf("separating collision\n");*/return;}
	vec3 v1_par_comp = v1-v1_perp_comp;
	
	vec3 v2_perp_comp = -v1_perp_comp;
	float v2_norm_length = 0;
	vec3 v2_par_comp = vec3(0,0,0);
	
	vec3 v1_par_friction = vec3(0,0,0);
	vec3 v2_par_friction = vec3(0,0,0);
	if(length(v1_par_comp)){
		vec3 v1_par_dir = normalize(v1_par_comp);
		float v1_par_len = length(v1_par_comp);
		if(mass1){v1_par_friction = -v1_par_dir*min(friction/mass1,v1_par_len);}
		if(mass2){v2_par_friction = v1_par_dir*min(friction/mass2,v1_par_len);}
	}
	//vec3 v2_perp_comp = proj(v2,n);
	//float v2_norm_length = dot(v2_perp_comp,n);
	//vec3 v2_par_comp = v2-v2_perp_comp;
	
	float v1_perp_comp_result, v2_perp_comp_result;
	
	resolve1Dcollision(v1_norm_length,mass1,v2_norm_length,mass2,&v1_perp_comp_result,&v2_perp_comp_result,restitution);
	
	
	vel1 = vel2+v1_par_comp+normalize(v1_perp_comp)*v1_perp_comp_result+v1_par_friction;
	vel2 = vel2+v2_par_comp+normalize(v2_perp_comp)*v2_perp_comp_result+v2_par_friction;
	
	if(col->body1->type == BODY_DYNAMIC){col->body1->vel = vel1;}
	if(col->body2->type == BODY_DYNAMIC){col->body2->vel = vel2;}
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
int bprecs = 0;
broadphaseinfo *getBroadphaseNodeOnly(octree_node *node){
	broadphaseinfo *bp1 = new broadphaseinfo();
	if(!node){return bp1;}
	if(!node->visitors.size()){return bp1;}
	for(auto I = node->visitors.begin(); I != node->visitors.end(); I++){
	//	string ename = (*I)->body->E->name;
	//	indent(bprecs+1,' '); printf("entity %s\n",ename.c_str());
		bp1->bodies.push_back((*I)->body);
	}
	//and make pairwise pairs of wise
	for(auto I = bp1->bodies.begin(); I != bp1->bodies.end(); I++){
		//if((*I)->type == BODY_NOCOLLIDE){continue;}
		for(auto J = I+1; J != bp1->bodies.end(); J++){
			if(!canCollide(*I,*J)){continue;}
			//S-D, D-D, T-S, T-D collisions only
			bp1->pairs.push_back({*I,*J});
			//string ename1 = (*I)->E->name;
			//string ename2 = (*J)->E->name;
			//printf("pair %s - %s\n",ename1.c_str(),ename2.c_str());
		}
	}
	return bp1;
}

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
				//string ename1 = (*I)->E->name;
				//string ename2 = (*J)->E->name;
				//string nname1 = (*I)->ov->curNode->getName();
				//string nname2 = (*J)->ov->curNode->getName();
				//printf("pair %s (@ %s) - %s (@ %s)\n",
				//	ename1.c_str(),nname1.c_str(),
				//	ename2.c_str(), nname2.c_str()
				//);
			}
		}
	}
	//also concatenate the new bps
	add(bp);
}

broadphaseinfo *checkCollisionBroadphase(octree_node *node){
	if(!node){return 0;}
	rec_counter rc(&bprecs);
	//first, add bodies from this node
	string nname = node->getName();
	//if(!node->isLeaf){
	//	indent(bprecs,'_'); printf("node %s:\n",nname.c_str());
	//}
	broadphaseinfo *bp1 = getBroadphaseNodeOnly(node);
	//then, add the bodies from sub-nodes
	if(!node->isLeaf){
		//indent(bprecs,' '); printf("subnodes of %s:\n",nname.c_str());
		broadphaseinfo *bp2[8];
		for(int I = 0; I < 8; I++){
			//if(node->children[I] && !node->children[I]->isLeaf){
			//	indent(bprecs+1,' '); printf("s.n %d:\n",I);
			//}
			bp2[I] = checkCollisionBroadphase(node->children[I]);
		}
		bp1->addJoin(bp2);
		for(int I = 0; I < 8; I++){delete bp2[I];}
	}
	return bp1;
}

void broadphaseRender(broadphaseinfo *bp){
	//setDepthTest(false);
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
		//printf("body %d pos = %s\n", j, toCString(pos));
		drawPoint(pos);
		j++;
	}
	for(auto I = bp->pairs.begin(); I != bp->pairs.end(); I++){
		drawLine((*I).first->ov->pos,(*I).second->ov->pos);
	}
	//setDepthTest(true);
	//printf("\n");
}

/*
void resolveCollision(collisioninfo col, vec3 *vel1, float mass1, vec3 *vel2, float mass2){
	float energy_retention = 0.9;
	//we treat mass = 0 as infinite mass
	vec3 n = col.c_to_c.normal;
	vec3 v1 = *vel1;
	vec3 v2 = *vel2;
	
	vec3 v1_perp_comp = glm::proj(v1,n);
	float v1_norm_length = dot(v1_perp_comp,n);
	vec3 v1_par_comp = v1-v1_perp_comp;
	vec3 v2_perp_comp = proj(v2,n);
	float v2_norm_length = dot(v2_perp_comp,n);
	vec3 v2_par_comp = v2-v2_perp_comp;
	
	float v1_perp_comp_result, v2_perp_comp_result;
	
	//sad bork :( resolve1Dcollision(v1_norm_length,mass1,v2_norm_length,mass2,&v1_perp_comp_result,&v2_perp_comp_result);
	
	
	*vel1 = v1_par_comp+normalize(v1_perp_comp)*v1_perp_comp_result;
	*vel2 = v2_par_comp+normalize(v2_perp_comp)*v2_perp_comp_result;
}
*/
/*
(aabbA.min.x < aabbB.max.x)&&
(aabbA.min.y < aabbB.max.y)&&
(aabbA.min.z < aabbB.max.z)&&
(aabbA.max.x > aabbB.min.x)&&
(aabbA.max.y > aabbB.min.y)&&
(aabbA.max.z > aabbB.min.z)
*/

//swaps collision parties A and B
collisioninfo *reverseCollision(collisioninfo *ci){
	auto body1 = ci->body1;
	ci->body1 = ci->body2;
	ci->body2 = body1;
	
	//collisionPoint cpt = ci->c_to_c;
	//dunno yet what to do with collision points
	return ci;
}

collisioninfo *checkCollisionAABB_AABB(	collisionbodyAABB *body1, collisionbodyAABB *body2){
	if(!body1){printf("ccAA: no body1\n"); return 0;}
	if(!body2){printf("ccAA: no body2\n"); return 0;}
	AABB aabbA = body1->aabb;//dynamic_cast<collisionbodyAABB*>(cpAABB1.body)->aabb;
	vec3 posA = body1->pos;//cpAABB1.pos;
	vec3 velA = body1->vel;//cpAABB1.vel;
	
	AABB aabbB = body2->aabb;//dynamic_cast<collisionbodyAABB*>(cpAABB2.body)->aabb;
	vec3 posB = body2->pos;//cpAABB2.pos;
	vec3 velB = body2->vel;//cpAABB2.vel;
	//AABB *aabbA = A->aabb;//dynamic_cast<AABB*>(A);
	//AABB *aabbB = B->aabb;//dynamic_cast<AABB*>(B);
	//if(!aabbA || !aabbB){return 0;}
	
	vec3 centerA = (aabbA.start+aabbA.end)/2.f;
	
	vec3 relative = posA-posB;
	
	aabbA = aabbA.moveBy(posA);
	aabbB = aabbB.moveBy(posB);
	
	
	float dist_right 	= aabbB.start.x 		- aabbA.end.x;
	float dist_left 	= aabbA.start.x 		- aabbB.end.x;
	float dist_front 	= aabbB.start.y 		- aabbA.end.y;
	float dist_back 	= aabbA.start.y 		- aabbB.end.y;
	float dist_top 		= aabbB.start.z 		- aabbA.end.z;
	float dist_bottom 	= aabbA.start.z 		- aabbB.end.z;
	
	
	
	//1) check if collision exists at all
	if(
		(dist_right >0)||
		(dist_left >0)||
		(dist_front >0)||
		(dist_back >0)||
		(dist_top >0)||
		(dist_bottom >0)
	){return 0;}
	
	//print2(aabbA.start);
	// print2(aabbA.end);
	// print2(aabbA.size);
	//print2(aabbB.start);
	// print2(aabbB.end);
	// print2(aabbB.size);
	if(false){
	print(dist_right);
	print(dist_left);
	print(dist_front);
	print(dist_back);
	print(dist_top);
	print(dist_bottom);
	}
	collisionpoint resolutions[6];
	//check candidate collision resolutions
	float sign = -1.f;
	setcandidate(0,sign*vec3( 1, 0, 0),dist_right);
	setcandidate(1,sign*vec3(-1, 0, 0),dist_left);
	setcandidate(2,sign*vec3( 0, 1, 0),dist_front);
	setcandidate(3,sign*vec3( 0,-1, 0),dist_back);
	setcandidate(4,sign*vec3( 0, 0, 1),dist_top);
	setcandidate(5,sign*vec3( 0, 0,-1),dist_bottom);
	
	float mindepth = fabs(resolutions[0].depth);
	int bestI = -1; //default = top
	for(int i = 0; i < 6; i++){
		if(fabs(resolutions[i].depth) <= mindepth){
			mindepth = fabs(resolutions[i].depth);
			bestI = i;
		}
	}
	if(bestI == -1){printf("can't find lowest depth\n");bestI = 4;}
	collisioninfo *cinfo = new collisioninfo();
	cinfo->body1 = body1;
	cinfo->body2 = body2;
	cinfo->c_to_c = resolutions[bestI];
	//printf("collision: side %d, dist %f\n",bestI,resolutions[bestI].depth);
	return cinfo;
}

collisioninfo *collisionCheckDispatch(collisionbody *body1, collisionbody *body2){
	collisionbodyAABB *body1AABB = dynamic_cast<collisionbodyAABB*>(body1);
	collisionbodyRay *body1Ray = dynamic_cast<collisionbodyRay*>(body1);
	collisionbodyAABB *body2AABB = dynamic_cast<collisionbodyAABB*>(body2);
	collisionbodyRay *body2Ray = dynamic_cast<collisionbodyRay*>(body2);
	if(body1AABB){
		if(body2AABB){
			return checkCollisionAABB_AABB(body1AABB,body2AABB);
		}else
		if(body2Ray){
			return reverseCollision(checkCollisionRay_AABB(body2Ray,body1AABB));
		}else{
			error("can't check collision pair AABB - uknown");
		}
	}else
	if(body1Ray){
		if(body2AABB){
			return checkCollisionRay_AABB(body1Ray,body2AABB);
		}else
		if(body2Ray){
			return 0; //rays can't collide with other rays
		}else{
			error("can't check collision pair Ray - unknown");
		}
	}else
	error("can't check collision pair unknown - unknown");
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


int numCollisionPairs = 0;//entity *I, entity *J, 
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2){
	if(!body1){printf("pcc: no body1\n"); return;}
	if(!body2){printf("pcc: no body2\n"); return;}
	numCollisionPairs++;
	
	if(body1 == body2){
		//printf("no collision (ignored/same)\n\n");
		return;
	}
	if(body1->type == BODY_NOCOLLIDE || body2->type == BODY_NOCOLLIDE){
		//printf("no collision (ignored/nocollide)\n\n");
		return; //no-collide bodies are not checked for collisions
	}
	if(body1->type == BODY_STATIC && body2->type == BODY_STATIC){
		//printf("no collision (ignored/static)\n\n");
		return; //static-static collisions are ignored
	}
	collisioninfo *col = collisionCheckDispatch(body1,body2);
	if(!col){
		//printf("no collision\n\n");
		return;
	}else{
		// printf("collision check:\n");
		// printf("body1:\n");
		// printf("	entity: %s\n",body1->E->name.c_str());
		// printf("	type: %s\n", toCString(body1->type));
		// printf("body2:\n");
		// printf("	entity: %s\n",body2->E->name.c_str());
		// printf("	type: %s\n", toCString(body2->type));
		// printf("yes collision\n\n");
	}
	//printf("collision!\n");
	resolveCollision(col);
	//vec3 vel1 = body1->vel;			
	//vec3 pos1 = body1->pos;
	//float mass1 = body1->mass;

	//vec3 vel2 = body2->vel;
	//vec3 pos2 = body2->pos;
	//float mass2 = body2->mass;
	float sign = -1.f;
	float separation = 1.01f;
	if(body1->type == BODY_DYNAMIC){//hasComponent(I,velocity)){
		//getComponent(I,velocity)->val = vel1;
		//body1->vel = vel1;
		if(col->c_to_c.depth < 0){
			//getComponent(I,position)->val = pos1+separation*sign*col->c_to_c.penetration;
			body1->pos += separation*sign*col->c_to_c.penetration;
		}
	}
	if(body2->type == BODY_DYNAMIC){//hasComponent(J,velocity)){
		//getComponent(J,velocity)->val = vel2;
		//body2->vel = vel2;
		if(col->c_to_c.depth < 0){
			//getComponent(J,position)->val = pos2-separation*sign*col->c_to_c.penetration;
			body2->pos -= separation*sign*col->c_to_c.penetration;
		}
	}
}

//-----------------------------------------------------------------------------

collisioninfo *raytrace(vec3 from, vec3 dir){
	collisionbodyRay *bodyRay = new collisionbodyRay(from,dir);
	//collisionParty cp[2];
	//cp[0].body = bodyRay;
	//cp[0].ent = 0;
	bodyRay->pos = vec3(0,0,0);
	bodyRay->vel = vec3(0,0,0);
	collisioninfo *bestcol = 0;
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->body){
			collisionbody *body2 = E->body;
			//cp[1].ent = *I;
			//cp[1].pos = getPosition(*I);
			//cp[1].vel = getVelocity(*I);
			collisioninfo *col = collisionCheckDispatch(bodyRay,body2);
			if(col){
				if(bestcol){
					if(bestcol->c_to_c.depth > col->c_to_c.depth){
						bestcol = col;
					}
				}else{
					bestcol = col;
				}
			}
		}
	}
	return bestcol;
}

//future collision stuff:
//checkCollision() - is there a collision at this time?
//checkDistanceStatic() - find the closest points between two bodies
//checkDistanceDynamic() - find the point in time when the static distance between
//							two moving bodies would be zero.
//resolveCollision() - compute the resulting velocities and positions for
//						a collision between two moving rigid bodies.











