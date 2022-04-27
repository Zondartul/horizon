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
#include "broadphase.h"
#include "narrowphase.h"
#include "resolution.h"
#include "modelprimitives.h"	//for debug drawing
#include "editmodel.h"
#include "geometry.h"

//extern octree_node *g_octree_root;

extern void debugDrawPoint(vec3 point, vec3 col);


string toString(collisionbodytype type){
	switch(type){
		case BODY_STATIC: return "BODY_STATIC"; break;
		case BODY_DYNAMIC: return "BODY_DYNAMIC"; break;
		case BODY_TRIGGER: return "BODY_TRIGGER"; break;
		case BODY_NOCOLLIDE: return "BODY_NOCOLLIDE"; break;
		default: return "ERROR"; break;
	}
}

collisionbody::collisionbody(){
	ov = new octree_visitor(g_octree_root, this, pos);
}
collisionbody::~collisionbody(){
	delete ov;
}
void collisionbody::render(renderOptions *options){
	vec3 offset = (aabb.end+aabb.start)/2.f;
	setPosition(pos+offset);
	setScale(vec3(1,1,1));
	setRotation(vec3(0,0,0));
	drawRmodel(rm);
}
string collisionbody::name(){
	string S = "";
	if(E){
		if(E->name == ""){
			S = "<anon>.";
		}else{
			S = E->name+".";
		}
	}
	if(bodyname == ""){
		S = S + "body";
	}else{
		S = S + bodyname;
	}
	return S;
}
void collisionbody::setAABB(AABB aabb){
    this->aabb = aabb;
    if(rm){setLayer(g_deleteLayer); deleteRmodel(rm); rm = 0;}
    if(aabb.size != vec3(0,0,0)){rm = generateBox(aabb.size)->getRmodel(1);}
}

AABB collisionbody::getAABB(){return aabb;}

collisionbodyAABB::collisionbodyAABB(AABB aabb){setAABB(aabb);}
collisionbodyRay::collisionbodyRay(vec3 from, vec3 dir){this->from = from; this->dir = dir;}

float& row_acessor::operator[](int Iy){return (*grid)[column+Iy*row_size];}
row_acessor gridKind::operator[](int Ix){return {&height_grid,Ix,x_point_count};}
vec3 gridKind::point(int Ix, int Iy){return vec3(Ix*x_step,Iy*y_step,(*this)[Ix][Iy]);}


vec3 tile::vA(){return vec3(Ix_A*grid->x_step,Iy_A*grid->y_step, yA());}
vec3 tile::vB(){return vec3(Ix_B*grid->x_step,Iy_B*grid->y_step, yB());}
vec3 tile::vC(){return vec3(Ix_C*grid->x_step,Iy_C*grid->y_step, yC());}
vec3 tile::vD(){return vec3(Ix_D*grid->x_step,Iy_D*grid->y_step, yD());}

float tile::yA(){return (*grid)[Ix_A][Iy_A];}
float tile::yB(){return (*grid)[Ix_B][Iy_B];}
float tile::yC(){return (*grid)[Ix_C][Iy_C];}
float tile::yD(){return (*grid)[Ix_D][Iy_D];}


char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}

vec3 tile::getDiagPointOnLine(vec2 p1, vec2 p2){
	vec3 p3;
	vec3 p12;
	float p1h = height(vec2(p1.x,p1.y));
	float p2h = height(vec2(p2.x,p2.y));
	if(p1h < p2h){p12 = vec3(p2.x,p2.y,p2h);}else{p12 = vec3(p1.x,p1.y,p1h);}

	//float u;// , t;
	vec2 diagStart, diagDir, diagEnd, segStart, segDir, segEnd,res;
	switch(shape){
		case TILE_DIAG:
TILE_DIAG_LABEL:
				diagStart = vB();
				diagEnd = vC();
				if(get_line_intersection(
					p1.x,p1.y,
					p2.x,p2.y,
					diagStart.x,diagStart.y,
					diagEnd.x,diagEnd.y,
					&res.x,&res.y)){
					p3 = toVec3(res,height(res));

					return p3; //for debug
				}
			break;
		case TILE_ALTERNATING_DIAG:
			if(((Ix_A+Iy_A) % 2) == 0){
				goto TILE_DIAG_LABEL;
			}else{
				diagStart = vA();
				diagEnd = vD();
				if(get_line_intersection(
					p1.x,p1.y,
					p2.x,p2.y,
					diagStart.x,diagStart.y,
					diagEnd.x,diagEnd.y,
					&res.x,&res.y)){
					p3 = toVec3(res,height(res));
					return p3; //for debug
				}
			}
			break;
		case TILE_BILINEAR:
			//no diagonal here
			//but a quadratic equation
			//x = (pos.x-A.x)/(B.x-A.x);
			//y = (pos.y-A.y)/(C.y-A.y);
			//FUCK IT
		break;
	};
	printf("fallback 1/2\n");
	return p12;
}
//struct v_tri{vec3 A,B,C};

float diagonalTileHeight(vec3 A, vec3 B, vec3 C, vec3 D, vec2 pos, bool flip = false){
	//normalize to a square tile
	vec2 size = vec2(B.x-A.x,C.y-A.y);
	B.x = (B.x-A.x)/size.x;
	B.y = (B.y-A.y)/size.y;
	C.x = (C.x-A.x)/size.x;
	C.y = (C.y-A.y)/size.y;
	D.x = (D.x-A.x)/size.x;
	D.y = (D.y-A.y)/size.y;
	pos.x = (pos.x-A.x)/size.x;
	pos.y = (pos.y-A.y)/size.y;

	float zA, zB, zC, zD;
	//bool tri1;
	float dz_dx, dz_dy;
	if(!flip){
		//now we have coords:
		// (0,0,zA)------------(1,0,zB)
		//    |                 / |
		//	  |      *        /   |
		//	  |(p.x,p.y,h)  /     |
		//	  |           /       |
		//	  |  1      /         |
		//	  |       /        2  |
		//	  |     /             |
		//	  |   /               |
		//	  | /                 |
		// (0,1,zC)------------(1,1,zD)
		if(pos.x <= 1.f-pos.y){	//plane of triangle 1 (A,B,C)
			zA = A.z;
			zB = B.z;
			zC = C.z;
			dz_dx = (zB-zA);
			zD = zC + dz_dx; //equivalent
		}else{	//plane of triangle 2 (B,D,C)
			zB = B.z;
			zC = C.z;
			zD = D.z;
			dz_dy = (zD-zB);
			zA = zC - dz_dy; //equivalent
		}
	}else{
		//now we have coords:
		// (0,0,zA)------------(1,0,zB)
		//    | \                 |
		//	  |   \           *   |
		//	  |     \ (p.x,p.y,h) |
		//	  |       \           |
		//	  |         \     1   |
		//	  |           \       |
		//	  |    2        \     |
		//	  |               \   |
		//	  |                 \ |
		// (0,1,zC)------------(1,1,zD)
		if(pos.x <= pos.y){	//plane of triangle 1 (A,B,D)
			zA = A.z;
			zB = B.z;
			zD = D.z;
			dz_dy = (zD-zB);
			zC = zA + dz_dy; //equivalent
		}else{	//plane of triangle 2 (A,D,C)
			zA = A.z;
			zC = C.z;
			zD = D.z;
			dz_dx = (zD-zC);
			zB = zA+dz_dx; // equivalent
		}
	}
	//the bilinear formula can be used for flat sheets too
	float h = (zA*(1-pos.x)+zB*pos.x)*(1-pos.y)+(zC*(1-pos.x)+zD*pos.x)*pos.y;
	return h;
}

#define FLIP_DIAGONAL true
vec2 diagonalTileGradient(vec3 A, vec3 B, vec3 C, vec3 D, vec2 pos, bool flip = false){
	//assumption: tile is rectangular and axis-aligned.
	//normalize to a square tile
	vec2 size = vec2(B.x-A.x,C.y-A.y);
	B.x = (B.x-A.x)/size.x;
	B.y = (B.y-A.y)/size.y;
	C.x = (C.x-A.x)/size.x;
	C.y = (C.y-A.y)/size.y;
	D.x = (D.x-A.x)/size.x;
	D.y = (D.y-A.y)/size.y;
	pos.x = (pos.x-A.x)/size.x;
	pos.y = (pos.y-A.y)/size.y;
	A.x = 0; A.y = 0;

	float zA, zB, zC, zD;
	//bool tri1;
	float dz_dx, dz_dy;
	if(!flip){
		//now we have coords:
		// (0,0,zA)------------(1,0,zB)
		//    |                 / |
		//	  |      *        /   |
		//	  |(p.x,p.y,h)  /     |
		//	  |           /       |
		//	  |         /         |
		//	  |       /           |
		//	  |     /             |
		//	  |   /               |
		//	  | /                 |
		// (0,1,zC)------------(1,1,zD)
		if(pos.x <= 1.f-pos.y){	//plane of triangle 1 (A,B,C)
			zA = A.z;
			zB = B.z;
			zC = C.z;
			dz_dx = (zB-zA);
			dz_dy = (zC-zA);
		}else{	//plane of triangle 2 (B,D,C)
			zB = B.z;
			zC = C.z;
			zD = D.z;
			dz_dx = (zD-zC);
			dz_dy = (zD-zB);
		}
	}else{
		//now we have coords:
		// (0,0,zA)------------(1,0,zB)
		//    | \                 |
		//	  |   \           *   |
		//	  |     \ (p.x,p.y,h) |
		//	  |       \           |
		//	  |         \         |
		//	  |           \       |
		//	  |             \     |
		//	  |               \   |
		//	  |                 \ |
		// (0,1,zC)------------(1,1,zD)
		if(pos.x <= pos.y){	//plane of triangle 1 (A,B,D)
			zA = A.z;
			zB = B.z;
			zD = D.z;
			dz_dx = (zB-zA);
			dz_dy = (zD-zB);
		}else{	//plane of triangle 2 (A,D,C)
			zA = A.z;
			zC = C.z;
			zD = D.z;
			dz_dx = (zD-zC);
			dz_dy = (zC-zA);
		}
	}
	//todo: reverse the normalization
	return vec2(dz_dx/size.x,dz_dy/size.y);
}

float bilinearTileHeight(vec3 A, vec3 B, vec3 C, vec3 D, vec2 pos){
	float x = (pos.x-A.x)/(B.x-A.x);
	float y = (pos.y-A.y)/(C.y-A.y);
	float h = (A.z*(1-x)+B.z*x)*(1-y)+(C.z*(1-x)+D.z*x)*y;
	return h;
}

vec2 bilinearTileGradient(vec3 A, vec3 B, vec3 C, vec3 D, vec2 pos){
	float x = (pos.x-A.x)/(B.x-A.x);
	float y = (pos.y-A.y)/(C.y-A.y);
	/*
	normals:

	zA
	- zA*x + zB*x
	- zA*y + Zc*y
	+ zA*x*y - zB*x*y - Zc*x*y + zD*x*y

	dz/dx = - zA + zB + zA*y - zB*y - zC*y + zD*y
	dz/dy = - zA + zC + zA*x - zB*x - zC*x + zD*x
	*/
	float dh_dx = -A.z+B.z+A.z*y - B.z*y - C.z*y + D.z*y;
	float dh_dy = -A.z+C.z+A.z*x - B.z*x - C.z*x + D.z*x;
	return vec2(dh_dx, dh_dy);
}

float tile::height(vec2 pos){
	if(!grid){return 0;}
	float h = 0;
	vec3 A = vA();
	vec3 B = vB();
	vec3 C = vC();
	vec3 D = vD();
	switch(shape){
		case TILE_DIAG:
			h = diagonalTileHeight(A,B,C,D,pos);
			break;
		case TILE_ALTERNATING_DIAG:
			if(((Ix_A+Iy_A) % 2) == 0){
				h = diagonalTileHeight(A,B,C,D,pos);
			}else{
				//if we are on a diagonal tile, flip the diagonal
				h = diagonalTileHeight(A,B,C,D,pos, FLIP_DIAGONAL);
			}
			break;
		case TILE_BILINEAR:
			h = bilinearTileHeight(A,B,C,D,pos);
			break;
	}
	return h;
}
vec2 tile::gradient(vec2 pos){
	if(!grid){return vec2();}
	vec2 g;
	vec3 A = vA();
	vec3 B = vB();
	vec3 C = vC();
	vec3 D = vD();
	switch(shape){
		case TILE_DIAG:
			g = diagonalTileGradient(A,B,C,D,pos);
			break;
		case TILE_ALTERNATING_DIAG:
			if(((Ix_A+Iy_A) % 2) == 0){
				g = diagonalTileGradient(A,B,C,D,pos);
			}else{
				//if we are on a diagonal tile, flip the diagonal
				g = diagonalTileGradient(A,B,C,D,pos, FLIP_DIAGONAL);
			}
			break;
		case TILE_BILINEAR:
			g = bilinearTileGradient(A,B,C,D,pos);
			break;
	}
	return g;
}
vec3 tile::normal(vec2 pos){
	if(!grid){return vec3(0,0,0);}
	vec2 g = gradient(pos);
	vec3 A(0,0,0);
	vec3 B(1,0,g.x);
	vec3 C(0,1,g.y);
	vec3 n = -triangle_normal(A,B,C);
	return n;
}

collisionbodyPoint::collisionbodyPoint(){ type = BODY_TRIGGER; }
collisionbodyPoint::collisionbodyPoint(vec3 p)
{
	type = BODY_TRIGGER;
	pos = p;
}

void collisionbodyTerrain::generateGridFromModel(int numx, int numy){
	if(!em){error("no editmodel!"); return;}
	int newsize = em->verts.size();
	grid.height_grid.resize(em->verts.size());
	grid.x_tile_count = numx;
	grid.y_tile_count = numy;
	grid.x_point_count = numx+1;
	grid.y_point_count = numy+1;
	printf("grid resized to (%d) * (%d) = %d verts (actually got %d verts)\n",
		grid.x_point_count, grid.y_point_count, (grid.x_point_count)*(grid.y_point_count),newsize);
	updateGridFromModel();
}

void collisionbodyTerrain::updateGridFromModel(){
	if(!em){error("no editmodel!"); return;}
	printf("udateGrid:\n\told aabb = %s\n",toCString(aabb));

	for(auto V = em->verts.begin(); V != em->verts.end(); V++){
		e_vertex &ev = **V;
		vec3 v = ev.pos;
		if(V == em->verts.begin()){
			aabb.end = aabb.start;
		}
		aabb.end = vec3_max(aabb.end, aabb.start+v);
	}
	aabb.size = aabb.end - aabb.start;
	printf("\tnew aabb = %s\n",toCString(aabb));
	float sizex = aabb.size.x;
	float sizey = aabb.size.y;
	grid.x_step = sizex / (float) (grid.x_tile_count);
	grid.y_step = sizey / (float) (grid.y_tile_count);

	for(auto V = em->verts.begin(); V != em->verts.end(); V++){
		e_vertex &ev = **V;
		vec3 v = ev.pos;
		int Ix = roundi(v.x/(float)grid.x_step);
		int Iy = roundi(v.y/(float)grid.y_step);
		int addr = Ix+Iy*grid.x_point_count;
		grid.height_grid[addr] = v.z;
		grid.debugVerts[addr] = v;
		printf("v %s to [%d,%d]\n",toCString(v),Ix,Iy);
	}
}

collisionbodyPlane::collisionbodyPlane(vec3 n, float offset):normal(n),offset(offset){aabb = AABB(vec3(1,1,1),-vec3(1,1,1));}
collisionbodyPlane::collisionbodyPlane(vec3 A, vec3 B, vec3 C){
	normal = triangle_normal(A,B,C);
	offset = dot(A,normal);
	float s = 100.f;
	aabb = AABB(s*vec3(1,1,1),-s*vec3(1,1,1));
}

tile gridKind::getTile(vec2 pos){
	int X1 = floori(pos.x/x_step);
	int Y1 = floori(pos.y/y_step);
	int X2 = X1+1;
	int Y2 = Y1+1;
	if((X1 < 0) || (X2 > (x_point_count-1))){return {0,TILE_DIAG,0,0,0,0,0,0,0,0};}
	if((Y1 < 0) || (Y2 > (y_point_count-1))){return {0,TILE_DIAG,0,0,0,0,0,0,0,0};}
	tile T = {this,TILE_DIAG,X1,Y1,X2,Y1,X1,Y2,X2,Y2};
	return T;
}

float gridKind::height(vec2 pos){
	return getTile(pos).height(pos);
}

vec2 gridKind::gradient(vec2 pos){
	return getTile(pos).gradient(pos);
}

vec3 gridKind::normal(vec2 pos){
	return getTile(pos).normal(pos);
}

vec2 gridKind::getEnd(){
	return vec2(x_step*x_tile_count,
				y_step*y_tile_count);
}

gridKind::gridKind(int numx, int numy, float sizex, float sizey){
	height_grid.resize(numx*numy);
	x_step = sizex;
	y_step = sizey;
	x_tile_count = numx;
	y_tile_count = numy;
	x_point_count = numx+1;
	y_point_count = numy+1;
}

void printCollisionBody(collisionbody *body){
	vec3 v = body->vel;
	float vl = length(v);
	float m = body->mass;
	float r = body->restitution;
	float f = body->friction;
	printf("vel = %s (vl = %f)\n",toCString(v),vl);
	printf("mass = %f\n",m);
	printf("restitution = %f\n",r);
	printf("friction = %f\n",f);
}

void printCollision(collisioninfo *col){
	collisionbody *body1 = col->body1;
	collisionbody *body2 = col->body2;
	string ename1 = body1->E->name;
	string ename2 = body2->E->name;
	string type1 = toString(body1->type);
	string type2 = toString(body2->type);

	printf("------------------\n");
	printf("collision between %s (%s) and %s (%s):\n",
		ename1.c_str(),
		type1.c_str(),
		ename2.c_str(),
		type2.c_str()
		);
	vec3 dv = body1->vel - body2->vel;
	vec3 n = col->c_to_c.normal;
	vec3 dv_perp = glm::proj(dv,n);
	vec3 dv_par = dv-dv_perp;
	printf("dv = %f\n",length(dv));
	printf("dv_perp = %f\n",length(dv_perp));
	printf("dv_par = %f\n",length(dv_par));
	printf("body1:\n");
	printCollisionBody(body1);
	printf("body2:\n");
	printCollisionBody(body2);
	printf("------------------\n\n");
}

//swaps collision parties A and B
//and flips normals
collisioninfo *reverseCollision(collisioninfo *ci){
	if(ci){
		auto body1 = ci->body1;
		ci->body1 = ci->body2;
		ci->body2 = body1;
		ci->c_to_c.normal = -(ci->c_to_c.normal);
		ci->c_to_c.penetration = -(ci->c_to_c.penetration);
	}
	//dunno yet what to do with collision points
	return ci;
}

//-----------------------------------------------------------------------------

collisioninfo *raytrace(vec3 from, vec3 dir,const vector<entity *> &ignore){
	entity *ERay = new entity();
	ERay->body = new collisionbodyRay(from,dir);
	ERay->body->bodyname = "raytrace";
	ERay->body->type = BODY_TRIGGER;
	ERay->name = "raytrace";
	collisioninfo *bestcol = 0;
	for(auto I = g_entities.begin(); I != g_entities.end(); I++){

		bool skip = false;
		for(auto J = ignore.begin(); J != ignore.end(); J++){
			if(*I == *J){skip = true; break;}
		}
		if(skip){continue;}

		entity *E = *I;
		if(E->body){
			collisionbody *body2 = E->body;
			collisioninfo *col = collisionCheckDispatch(ERay->body,body2);
			if(col){
				if(bestcol){
					if(bestcol->c_to_c.depth > col->c_to_c.depth){
						delete bestcol;
						bestcol = col;
					}else{
						delete col;
					}
				}else{
					bestcol = col;
				}
			}
		}
	}
	delete ERay;
	return bestcol;
}


//future collision stuff:
//checkCollision() - is there a collision at this time?
//checkDistanceStatic() - find the closest points between two bodies
//checkDistanceDynamic() - find the point in time when the static distance between
//							two moving bodies would be zero.
//resolveCollision() - compute the resulting velocities and positions for
//						a collision between two moving rigid bodies.











