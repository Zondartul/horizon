#include "collision.h"
#include "narrowphase.h"
#include "simplemath.h"
#include "globals.h"
#include "geometry.h"
#include "stringUtils.h"

#define print(x) printf(#x ": %f\n", x)
#define print2(x) printf(#x ": %s\n", toString(x).c_str())

string g_sideNames[] = {
	"right",
	"left",
	"front",
	"back",
	"top",
	"bottom"
};
//when the cube is facing north, its right side is to the right (west) of its center.
vec3 g_sideNormals[] = {
	{1,0,0},	//1: right
	{-1,0,0},	//2: left
	{0,1,0},	//3: front
	{0,-1,0},	//4: back
	{0,0,1},	//5: top
	{0,0,-1}	//6: bottom
};
//returns the index of the side closest to the point
int nearestSide(AABB aabb, vec3 p, float *res_dist=0){
	union{
		float dists[6];
		struct{
			float dist_right ;
			float dist_left	 ;
			float dist_front ;
			float dist_back  ;
			float dist_top 	 ;
			float dist_bottom;
		};
	} distances;
	distances.dist_right 	= fabs(aabb.end.x 		- p.x);
	distances.dist_left 	= fabs(aabb.start.x 	- p.x);
	distances.dist_front 	= fabs(aabb.end.y 		- p.y);
	distances.dist_back 	= fabs(aabb.start.y 	- p.y);
	distances.dist_top 		= fabs(aabb.end.z 		- p.z);
	distances.dist_bottom 	= fabs(aabb.start.z 	- p.z);
	int bestI = 0; float bestDist = distances.dist_right;
	for(int i = 1; i < 6; i++){
		if(distances.dists[i] < bestDist){
			bestI = i;
			bestDist = distances.dists[i];
		}
	}
	if(res_dist){*res_dist = bestDist;}
	return bestI;
	
}



collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB){
	vec3 from = bodyRay->from;
	vec3 dir = bodyRay->dir;
	AABB aabb = bodyAABB->getAABB().moveBy(bodyAABB->pos);

	//times of intersection between ray and each of the AABB planes.
	//we have intersection if there is time t such that
	//the ray has gone "inside" on every axis yet not has gone "outside" on any axis.
	float xmin, xmax, ymin, ymax, zmin, zmax;
	float t1,t2;
	if(dir.x != 0){
		t1 = (aabb.start.x - from.x)/dir.x;
		t2 = (aabb.end.x - from.x)/dir.x;
		if((t1 < 0) && (t2 < 0)){return 0;} //our rays are pointy
		xmin = min(max(t1,0),max(t2,0));
		xmax = max(t1,t2);
	}else{
		if((from.x < aabb.start.x) || (from.x > aabb.end.x)){return 0;}
		xmin = 0; xmax = INFINITY;
	}
	if(dir.y != 0){
		t1 = (aabb.start.y - from.y)/dir.y;
		t2 = (aabb.end.y - from.y)/dir.y;
		if((t1 < 0) && (t2 < 0)){return 0;} //our rays are pointy
		ymin = min(max(t1,0),max(t2,0));
		ymax = max(t1,t2);
	}else{
		if((from.y < aabb.start.y) || (from.y > aabb.end.y)){return 0;}
		ymin = 0; ymax = INFINITY;
	}
	if(dir.z != 0){
		t1 = (aabb.start.z - from.z)/dir.z;
		t2 = (aabb.end.z - from.z)/dir.z;
		if((t1 < 0) && (t2 < 0)){return 0;} //our rays are pointy
		zmin = min(max(t1,0),max(t2,0));
		zmax = max(t1,t2);
	}else{
		if((from.z < aabb.start.z) || (from.z > aabb.end.z)){return 0;}
		zmin = 0; zmax = INFINITY;
	}
	float tenter = max(max(xmin,ymin),zmin);
	float texit = min(min(xmax,ymax),zmax);
	
	
	if(tenter <= texit){
		

		
		if(tenter == 0){tenter = texit;} //if we are inside the object, report the exit point instead.
		collisioninfo *col = new collisioninfo();
		col->body1 = bodyRay;
		col->body2 = bodyAABB;
		collisionpoint cpt1;
		cpt1.pos = from+dir*tenter;
		cpt1.depth = length(dir)*tenter;
		cpt1.normal = g_sideNormals[nearestSide(aabb, cpt1.pos)];
		col->c_to_c = cpt1;
		return col;
	}else{
		return 0;
	}
}

#define setcandidate(x,n,z)	{		    \
	resolutions[x].pos = centerA;       \
	resolutions[x].normal = n;          \
	resolutions[x].penetration = n*z;   \
	resolutions[x].depth = z;		    \
}

collisioninfo *checkCollisionAABB_AABB(	collisionbodyAABB *body1, collisionbodyAABB *body2){
	if(!body1){printf("ccAA: no body1\n"); return 0;}
	if(!body2){printf("ccAA: no body2\n"); return 0;}
	AABB aabbA = body1->getAABB();
	vec3 posA = body1->pos;
	vec3 velA = body1->vel;

	AABB aabbB = body2->getAABB();
	vec3 posB = body2->pos;
	vec3 velB = body2->vel;

	vec3 centerA = posA+(aabbA.start+aabbA.end)/2.f;

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
	return cinfo;
}

#include "paint.h"
void debugDrawAABB(AABB aabb, vec3 col){
	setLayer(g_layerDebug);
	setPosition(vec3(0,0,0));
	setColor(col);
	drawBoxWireframe(aabb);
}

void debugDrawPoint(vec3 point, vec3 col){
	setLayer(g_layerDebug);
	setPointSize(8.f);
	setPosition(vec3(0,0,0));
	setColor(col);
	drawPoint(point);
}

void debugDrawLine(vec3 p1, vec3 p2, vec3 col){
	setLayer(g_layerDebug);
	setPosition(vec3(0,0,0));
	setRotation(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setColor(col);
	drawLine(p1,p2);
}

string truncateString(string S, int n){
	if(S.length() > n){
		return (S.substr(0,n-3)+"...");
	}else{
		return S;
	}
}

//fashions a collisioninfo, given that:
// - the point p1 of body1 penetrated body2 through entry point p2
// - the bodies were moving with their current velocities when that happened
// - depth comes from (p1-p2) and direction comes from relative velocity.
collisioninfo *pointToPointPenetration(vec3 p1, collisionbody *body1, vec3 p2, collisionbody *body2, vec3 normal){
	float depth = length(p2-p1);
	vec3 dv = body1->vel - body2->vel;
	vec3 dv_dir = normalizeSafe(dv);
	
	collisioninfo *ci = new collisioninfo();
	ci->body1 = body1;
	ci->body2 = body2;
	ci->c_to_c.pos = p1;
	ci->c_to_c.depth = depth;
	ci->c_to_c.penetration = -normal * depth;
	ci->c_to_c.normal = normal;
	
	return ci;
}

collisioninfo *checkCollisionPoint_Terrain( collisionbodyPoint *body1, collisionbodyTerrain *body2){
	vec3 p = body1->pos;
	vec3 p_rel = p - body2->pos;
	vec2 p_rel2 = vec2(p_rel.x, p_rel.y);
	vec2 t_end = body2->grid.getEnd();
	if((p_rel2.x < 0) || (p_rel2.y < 0) ||
		(p_rel2.x > t_end.x) || (p_rel2.y > t_end.y)){
			return 0; //point is outside of terrain horizontally
		}
	float tz = body2->grid.height(p_rel2); //get terrain height at a point relative to the beginning of terrain
	if(tz > p_rel.z){ //if terrain (relative to it's start) is higher then the point (also relative to terrain's start)
		vec3 p2 = p;
		p2.z = tz + body2->pos.z;
		vec3 normal = body2->grid.normal(p_rel2);
		return pointToPointPenetration(p, body1, p2, body2, normal);
	}else{
		return 0;
	}
}

collisioninfo *checkCollisionAABB_Terrain( collisionbodyAABB *body1, collisionbodyTerrain *body2){

	bool check_center_terrain = false;			//generate a point-terrain collision between center of aabb, and terrain
	bool check_bottomcenter_terrain = true;	//generate a point-terrain collision between center of bottom face of aabb, and terrain
	bool check_corners_terrain = false;			//generate a point-terrain collision between the conrners of the bottom face of the aabb, and terrain
	bool check_triangles = false;				//generate a triangle-aabb collision between triangles of terrain, and aabb.
	bool average_manifold = false;				//true = make an average of all collisions, false = return the first one
 
	vector<collisioninfo*> cis;
	AABB boxaabb = body1->getAABB().moveBy(body1->pos);
	
	auto checkPointTerrain = [&](vec3 p){
		auto cbp  = collisionbodyPoint(p);
		cbp.vel = body1->vel;
		collisioninfo *ci = checkCollisionPoint_Terrain(&cbp, body2);
		if(ci){
			ci->body1 = body1;
			cis.push_back(ci);
		}
	};
	
	if(check_center_terrain){
		vec3 p = (boxaabb.start+boxaabb.end)/2.f;
		checkPointTerrain(p);
	}
	if(check_bottomcenter_terrain){
		vec3 p = (boxaabb.start+boxaabb.end)/2.f;
		p.z = boxaabb.start.z;
		checkPointTerrain(p);
	}
	if(check_corners_terrain){
		vec3 p1 = vec3(boxaabb.start.x, boxaabb.start.y, boxaabb.start.z);
		vec3 p2 = vec3(boxaabb.end.x, boxaabb.start.y, boxaabb.start.z);
		vec3 p3 = vec3(boxaabb.end.x, boxaabb.end.y, boxaabb.start.z);
		vec3 p4 = vec3(boxaabb.start.x, boxaabb.end.y, boxaabb.start.z);
		checkPointTerrain(p1);
		checkPointTerrain(p2);
		checkPointTerrain(p3);
		checkPointTerrain(p4);
	}
	
	if(average_manifold){
		//averaging code here
		return 0;
	}else{
		if(cis.size()){
			return cis[0];
		}else{
			return 0;
		}
	}
	
	return 0;
}

/*

============================== AABB-TERRAIN NOTES =======================================

	A	      B
*-------J-----*		EG - line at the input Y-coord (from AABB)
|       |   * |		F - intersection of EG's projection and BC's (diagonal's) projection
|E     F| *   |G	EF not collinear to FG, nor to EG
*-------*-----*		J,H to make complete tiles
|     * |     |
|   *   |     |
| *     |     |
*-------*-----*
C	H     D
.	      .
1             2


E.y = F.y = G.y = Yb
E.x = A.x = C.x = X1
G.x = B.x = D.x = X2

dX1 = A.x - C.x
dY1 = A.y - C.y
dZ1 = A.z - C.z

dX2 = B.x - D.x
dY2 = B.y - D.y
dZ2 = B.z - D.z


dYd = B.y - C.y

|dYE = E.y - C.y = Yb - C.y
|dYG = G.y - D.y = Yb - D.y
|dYF = F.y - C.y = Yb - C.y
|... dYany = Yb - C.y

|dX = X2-X1

dZd = B.z - C.z

k = dYE/dY1 = dYG/dY2 = dYF/dYd
  = dZE/dZ1 = dZG/dZ2 = dZF/dZd

E = (C.x, C.y+k*dY1, C.z+k*dZ1)
F = (C.x+k*dX, Cy+k*dYd, Cz+k*dZd)
G = (D.x, D.x+k*dY2, D.z+k*dZ2)

alternatively, use bilinear height function

z(x,y) = (zA*(1-x)+zB*(x))*(1-y)+(zC*(1-x)+zD*(x))*(y)

normals:

zA
- zA*x + zB*x
- zA*y + Zc*y
+ zA*x*y - zB*x*y - Zc*x*y + zD*x*y

dz/dx = - zA + zB + zA*y - zB*y - Zc*y + zD*y
dz/dy = - zA + zC + zA*x - zB*x - zC*x + zD*x


	*/

collisioninfo *checkCollisionRay_Terrain( collisionbodyRay *body1, collisionbodyTerrain *body2){
	//todo: throw out the triangles, use a grid of height values
	if(body2->em){
		vec3 ray_start = body1->from-body2->pos;
		vec3 ray_dir = body1->dir;
		float mindist = -1.f;
		vec3 bestp;
		vec3 bestnormal;
		bool has_intersection = false;
		for(auto I = body2->em->tris.begin(); I != body2->em->tris.end(); I++){
			vec3 p;
			vec2 bari;
			e_triangle &tri = **I;
			e_vertex &vertA = *(tri.definition.verts[0]);
			e_vertex &vertB = *(tri.definition.verts[1]);
			e_vertex &vertC = *(tri.definition.verts[2]);

			vec3 &A = vertA.pos;
			vec3 &B = vertB.pos;
			vec3 &C = vertC.pos;

			bool has_hit = ray_triangle_intersection(ray_start, ray_dir, A,B,C, &p, &bari);
			if(has_hit){
				has_intersection = true;
				float dist = length(p-ray_start);
				if((mindist < 0) || (dist < mindist)){
					mindist = dist;
					bestp = p;
					bestnormal = triangle_normal(A,B,C);
				}
			}
		}
		if(has_intersection){
			collisioninfo *col = new collisioninfo();
			col->body1 = body1;
			col->body2 = body2;
			col->c_to_c.pos = bestp+body2->pos;
			col->c_to_c.normal = bestnormal;
			col->c_to_c.penetration = vec3(0,0,0);
			col->c_to_c.depth = mindist;
			return col;
		}else{
		}
	}
	return 0;
};

collisioninfo *checkCollisionPoint_AABB(collisionbody *body1, collisionbody *body2){
    AABB aabb = body2->getAABB();
    if(aabb.contains(body1->pos-body2->pos)){
        collisioninfo *col = new collisioninfo();
        col->body1 = body1;
        col->body2 = body2;
        col->c_to_c.pos = body1->pos;
		col->c_to_c.normal = g_sideNormals[nearestSide(aabb, body1->pos - body2->pos)];
        return col;
    }
    return 0;
}

collisioninfo *checkCollisionAABB_Plane(collisionbody *body1, collisionbodyPlane *body2){
	vec3 n = body2->normal;
	float offset = body2->offset;
	
	AABB aabb = body1->getAABB().moveBy(body1->pos);
	float x1 = aabb.start.x; float x2 = aabb.end.x;
	float y1 = aabb.start.y; float y2 = aabb.end.y;
	float z1 = aabb.start.z; float z2 = aabb.end.z;
	
	vec3 pts[8];
	
	pts[0] = vec3(x1,y1,z1); //A
	pts[1] = vec3(x2,y1,z1); //B
	pts[2] = vec3(x1,y2,z1); //C
	pts[3] = vec3(x2,y2,z1); //D
	pts[4] = vec3(x1,y1,z2); //E
	pts[5] = vec3(x2,y1,z2); //F
	pts[6] = vec3(x1,y2,z2); //G
	pts[7] = vec3(x2,y2,z2); //H
	
	float bestDepth = -1.f; int bestI = 0;
	for(int i = 0; i < 8; i++){
		vec3 p = pts[i];
		float depth = -(dot(p,n) - offset);
		if(depth > bestDepth){bestDepth = depth; bestI = i;}
	}
	if(bestDepth > 0){
		vec3 pos = pts[bestI];
		collisioninfo *col = new collisioninfo();
		col->body1 = body1;
		col->body2 = body2;
		col->c_to_c.pos = pos;
		col->c_to_c.normal = n;
		col->c_to_c.depth = bestDepth;
		col->c_to_c.penetration = -bestDepth*n;
		return col;
	}
	return 0;
}

collisioninfo *checkCollisionRay_Plane(collisionbodyRay *body1, collisionbodyPlane *body2){
	vec3 dir = body1->dir;
	vec3 start = body1->from;
	vec3 n = body2->normal;
	float offset = body2->offset;
	vec3 pos;
	
	////if ray is pointing towards plane, i.e.:
	//if(((dot(start,n) < offset) && (dot(dir,n) > 0)) //ray start below plane and in same direction
	//	|| (dot(dir,n) < 0))	//or ray start above plane and in opposite direction
	if(ray_plane_intersection(start, dir, n, offset, &pos)){//then we have a collision
		collisioninfo *col = new collisioninfo();
		col->body1 = body1;
		col->body2 = body2;
		col->c_to_c.pos = pos;
		col->c_to_c.normal = n;
		vec3 dv = pos-start;
		col->c_to_c.depth = length(dv);
		col->c_to_c.penetration = dv;
		return col;
	}
	return 0;
}

collisioninfo *checkCollisionPoint_Plane(collisionbody *body1, collisionbodyPlane *body2){
	vec3 p = body1->pos;
	vec3 n = body2->normal;
	float offset = body2->offset;
	float depth = offset - dot(p,n);
	if(depth > 0){
		collisioninfo *col = new collisioninfo();
		col->body1 = body1;
		col->body2 = body2;
		col->c_to_c.pos = p;
		col->c_to_c.normal = n;
		col->c_to_c.depth = depth;
		col->c_to_c.penetration = -depth*n;
		return col;
	}
	return 0;
}

//The separating axis that realizes the axis of minimum penetration is
//either a face normal or the cross product between two edges.

collisioninfo *checkCollisionSAT(collisionbody *body1, collisionbody *body2){
	return 0;
}

/*
!!!!!!!!!! dynamic intersection test with SAT:

The algorithm goes like this. You work with the relative velocity vector of the two convex bodies.
Projecting each of the two bodies and the relative velocity vector onto a particular separating axis
at t₀ gives two 1-D intervals and a 1-D velocity, such that it is easy to tell whether the two intervals
intersect, and if not, whether they are moving apart or moving together. If they are separated and
moving apart on any of the separating axes (or, in fact, on any axis whatever), then you know that
there is no future collision. If on any separating axis the two projected intervals intersect at t₀ or
are separated and are moving together, then it is easy to compute (by two simple 1D linear
expressions) the earliest future time at which the two intervals will first intersect and (assuming
continuing rectilinear motion) the latest future time at which the two intervals will last intersect and
begin moving apart. (If they are intersecting at t₀ then the earliest future intersection time is t₀). Do
this for at most all the separating axes. If the maximum over all the axes of the earliest future
intersection time is less than the minimum over all the axes of the latest future intersection time
then that maximum earliest future intersection time is the exact time of first collision of the two 3D
polyhedra, otherwise there is no collision in the future.
*/

collisioninfo *collisionCheckDispatch(collisionbody *body1, collisionbody *body2){
    collisioninfo *col = 0;

	collisionbodyTerrain *body1Terrain = dynamic_cast<collisionbodyTerrain*>(body1);
	collisionbodyAABB *body1AABB = dynamic_cast<collisionbodyAABB*>(body1);
	collisionbodyRay *body1Ray = dynamic_cast<collisionbodyRay*>(body1);
	collisionbodyPoint *body1Point = dynamic_cast<collisionbodyPoint*>(body1);
	collisionbodyPlane *body1Plane = dynamic_cast<collisionbodyPlane*>(body1);
	collisionbodyTerrain *body2Terrain = dynamic_cast<collisionbodyTerrain*>(body2);
	collisionbodyAABB *body2AABB = dynamic_cast<collisionbodyAABB*>(body2);
	collisionbodyRay *body2Ray = dynamic_cast<collisionbodyRay*>(body2);
	collisionbodyPoint *body2Point = dynamic_cast<collisionbodyPoint*>(body2);
	collisionbodyPlane *body2Plane = dynamic_cast<collisionbodyPlane*>(body2);
	if(body1Terrain){
		if(body2Terrain){col = 0; /*terrain can't collide with terrain*/}else
		if(body2AABB){col = reverseCollision(checkCollisionAABB_Terrain(body2AABB,body1Terrain));}else
		if(body2Ray){col = reverseCollision(checkCollisionRay_Terrain(body2Ray,body1Terrain));}else
		if(body2Point){col = reverseCollision(checkCollisionPoint_Terrain(body2Point,body1Terrain));}else
		if(body2Plane){col = 0; /*terrain can't collide with plane*/}else
		{error("can't check collision pair AABB - uknown");}
	}else
	if(body1AABB){
		if(body2Terrain){col = checkCollisionAABB_Terrain(body1AABB,body2Terrain);}else
		if(body2AABB){col = checkCollisionAABB_AABB(body1AABB,body2AABB);}else
		if(body2Ray){col = reverseCollision(checkCollisionRay_AABB(body2Ray,body1AABB));}else
		if(body2Point){col = reverseCollision(checkCollisionPoint_AABB(body2Point,body1AABB));}else
		if(body2Plane){col = checkCollisionAABB_Plane(body1AABB,body2Plane);}else
		{error("can't check collision pair AABB - uknown");}
	}else
	if(body1Ray){
		if(body2Terrain){col = checkCollisionRay_Terrain(body1Ray,body2Terrain);}else
		if(body2AABB){col = checkCollisionRay_AABB(body1Ray,body2AABB);}else
		if(body2Ray){col = 0; /*rays can't collide with other rays*/}else
		if(body2Point){col = 0; /*or with points for that matter*/}else
		if(body2Plane){col = checkCollisionRay_Plane(body1Ray,body2Plane);}else
		{error("can't check collision pair Ray - unknown");}
	}else
	if(body1Point){
        if(body2Terrain){col = checkCollisionPoint_Terrain(body1Point,body2Terrain);}else
        if(body2AABB){col = checkCollisionPoint_AABB(body1Point,body2AABB);}else
        if(body2Ray){col = 0; /*points can't collide with rays*/}else
        if(body2Point){col = 0; /*points also can't collide with points*/}else
        if(body2Plane){col = checkCollisionPoint_Plane(body1Point,body2Plane);}else
		{error("can't check collision pair Point - unknown");}
	}else
	if(body1Plane){
		if(body2Terrain){col = 0; /*plane can't collide with terrain*/}else
		if(body2AABB){col = reverseCollision(checkCollisionAABB_Plane(body2AABB,body1Plane));}else
		if(body2Ray){col = reverseCollision(checkCollisionRay_Plane(body2Ray, body1Plane));}else
		if(body2Point){col = reverseCollision(checkCollisionPoint_Plane(body2Point, body1Plane));}else
		if(body2Plane){col = 0; /*plane can't collide with another plane*/}else
		{error("can't check collision pair Plane - unknown");}
	}else{
        error("can't check collision pair unknown - unknown");
	}

	if(col){
        if(body1->onCollision){body1->onCollision(col);}
        if(body2->onCollision){body2->onCollision(col);}
        return col;
	}
	return 0;
}

int g_numCollisionPairs = 0;//entity *I, entity *J,
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2, collisionOptions &options){
	if(!body1){printf("pcc: no body1\n"); return;}
	if(!body2){printf("pcc: no body2\n"); return;}
	g_numCollisionPairs++;
	if(!canCollide(body1,body2)){return;}
	collisioninfo *col = collisionCheckDispatch(body1,body2);
	if(!col){
		return;
	}else{
	}
	if((body1->type != BODY_TRIGGER) && (body2->type != BODY_TRIGGER)){
        if(options.separate){separateCollision(col);}
        if(options.resolve){resolveCollision(col);}
	}
	col->body1->ov->moveTo(body1->pos);
	col->body2->ov->moveTo(body2->pos);
	delete col;
}
