#include "collision.h"
#include "narrowphase.h"
#include "simpleMath.h"
#include "globals.h"
#include "geometry.h"
#include "stringUtils.h"

#define print(x) printf(#x ": %f\n", x)
#define print2(x) printf(#x ": %s\n", toString(x).c_str())

//#define INFINITY (1.f/0.f)

string sideNames[] = {
	"right",
	"left",
	"front",
	"back",
	"top",
	"bottom"
};
//when the cube is facing north, its right side is to the right (west) of its center.
vec3 sideNormals[] = {
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
	//printf("side = %s, bestDist = %f\n",sideNames[bestI].c_str(), bestDist);
	return bestI;
	
}



collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB){
	//collisionbodyRay *bodyRay = dynamic_cast<collisionbodyRay*>(cpRay.body);
	//collisionbodyAABB *bodyAABB = dynamic_cast<collisionbodyAABB*>(cpAABB.body);
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
		cpt1.normal = sideNormals[nearestSide(aabb, cpt1.pos)];
		//printf("ccRay_AABB: from = %s, dir = %s, tenter = %s, length(dir) = %s\n",
		//		toCString(from),
		//		toCString(dir),
		//		toCString(tenter),
		//		toCString(length(dir)));
		col->c_to_c = cpt1;
		return col;
	}else{
		return 0;
	}
	/*
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
	*/
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
	AABB aabbA = body1->getAABB();//dynamic_cast<collisionbodyAABB*>(cpAABB1.body)->aabb;
	vec3 posA = body1->pos;//cpAABB1.pos;
	vec3 velA = body1->vel;//cpAABB1.vel;

	AABB aabbB = body2->getAABB();//dynamic_cast<collisionbodyAABB*>(cpAABB2.body)->aabb;
	vec3 posB = body2->pos;//cpAABB2.pos;
	vec3 velB = body2->vel;//cpAABB2.vel;
	//AABB *aabbA = A->aabb;//dynamic_cast<AABB*>(A);
	//AABB *aabbB = B->aabb;//dynamic_cast<AABB*>(B);
	//if(!aabbA || !aabbB){return 0;}

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



#include "paint.h"
void debugDrawAABB(AABB aabb, vec3 col){
	setLayer(layerDebug);
	setPosition(vec3(0,0,0));
	setColor(col);
	drawBoxWireframe(aabb);
}

void debugDrawPoint(vec3 point, vec3 col){
	setLayer(layerDebug);
	setPointSize(8.f);
	setPosition(vec3(0,0,0));
	setColor(col);
	drawPoint(point);
}

void debugDrawLine(vec3 p1, vec3 p2, vec3 col){
	setLayer(layerDebug);
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
	ci->c_to_c.penetration = -normal * depth;//-dv_dir * depth;
	ci->c_to_c.normal = normal;
	
	//printf("ptpPen: pos = %s, depth = %f, pen = %s, normal = %s\n", toCString(ci->c_to_c.pos),
	//																ci->c_to_c.depth,
	//																toCString(ci->c_to_c.penetration),
	//																toCString(ci->c_to_c.normal));
	//printf("b1vel = %s, b2vel = %s, dv = %s, dv_dir = %s\n", toCString(body1->vel),
	//															toCString(body2->vel),
	//															toCString(dv),
	//															toCString(dv_dir));
	return ci;
}

collisioninfo *checkCollisionPoint_Terrain( collisionbodyPoint *body1, collisionbodyTerrain *body2){
	vec3 p = body1->pos;
	vec3 p_rel = p - body2->pos;
	vec2 p_rel2 = vec2(p_rel.x, p_rel.y);
	vec2 t_end = body2->grid.getEnd();
	//printf("________________________________\nccPT: input (%s)\n",toCString(p));
	if((p_rel2.x < 0) || (p_rel2.y < 0) ||
		(p_rel2.x > t_end.x) || (p_rel2.y > t_end.y)){
			//printf("ccPT: false / pt %s is outside (max = %s)\n",toCString(p_rel2), toCString(t_end));
			return 0; //point is outside of terrain horizontally
		}
	float tz = body2->grid.height(p_rel2); //get terrain height at a point relative to the beginning of terrain
	if(tz > p_rel.z){ //if terrain (relative to it's start) is higher then the point (also relative to terrain's start)
		vec3 p2 = p;
		p2.z = tz + body2->pos.z;
		//vec2 gradient = body2->grid.gradient(p_rel2);
		vec3 normal = body2->grid.normal(p_rel2);//normalizeSafe(vec3(gradient.x, gradient.y, 1.f));
		//printf("ccPT true / pt %s is below tz %f\n", toCString(p_rel), tz);
		return pointToPointPenetration(p, body1, p2, body2, normal);
	}else{
		//printf("ccPT false / pt %s is above tz %f\n", toCString(p_rel), tz);
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
collisioninfo *checkCollisionAABB_Terrain( collisionbodyAABB *body1, collisionbodyTerrain *body2){
	//static bool debugStop = false;
	bool debugStop = (body1->vel == vec3(0,0,0));
	if(!debugStop)
	printf("\n\nccAABB_Terrain(%s, %s)\n", toCString(body1->name()), toCString(body2->name()));

	//1. init vars
	//1.1. result vars
	collisioninfo *ci = 0;
	collisioninfo *cray = 0;
	collisionbodyRay ray;
	bool RateOnlyHeight = false;
	int Ix_best = 0;
	int Iy_best = 0;
	float bestscore = 0;
	bool hasCollision = false;
	vec3 dv = body2->vel - body1->vel;
	vec3 pendir = normalizeSafe(dv);
	AABB boxaabb = body1->getAABB().moveBy(body1->pos);
	debugDrawAABB(boxaabb,vec3(128,0,0));
	//1.2. box coords
	float b_z1 = boxaabb.start.z;
	float b_z2 = boxaabb.end.z;
	float b_x1 = boxaabb.start.x;
	float b_x2 = boxaabb.end.x;
	float b_y1 = boxaabb.start.y;
	float b_y2 = boxaabb.end.y;

	//
	//add to class:
	//struct {
	//	vector<float> height_grid;
	//	float x_step, y_step;
	//	int x_count, y_count;
	//  float operator[](int Ix, int Iy){
	//		return height_grid[Ix+Iy*x_count];
	//	}
	//} grid;

	//1.3. grid coords
	AABB groundaabb = body2->getAABB().moveBy(body2->pos);
	vec3 offs = body2->pos;
	float grid_x1 = groundaabb.start.x;
	float grid_x2 = groundaabb.end.x;
	float grid_y1 = groundaabb.start.y;
	float grid_y2 = groundaabb.end.y;
	float grid_z1 = groundaabb.start.z;
	float grid_z2 = groundaabb.end.z;
	debugDrawAABB(groundaabb,vec3(0,0,128));

	auto &grid = body2->grid;
	//2. early exit if no AABB intersection
	bool hasAABBcol = true;
	if(b_x1 > grid_x2){hasAABBcol = false;}
	if(b_x2 < grid_x1){hasAABBcol = false;}
	if(b_y1 > grid_y2){hasAABBcol = false;}
	if(b_y2 < grid_y1){hasAABBcol = false;}
	if(b_z1 > grid_z2){hasAABBcol = false;}
	if(b_z2 < grid_z1){hasAABBcol = false;} //comment this out for infinitely deep terrain
	//if(!debugStop)
	//printf("hasAABBcol = %d\n",hasAABBcol);
	if(!hasAABBcol){return 0;}

	rmodel *rm = new rmodel();
	for(int Ix = 0; Ix < grid.x_point_count; Ix++){
		for(int Iy = 0; Iy < grid.y_point_count; Iy++){
			vec3 v = grid.point(Ix,Iy)+offs;
			rm->vertices->push_back(v);
		}
	}
	rm->finalize();

	setLayer(layerDebug);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setRenderMode(1);
	setPointSize(3.f);
	setColor(vec3(32,32,32));
	uploadRmodel(rm);
	drawRmodel(rm);
	deleteRmodel(rm);

	rm = new rmodel();
	for(int Ix = 0; Ix < grid.x_point_count-1; Ix++){
		for(int Iy = 0; Iy < grid.y_point_count-1; Iy++){
			vec3 vA = grid.point(Ix,Iy)+offs;
			vec3 vB = grid.point(Ix+1,Iy)+offs;
			vec3 vC = grid.point(Ix,Iy+1)+offs;
			vec3 vD = grid.point(Ix+1,Iy+1)+offs;

			rm->vertices->push_back(vA);
			rm->vertices->push_back(vB);

			rm->vertices->push_back(vB);
			rm->vertices->push_back(vD);

			rm->vertices->push_back(vD);
			rm->vertices->push_back(vC);

			rm->vertices->push_back(vC);
			rm->vertices->push_back(vA);

			rm->vertices->push_back(vB);
			rm->vertices->push_back(vC);
		}
	}
	rm->finalize();

	setLayer(layerDebug);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setRenderMode(2);
	setColor(vec3(0,0,0));
	uploadRmodel(rm);
	drawRmodel(rm);
	deleteRmodel(rm);

	//3. find grid indices
	float g_dx1 = b_x1 - grid_x1;
		int g_x1_out_idx = clampi(floori(g_dx1/grid.x_step),  0,grid.x_point_count-1);
		int g_x1_in_idx  = clampi(floori(g_dx1/grid.x_step)+1,0,grid.x_point_count-1);

	float g_dx2 = b_x2 - grid_x1;
		int g_x2_out_idx = clampi(floori(g_dx2/grid.x_step)+1,0,grid.x_point_count-1);
		int g_x2_in_idx  = clampi(floori(g_dx2/grid.x_step),  0,grid.x_point_count-1);
												//i don't trust int(floor(x))
	float g_dy1 = b_y1 - grid_y1;
		int g_y1_out_idx = clampi(floori(g_dy1/grid.y_step),  0,grid.y_point_count-1);
		int g_y1_in_idx  = clampi(floori(g_dy1/grid.y_step)+1,0,grid.y_point_count-1);

	float g_dy2 = b_y2 - grid_y1;
		int g_y2_out_idx = clampi(floori(g_dy2/grid.y_step)+1,0,grid.y_point_count-1);
		int g_y2_in_idx  = clampi(floori(g_dy2/grid.y_step),  0,grid.y_point_count-1);


	//4. clip box coords to grid area
	if(b_x2 > grid_x2){b_x2 = grid_x2;}
	if(b_x1 < grid_x1){b_x1 = grid_x1;}
	if(b_y2 > grid_y2){b_y2 = grid_y2;}
	if(b_y1 < grid_y1){b_y1 = grid_y1;}

	if(!debugStop)
	printf("g_out_idx: x1 %d, x2 %d, y1 %d, y2 %d\n",g_x1_out_idx, g_x2_out_idx, g_y1_out_idx, g_y2_out_idx);
	//5. iterate over the grid
	//5.1 check verts for the tiles that are fully inside
	vec3 dp1 = grid.point(g_x1_out_idx,g_y1_out_idx);
	vec3 dp1v = grid.debugVerts[g_x1_out_idx+g_y1_out_idx*grid.x_point_count];
	debugDrawPoint(dp1+offs,vec3(128,0,128));
	debugDrawLine(dp1+offs,dp1v+offs,vec3(0,255,0));

	vec3 dp2 = grid.point(g_x2_out_idx,g_y1_out_idx);
	vec3 dp2v = grid.debugVerts[g_x2_out_idx+g_y1_out_idx*grid.x_point_count];
	debugDrawPoint(dp2+offs,vec3(255,64,0));
	debugDrawLine(dp2+offs,dp2v+offs,vec3(0,255,0));

	vec3 dp3 = grid.point(g_x1_out_idx,g_y2_out_idx);
	vec3 dp3v = grid.debugVerts[g_x1_out_idx+g_y2_out_idx*grid.x_point_count];
	debugDrawPoint(dp3+offs,vec3(255,64,0));
	debugDrawLine(dp3+offs,dp3v+offs,vec3(0,255,0));

	vec3 dp4 = grid.point(g_x2_out_idx,g_y2_out_idx);
	vec3 dp4v = grid.debugVerts[g_x2_out_idx+g_y2_out_idx*grid.x_point_count];
	debugDrawPoint(dp4+offs,vec3(255,64,0));
	debugDrawLine(dp4+offs,dp4v+offs,vec3(0,255,0));


	int outside_x_count = g_x2_out_idx-g_x1_out_idx;
	int outside_y_count = g_y2_out_idx-g_y1_out_idx;
	int inside_x_count = g_x2_in_idx-g_x1_in_idx;
	int inside_y_count = g_y2_in_idx-g_y1_in_idx;

	if(inside_x_count && inside_y_count){
		debugDrawPoint(grid.point(g_x1_in_idx,g_y1_in_idx)+offs,vec3(255,0,255));
		debugDrawPoint(grid.point(g_x2_in_idx,g_y1_in_idx)+offs,vec3(255,255,0));
		debugDrawPoint(grid.point(g_x1_in_idx,g_y2_in_idx)+offs,vec3(255,255,0));
		debugDrawPoint(grid.point(g_x2_in_idx,g_y2_in_idx)+offs,vec3(255,255,0));
	}
	if(!debugStop)
	printf("ext. tiles: (%d x %d), int. tiles: (%d x %d)\n",outside_x_count,outside_y_count,inside_x_count,inside_y_count);
	int inside_point_count = 0;

	//------------------------------------- internal functions --------------------------------------------
	auto inBox = [&](vec3 p)->bool{
		bool in=((p.x >= b_x1) && (p.x <= b_x2) &&
				(p.y >= b_y1) && (p.y <= b_y2) &&
				(p.z >= b_z1)); //points above the box count as inside (pen. from below)
		//printf("p %s %s\n",toCString(p), in? "inBox!" : "out");
		return in;
	};

	//getTileFromDir() - 	Given ray "pendir" coming into vertex with index [Ix,Iy],
	//						returns one of the four tiles that share the [Ix,Iy] corner
	//						that is also the last tile under the ray.
	//						Used to find the correct face of a spike/pyramid/hill
	//						for collision normal calculation.
	auto getTileFromDir = [&](vec3 pendir, int Ix, int Iy)->tile{
		tile T = {&grid,TILE_DIAG,0,0,0,0};
		if(pendir.x >= 0){
			T.Ix_A = Ix; T.Ix_B = Ix+1;
		}else{
			T.Ix_A = Ix-1; T.Ix_B = Ix;
		}
		if(T.Ix_A < 0){T.Ix_A = 0; T.Ix_B = 1;}
		if(T.Ix_B > grid.x_point_count-1){T.Ix_B = grid.x_point_count-1; T.Ix_A = T.Ix_B-1;}
		T.Ix_C = T.Ix_A; T.Ix_D = T.Ix_B;

		if(pendir.y >= 0){
			T.Iy_A = Iy;
			T.Iy_C = Iy+1;
		}else{
			T.Iy_A = Iy-1;
			T.Iy_C = Iy;
		}
		if(T.Iy_A < 0){T.Iy_A = 0; T.Iy_C = 1;}
		if(T.Iy_C > grid.y_point_count-1){T.Iy_C = grid.y_point_count-1; T.Iy_A = T.Iy_C-1;}
		T.Iy_B = T.Iy_A;
		T.Iy_D = T.Iy_C;

		return T;
	};

	//getCollisionNormal() - returns the normal at point p by sampling the gradient
	//						 of the tile T a small distance away from the point.
	//						 theoretically, should be more stable than just
	//						 sampling at point, because it's undefined (unstable)
	//						 whether the point is on the right or left side of the
	//						 tile diagonal.
	auto getCollisionNormal = [&](tile T, vec3 p, vec3 pendir)->vec3{
		//get collision normal; we assume the collision point 'p' is a vertex,
		//so we move a little bit away from it along the penetration direction 'pendir'
		//to make sure we are on the right triangle, and then sample the normal there.
		vec3 small_offset = pendir*(min(grid.x_step,grid.y_step)*0.01f);
		//float sample_x = grid.x_step/2.f; //idk, this is where using vertices as collision points breaks down
		//float sample_y = grid.y_step/2.f;
		vec3 pos = p+small_offset;
		vec2 gradient = T.gradient({pos.x,pos.y}); //let the tile decide on how to get the gradient
		//bilinearGradient(T.yA(),T.yB(),T.yC(),T.yD(),dx,dy,sample_x,sample_y);
		vec3 norm = normalizeSafe(vec3(gradient.x, gradient.y, 1.f));
		//ci->c_to_c.normal = norm;
		return norm;
	};

	//checkCandidatePoint() - rates candidate collision point p. essentially,
	//						  collisioninfo *ci = best(ci, newci(p)).
	//						  assumes point p is in the box.
	auto checkCandidatePoint = [&](vec3 p, tile T)->bool
	{
		float height = p.z - b_z1;
		string CCreport = "\n";
		if(!ci){ //init CI if we haven't already
			ci = new collisioninfo();
			ci->body1 = body1;
			ci->body2 = body2;
			ci->c_to_c.pos = p;
			ci->c_to_c.depth = height;
			ci->c_to_c.penetration = pendir;
			ci->c_to_c.normal = -vec3(0,0,1);
		}

		if(RateOnlyHeight){
			float updot = pendir.z;
			float depth = height/updot;	//pen.dot(normal) should be == depth in 'normal' direction
			if(depth > bestscore){
				bestscore = depth;
				ci->c_to_c.pos = p;
				ci->c_to_c.depth = depth;
				ci->c_to_c.penetration = pendir*depth;
				vec3 normal = getCollisionNormal(T,p,pendir);
				ci->c_to_c.normal = normal;//normal always up;
				CCreport += fstring("(only height) col OK\ndepth = %f\npen = %s\nnorm = %s\n",depth,toCString(pendir*depth),toCString(normal));
				debugFloatingText(p, CCreport);
				return true;
			}else{
				CCreport += fstring("not the best (this %f <= best %f)\n",depth,bestscore);
				debugFloatingText(p, CCreport);
				return false;
			}
		}else{
			//tile penTile = getTileFromDir(pendir);
			//vec3 normal = getCollisionNormal(tile);
			if(cray){delete cray;}
			ray.from = p;
			ray.dir = pendir;
			cray = checkCollisionRay_AABB(&ray, body1);
			//the ray should always hit the box
			//unless we are separating... but that shouldn't happen?
			if(!cray){
				//error("AABB-Terrain: don't call me on separating collision\n"); return false;
				//separating collision, let's treat it as a continuation of a previous collision in this direction
				//printf("warning: separating collision\n");
				CCreport += "warning: separating collision\n";
				//note: may be separating for some points, but not others
				pendir = -pendir;
				ray.dir = pendir;
				cray = checkCollisionRay_AABB(&ray,body1);
				if(!cray){
					//printf("warning: no collision ray hit\n");
					CCreport += "warning: no collision ray hit\n";
					debugFloatingText(p, CCreport);
					return false;
				}

			}
			CCreport += fstring("CRAY = %s\n",toCString(cray));
			float depth = cray->c_to_c.depth;
			
			if(depth > bestscore){
				bestscore = depth;
				vec3 normal = getCollisionNormal(T,p,pendir);
				ci->c_to_c.pos = p;
				ci->c_to_c.depth = depth;
				ci->c_to_c.penetration = pendir*depth;
				ci->c_to_c.normal = normal;
				CCreport += fstring("col OK\ndepth = %f\npen = %s\nnorm = %s\n",depth,toCString(pendir*depth),toCString(normal));
				debugFloatingText(p, CCreport);
				delete cray;
				return true;
			}else{
				//CCreport += fstring("not the best (this %f <= best %f)\n",depth,bestscore);
				//CCreport += fstring("pos = %s, normal = %s, pen = %s\n", toCString(cray->c_to_c.pos), toCString(cray->c_to_c.normal), toCString(cray->c_to_c.penetration));
				debugFloatingText(p, CCreport);
				delete cray;
				return false;
			}
		}
		assert(false);//return false;
	};
	//----------------------------------------- vertex loop------------------------------------------------
	//--------- inner tiles ------------------
	for(int Ix = g_x1_in_idx; Ix <= g_x2_in_idx; Ix++){
		for(int Iy = g_y1_in_idx; Iy <= g_y2_in_idx; Iy++){
			//it's entirely possible there will be no such tiles. but if there are:
			float t_z = grid[Ix][Iy]+offs.z;
			if(t_z > b_z1){
				hasCollision = true;
				inside_point_count++;
				//we got a candidate point!
				//init CI if we haven't already

				vec3 point = grid.point(Ix,Iy)+offs;//vec3(grid_x1+Ix*grid.y_step, grid_y1+Iy*grid.y_step, t_z);

				debugDrawPoint(point,vec3(0,255,0));

				if(!ci){ //init CI if we haven't already
					ci = new collisioninfo();
					ci->body1 = body1;
					ci->body2 = body2;
					ci->c_to_c.pos = point;
					ci->c_to_c.depth = t_z-b_z1;
					ci->c_to_c.penetration = vec3(0,0,1)*(t_z-b_z1);
					ci->c_to_c.normal = -vec3(0,0,1);
				}

				//this chunk of code rates T.v-in-B contact points
				assert(grid.x_tile_count > 0); //we must have at least one tile
				assert(grid.y_tile_count > 0);


				tile T = getTileFromDir(pendir,Ix,Iy);
				checkCandidatePoint(point,T);
			}
		}
	}
	//----- outer tiles ----------
	printf("\n--------snap-------\n");
	for(int Ix1 = g_x1_out_idx; Ix1 < g_x2_out_idx; Ix1++){
		//printf("left\n");
		//left tiles: y1 < y
		{
			int Ix2 = Ix1+1;
			int Iy1 = g_y1_out_idx;
			int Iy2 = g_y1_in_idx;

			float x = b_x1-offs.x;
			float y = b_y1-offs.y;
			debugDrawPoint(vec3(x,y,b_z1-offs.z)+offs,vec3(255,255,255));
			tile T;
			T.grid = &grid;
			T.Ix_A = Ix1; T.Iy_A = Iy1;
			T.Ix_B = Ix2; T.Iy_B = Iy1;
			T.Ix_C = Ix1; T.Iy_C = Iy2;
			T.Ix_D = Ix2; T.Iy_D = Iy2;

			//point1: x1,y (if inside) //x1_out always outside, but try [x1_in,y] next
			//point2: xdiag,y (if inside) ... or something
			//point3: x,y (only for the first/last if inside)
			vec3 gp1 = grid.point(Ix1,Iy1);
			vec3 gp2 = grid.point(Ix2,Iy1); //was Ix2,Iy2
			if(Ix1 != g_x1_out_idx){
				//middle grid points
				vec3 p1 = vec3(gp1.x,y,T.height(vec2(gp1.x,y)))+offs;
				if(inBox(p1)){
					debugDrawPoint(p1,vec3(0,255,0));
					checkCandidatePoint(p1,T);
				}else{
					debugDrawPoint(p1,vec3(128,128,128));
				}
				debugDrawLine(p1,gp1+offs,vec3(255,0,0));
				debugFloatingText(p1,fstring("p1-%s (%s)",toCString(p1), inBox(p1) ? "in" : "out"));
			}

			if(Ix1 == g_x1_out_idx){	//yellow
				//box first corner
				x = b_x1-offs.x;
				vec3 p3 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p3)){
					debugDrawPoint(p3,vec3(0,255,0));
					checkCandidatePoint(p3,T);
				}else{
					debugDrawPoint(p3,vec3(128,128,128));
				}
				debugDrawLine(p3,gp1+offs,vec3(255,255,0));
				debugFloatingText(p3,fstring("p3-%s (%s)",toCString(p3), inBox(p3) ? "in" : "out"));

			}else if(Ix2 == g_x2_out_idx){	//purple
				//box last corner
				x = b_x2-offs.x;
				vec3 p4 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p4)){
					debugDrawPoint(p4,vec3(0,255,0));
					checkCandidatePoint(p4,T);
				}else{
					debugDrawPoint(p4,vec3(128,128,128));
				}
				debugDrawLine(p4,gp2+offs,vec3(255,0,255));
				debugFloatingText(p4,fstring("p4-%s (%s)",toCString(p4), inBox(p4) ? "in" : "out"));
			}

			//diagonal (off-grid) points
			vec2 bp1 = vec2(gp1.x,y);
			vec2 bp2 = vec2(gp2.x,y);
			vec3 p5 = T.getDiagPointOnLine(bp1,bp2)+offs;
			if(inBox(p5)){
				debugDrawPoint(p5,vec3(0,255,0));
				checkCandidatePoint(p5,T);
			}else{
				debugDrawPoint(p5,vec3(128,128,128));
			}
			debugDrawLine(p5,toVec3(bp1,T.height(bp1))+offs,vec3(128,255,128)); //lime
			debugFloatingText(p5,fstring("p5-%s (%s)",toCString(p5), inBox(p5) ? "in" : "out"));
		}
		//------------------------------------------------------------------------------------
		//printf("right\n");
		//right tiles: y < y2
		{
			int Ix2 = Ix1+1;
			int Iy1 = g_y2_in_idx;
			int Iy2 = g_y2_out_idx;

			float x = b_x1-offs.x;
			float y = b_y2-offs.y;
			debugDrawPoint(vec3(x,y,b_z1-offs.z)+offs,vec3(255,255,255));
			tile T;
			T.grid = &grid;
			T.Ix_A = Ix1; T.Iy_A = Iy1;
			T.Ix_B = Ix2; T.Iy_B = Iy1;
			T.Ix_C = Ix1; T.Iy_C = Iy2;
			T.Ix_D = Ix2; T.Iy_D = Iy2;

			//point1: x1,y (if inside) //x1_out always outside, but try [x1_in,y] next
			//point2: xdiag,y (if inside) ... or something
			//point3: x,y (only for the first/last if inside)
			vec3 gp1 = grid.point(Ix1,Iy1);
			vec3 gp2 = grid.point(Ix2,Iy1); //was Ix2,Iy2
			if(Ix1 != g_x1_out_idx){
				//middle grid points
				vec3 p1 = vec3(gp1.x,y,T.height(vec2(gp1.x,y)))+offs;
				if(inBox(p1)){
					debugDrawPoint(p1,vec3(0,255,0));
					checkCandidatePoint(p1,T);
				}else{
					debugDrawPoint(p1,vec3(128,128,128));
				}
				debugDrawLine(p1,gp1+offs,vec3(255,0,0));
				debugFloatingText(p1,fstring("p1-%s (%s)",toCString(p1), inBox(p1) ? "in" : "out"));
			}

			if(Ix1 == g_x1_out_idx){	//yellow
				//box first corner
				x = b_x1-offs.x;
				vec3 p3 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p3)){
					debugDrawPoint(p3,vec3(0,255,0));
					checkCandidatePoint(p3,T);
				}else{
					debugDrawPoint(p3,vec3(128,128,128));
				}
				debugDrawLine(p3,gp1+offs,vec3(255,255,0));
				debugFloatingText(p3,fstring("p3-%s (%s)",toCString(p3), inBox(p3) ? "in" : "out"));

			}else if(Ix2 == g_x2_out_idx){	//purple
				//box last corner
				x = b_x2-offs.x;
				vec3 p4 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p4)){
					debugDrawPoint(p4,vec3(0,255,0));
					checkCandidatePoint(p4,T);
				}else{
					debugDrawPoint(p4,vec3(128,128,128));
				}
				debugDrawLine(p4,gp2+offs,vec3(255,0,255));
				debugFloatingText(p4,fstring("p4-%s (%s)",toCString(p4), inBox(p4) ? "in" : "out"));
			}

			//diagonal (off-grid) points
			vec2 bp1 = vec2(gp1.x,y);
			vec2 bp2 = vec2(gp2.x,y);
			vec3 p5 = T.getDiagPointOnLine(bp1,bp2)+offs;
			if(inBox(p5)){
				debugDrawPoint(p5,vec3(0,255,0));
				checkCandidatePoint(p5,T);
			}else{
				debugDrawPoint(p5,vec3(128,128,128));
			}
			debugDrawLine(p5,toVec3(bp1,T.height(bp1))+offs,vec3(128,255,128)); //lime
			debugFloatingText(p5,fstring("p5-%s (%s)",toCString(p5), inBox(p5) ? "in" : "out"));
		}
	}
	//---------------------------------------- other two sides
	for(int Iy1 = g_y1_out_idx; Iy1 < g_y2_out_idx; Iy1++){
		//top tiles: x1 > x
		//printf("top\n");
		{
			int Iy2 = Iy1+1;
			int Ix1 = g_x1_out_idx;
			int Ix2 = g_x1_in_idx;

			float y = b_y1-offs.y;
			float x = b_x1-offs.x;
			debugDrawPoint(vec3(x,y,b_z1-offs.z)+offs,vec3(255,255,255));
			tile T;
			T.grid = &grid;
			T.Ix_A = Ix1; T.Iy_A = Iy1;
			T.Ix_B = Ix2; T.Iy_B = Iy1;
			T.Ix_C = Ix1; T.Iy_C = Iy2;
			T.Ix_D = Ix2; T.Iy_D = Iy2;

			//point1: x1,y (if inside) //x1_out always outside, but try [x1_in,y] next
			//point2: xdiag,y (if inside) ... or something
			//point3: x,y (only for the first/last if inside)
			vec3 gp1 = grid.point(Ix1,Iy1);
			vec3 gp2 = grid.point(Ix1,Iy2); //was Ix2,Iy2
			if(Iy1 != g_y1_out_idx){
				//middle grid points
				vec3 p1 = vec3(x,gp1.y,T.height(vec2(x,gp1.y)))+offs;
				if(inBox(p1)){
					debugDrawPoint(p1,vec3(0,255,0));
					checkCandidatePoint(p1,T);
				}else{
					debugDrawPoint(p1,vec3(128,128,128));
				}
				debugDrawLine(p1,gp1+offs,vec3(255,0,0));
				debugFloatingText(p1,fstring("p1-%s (%s)",toCString(p1), inBox(p1) ? "in" : "out"));
			}

			if(Iy1 == g_y1_out_idx){	//yellow
				//box first corner
				y = b_y1-offs.y;
				vec3 p3 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p3)){
					debugDrawPoint(p3,vec3(0,255,0));
					checkCandidatePoint(p3,T);
				}else{
					debugDrawPoint(p3,vec3(128,128,128));
				}
				debugDrawLine(p3,gp1+offs,vec3(255,255,0));
				debugFloatingText(p3,fstring("p3-%s (%s)",toCString(p3), inBox(p3) ? "in" : "out"));

			}else if(Iy2 == g_y2_out_idx){	//purple
				//box last corner
				y = b_y2-offs.y;
				vec3 p4 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p4)){
					debugDrawPoint(p4,vec3(0,255,0));
					checkCandidatePoint(p4,T);
				}else{
					debugDrawPoint(p4,vec3(128,128,128));
				}
				debugDrawLine(p4,gp2+offs,vec3(255,0,255));
				debugFloatingText(p4,fstring("p4-%s (%s)",toCString(p4), inBox(p4) ? "in" : "out"));
			}

			//diagonal (off-grid) points
			vec2 bp1 = vec2(x,gp1.y);
			vec2 bp2 = vec2(x,gp2.y);
			vec3 p5 = T.getDiagPointOnLine(bp1,bp2)+offs;
			if(inBox(p5)){
				debugDrawPoint(p5,vec3(0,255,0));
				checkCandidatePoint(p5,T);
			}else{
				debugDrawPoint(p5,vec3(128,128,128));
			}
			debugDrawLine(p5,toVec3(bp1,T.height(bp1))+offs,vec3(128,255,128)); //lime
			debugFloatingText(p5,fstring("p5-%s (%s)",toCString(p5), inBox(p5) ? "in" : "out"));
		}
		//------------------------------------------------------------------------------------
		//printf("bottom\n");
		//bottom tiles: x < x2
		{
			int Iy2 = Iy1+1;
			int Ix1 = g_x2_in_idx;
			int Ix2 = g_x2_out_idx;

			float y = b_y1-offs.y;
			float x = b_x2-offs.x;
			debugDrawPoint(vec3(x,y,b_z1-offs.z)+offs,vec3(255,255,255));
			tile T;
			T.grid = &grid;
			T.Ix_A = Ix1; T.Iy_A = Iy1;
			T.Ix_B = Ix2; T.Iy_B = Iy1;
			T.Ix_C = Ix1; T.Iy_C = Iy2;
			T.Ix_D = Ix2; T.Iy_D = Iy2;

			//point1: x1,y (if inside) //x1_out always outside, but try [x1_in,y] next
			//point2: xdiag,y (if inside) ... or something
			//point3: x,y (only for the first/last if inside)
			vec3 gp1 = grid.point(Ix1,Iy1);
			vec3 gp2 = grid.point(Ix1,Iy2); //was Ix2,Iy2
			if(Iy1 != g_y1_out_idx){
				//middle grid points
				vec3 p1 = vec3(x,gp1.y,T.height(vec2(x,gp1.y)))+offs;
				if(inBox(p1)){
					debugDrawPoint(p1,vec3(0,255,0));
					checkCandidatePoint(p1,T);
				}else{
					debugDrawPoint(p1,vec3(128,128,128));
				}
				debugDrawLine(p1,gp1+offs,vec3(255,0,0));
				debugFloatingText(p1,fstring("p1-%s (%s)",toCString(p1), inBox(p1) ? "in" : "out"));
			}

			if(Iy1 == g_y1_out_idx){	//yellow
				//box first corner
				y = b_y1-offs.y;
				vec3 p3 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p3)){
					debugDrawPoint(p3,vec3(0,255,0));
					checkCandidatePoint(p3,T);
				}else{
					debugDrawPoint(p3,vec3(128,128,128));
				}
				debugDrawLine(p3,gp1+offs,vec3(255,255,0));
				debugFloatingText(p3,fstring("p3-%s (%s)",toCString(p3), inBox(p3) ? "in" : "out"));

			}else if(Iy2 == g_y2_out_idx){	//purple
				//box last corner
				y = b_y2-offs.y;
				vec3 p4 = vec3(x,y,T.height(vec2(x,y)))+offs;
				if(inBox(p4)){
					debugDrawPoint(p4,vec3(0,255,0));
					checkCandidatePoint(p4,T);
				}else{
					debugDrawPoint(p4,vec3(128,128,128));
				}
				debugDrawLine(p4,gp2+offs,vec3(255,0,255));
				debugFloatingText(p4,fstring("p4-%s (%s)",toCString(p4), inBox(p4) ? "in" : "out"));
			}

			//diagonal (off-grid) points
			vec2 bp1 = vec2(x,gp1.y);
			vec2 bp2 = vec2(x,gp2.y);
			vec3 p5 = T.getDiagPointOnLine(bp1,bp2)+offs;
			if(inBox(p5)){
				debugDrawPoint(p5,vec3(0,255,0));
				checkCandidatePoint(p5,T);
			}else{
				debugDrawPoint(p5,vec3(128,128,128));
			}
			debugDrawLine(p5,toVec3(bp1,T.height(bp1))+offs,vec3(128,255,128)); //lime
			debugFloatingText(p5,fstring("p5-%s (%s)",toCString(p5), inBox(p5) ? "in" : "out"));
		}
	}




	if(hasCollision){
		debugDrawPoint(ci->c_to_c.pos,vec3(0,255,255));
		if(!debugStop)
		printf("inside points = %d\n",inside_point_count);
	}
	//5.2 check verts for tiles that are on the edge (sample box/grid intersections)

	if(hasCollision){
		//STOP! Debug time
		//body1->vel = vec3(0,0,0);
		//debugStop = true;
	}
	return ci;
};
*/

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
			//printf("+");
			collisioninfo *col = new collisioninfo();
			col->body1 = body1;
			col->body2 = body2;
			col->c_to_c.pos = bestp+body2->pos;
			col->c_to_c.normal = bestnormal;
			col->c_to_c.penetration = vec3(0,0,0);
			col->c_to_c.depth = mindist;
			return col;
		}else{
			//printf("-");
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
		col->c_to_c.normal = sideNormals[nearestSide(aabb, body1->pos - body2->pos)];
        return col;
    }
    return 0;
}

collisioninfo *checkCollisionAABB_Plane(collisionbody *body1, collisionbodyPlane *body2){
	vec3 n = body2->normal;
	float offset = body2->offset;
	//printf("ccAPl: n = %s, offs = %f\n", toCString(n), offset);
	
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
		//printf("pt %s, dot = %f, depth = %f\n",toCString(p), (float)(dot(p,n)),depth);
		if(depth > bestDepth){bestDepth = depth; bestI = i;}
	}
	//printf("bestDepth = %f\n",bestDepth);
	if(bestDepth > 0){
		vec3 pos = pts[bestI];
		collisioninfo *col = new collisioninfo();
		col->body1 = body1;
		col->body2 = body2;
		col->c_to_c.pos = pos;
		col->c_to_c.normal = n;
		col->c_to_c.depth = bestDepth;
		col->c_to_c.penetration = -bestDepth*n;
		//printf("ccAPl: yes\n");
		return col;
	}
	//printf("ccAPl: no\n");
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
		//printf("ccRPl: yes\n");
		return col;
	}
	//printf("ccRPl: no\n");
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
		//printf("ccPtPl: yes\n");
		return col;
	}
	//printf("ccPtPl: no\n");
	return 0;
}

/*
collisioninfo *checkCollisionPoint_Terrain(collisionbody *body1, collisionbody *body2){
    collisioninfo *col = checkCollisionPoint_AABB(body1, body2);
    if(!col){return 0;}
	vec3 p = body1->pos-body2->pos;
	collisionbodyTerrain *body2Terrain = static_cast<collisionbodyTerrain*>(body2);
	if(body2Terrain->grid.height(vec2(p.x,p.y)) >= p.z){
        return col;
	}else{
        return 0;
	}
}
*/
//The separating axis that realizes the axis of minimum penetration is
//either a face normal or the cross product between two edges.

collisioninfo *checkCollisionSAT(collisionbody *body1, collisionbody *body2){
/*	vector<vec3> normals;
	vector<vec3> points1;
	vector<vec3> points2;
	getCollisionbodyNormals(normals,body1);
	getCollisionbodyNormals(normals,body2);
	if(normals.size() == 0){return 0;}
	getCollisionbodyPoints(points1,body1);
	if(points1.size() == 0){return 0;}
	getCollisionbodyPoints(points2,body2);
	if(points2.size() == 0){return 0;}

	float max1=min1=points1[0].dot(normals[0]);
	float max2=min2=points2[0].dot(normals[0]);
	for(auto I = normals.begin(); I != normals.end(); I++){
		float p;	//projected point
		//float p1;	//point1 of intersection interval
		//float p2;	//point2 of intersection interval
		for(auto J = points1.begin(); J != points1.end(); J++){
			p = (*J).dot(*I);
			if(p > max1){max1 = p;}
			if(p < min1){min1 = p;}
		}
		for(auto J = points2.begin(); J != points2.end(); J++){
			p = (*J).dot(*I);
			if(p > max2){max2 = p;}
			if(p < min2){min2 = p;}
		}
		if((min1 > max2) || (min2 > max1)){
			return 0;	//separating axis found -> there is no collision
		}

	}
	return true; //figure out the collision points and separation, might need dynamic case
*/
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

int numCollisionPairs = 0;//entity *I, entity *J,
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2, collisionOptions &options){
	if(!body1){printf("pcc: no body1\n"); return;}
	if(!body2){printf("pcc: no body2\n"); return;}
	numCollisionPairs++;
	if(!canCollide(body1,body2)){return;}
	collisioninfo *col = collisionCheckDispatch(body1,body2);
	if(!col){
		//printf("no collision\n\n");
		return;
	}else{
		//printCollision(col);
	}
	//printf("collision!\n");
	if((body1->type != BODY_TRIGGER) && (body2->type != BODY_TRIGGER)){
        if(options.separate){separateCollision(col);}
        if(options.resolve){resolveCollision(col);}
	}
	col->body1->ov->moveTo(body1->pos);
	col->body2->ov->moveTo(body2->pos);
}
