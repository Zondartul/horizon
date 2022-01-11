#include "physbox.h"
#include "entity.h"
#include "stringUtils.h"
#include "modelPrimitives.h"
#include "rmodel.h"
#include "model.h"
#include "octree.h"
#include "resource.h"
#include "camera.h"
#include "simplemath.h"
#include "editmodel.h"
#include "paint.h"
#include "timer.h"
#include "hook.h"
extern octree_node *octree_root;

//should really figure out texscale on it's own
entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider,bool is_static){
	entity *E = new entity();
	E->name = string()+"box_"+(int)entities.size();
	
	renderable *r = new renderable();
	e_model *em = generateBox(end-start,texscale);
	r->rm = em->getRmodel();
	r->color = vec3(255,255,255);
	r->t = t;
	r->upload();
	E->r = r;
	
	collisionbody *body = new collisionbodyAABB(E->r->rm->toModel()->getAABB());
	body->type = BODY_STATIC;
	body->E = E;
	E->body = body;
	
	
	//body->gravity = vec3(0,0,0);
	//body->mass = 1.0f;
	//body->restitution = 1.0f;
	//body->friction = 1.0f;
	
	E->setPosition((start+end)/2.0f);
	E->setVelocity(vec3(0,0,0));
	E->setGravity(vec3(0,0,0));
	E->setMass(1.0f);
	E->setBouncyness(1.0f);
	E->setFriction(1.0f);
	//if(hascollider){
	//	body->type = BODY_STATIC;
	//}else{
	//	body->type = BODY_NOCOLLIDE;
	//}
	
	return E;
}

void wall(vec3 start, vec3 end,float scale){
	//box(start,end,getTexture("materials/brick2"),1.0f/2.0f);
	entity *E = box(start,end,getTexture("materials/brick2"),scale);
	E->name = string()+"wall_"+(int)entities.size();
}

void floor(vec3 start, vec3 end){
	entity *E = box(start,end,getTexture("materials/brick3"),1/2.0f,true);
	E->name = string()+"floor_"+(int)entities.size();
}

//creates a physbox in front of the camera
void physbox(string tex, float size, float mass, float bouncyness, float friction){
	entity *E = new entity();
	E->name = string()+"physbox_"+(int)entities.size();
	
	renderable *r = new renderable();
	r->rm = generateBox(size*vec3(1,1,1))->getRmodel();
	r->color = vec3(255,255,255);
	r->t = getTexture(tex);
	r->upload();
	E->r = r;
	
	collisionbody *body = new collisionbodyAABB(E->r->rm->toModel()->getAABB());
	E->body = body;
	body->E = E;
	body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
	body->vel = camera.forward()*0.2f;
	body->gravity = vec3(0,0,-0.25);
	body->mass = mass;
	body->restitution = bouncyness;
	body->friction = friction;
	
	body->type = BODY_DYNAMIC;
	
	//ov is handled automatically by collisionbody
	//body->ov = new octree_visitor(octree_root,body,body->pos);
	//ov->curNode = octree_root;
	//ov->body = body;
	//octree_root->addVisitor(ov);
	//ov->moveTo(body->pos);
}

void makeScene1(vec3 offset){
	floor(offset+vec3(-10,-10,-2),offset+vec3(10,10,-1));
	wall(offset+vec3(-6,-3,-1),offset+vec3(-5,10,1));
	wall(offset+vec3(-6,-10,-1),offset+vec3(-5,-4,1));
	
	wall(offset+vec3(-3,-6,-1),offset+vec3(4,-5,1));
	wall(offset+vec3(3,-5,-1),offset+vec3(4,3,1));
	
	wall(offset+vec3(1,-4,-1.0),offset+vec3(2,-3,-0.5));
	wall(offset+vec3(-1,-3,-1.5),offset+vec3(1,-1,-0.5));
}

void makeScene2helper(vec3 start, vec3 size){
	// for(int x = 0; x < 20; x++){
		// for(int y = 0; y < 20; y++){
			// wall(p1+vec3(5*x,5*y,1),p1+vec3(5*x+3,5*y+3,1+random(0,10)));
		// }
	// }
	bool xdiv;	//split across x
	bool ydiv;	//split across y

	float Rh = random(0,1);	//height
	float Rf = random(0,1);	//is floor
	float height = 2.f*pow(1.5f,Rh*Rh*7);
	bool floor = Rf > 0.9f;
	if(floor){height = 0.1;}

	bool too_big = size.x > 40 or size.y > 40 or size.x*size.y*height > 2000;
	bool too_small_x = size.x < 16;
	bool too_small_y = size.y < 16;
	float Ra = random(-1,1); //desired aspect
	float aspect = (size.x-size.y)/(size.x+size.y); //actual aspect (-1..1)
	//printf("aspect: desired: %f, actual: %f, diff: %f\n",Ra,aspect,fabs(Ra-aspect));
	if(fabs(Ra-aspect) < 0.3f){	//kinda squarish
		xdiv = too_big and not too_small_x;
		ydiv = too_big and not too_small_y;
	}else if(Ra > aspect and not too_small_y){
		xdiv = false;
		ydiv = true;
	}else if(Ra < aspect and not too_small_x){
		xdiv = true;
		ydiv = false;
	}else{
		xdiv = false;
		ydiv = false;
	}
	
	//bool x_aspect_split = (Rx > 0.5f);//(R > size.y/size.x);
	//bool y_aspect_split = (Ry > 0.5f);//false;//(R > size.x/size.y);
	//bool too_small = (size.x < 8) or (size.y < 8);
	//bool too_big = (size.x > 40) or (size.y > 40);// or (R > 0.5f);
	
	//bool xdiv = (size.x > 8) and ((size.x > 80) or (Rx > 0.5f));//not too_small and (x_aspect_split or too_big);
	//bool ydiv = (size.y > 8) and ((size.y > 80) or (Ry > 0.5f));//not too_small and (y_aspect_split or too_big);
	
	vec3 halfw = vec3(size.x/2,size.y,size.z);
	vec3 halfh = vec3(size.x,size.y/2,size.z);
	vec3 quarter = vec3(size.x/2.f,size.y/2.f,size.z);
	vec3 offw = vec3(halfw.x,0,0);
	vec3 offh = vec3(0,halfh.y,0);
	vec3 off = quarter;
	if(xdiv and ydiv){
		makeScene2helper(start,		quarter);
		makeScene2helper(start+offw,quarter);
		makeScene2helper(start+offh,quarter);
		makeScene2helper(start+off,	quarter);
	}else if(xdiv){
		makeScene2helper(start,halfw);
		makeScene2helper(start+offw,halfw);
	}else if(ydiv){
		makeScene2helper(start,halfh);
		makeScene2helper(start+offh,halfh);
	}else{
		vec3 p1 = vec3(start.x,start.y,start.z);
		vec3 p2 = vec3(start.x+size.x-4,start.y+size.y-4,start.z+height);
		printf("p1 = %s, p2 = %s\n",toCString(p1),toCString(p2));
		if(floor){
			entity *E = box(p1,p2,getTexture("materials/grass1"),1.f);
			E->name = string()+"grass_"+(int)entities.size();
		}else{
			entity *E = box(p1,p2,getTexture("materials/building2"),0.1f);
			E->name = string()+"building_"+(int)entities.size();
		}
	}
}

void makeScene2(vec3 offset){
	vec3 start = offset+vec3(10,10,0);
	vec3 size = vec3(200,200,1);
	//vec3 p1 = offset+vec3(start.x,start.y,-2);
	//vec3 p2 = offset+vec3(start.x+size.x,start.y+size.y,-1);
	entity *E = box(start,start+size,/*p1,p2,*/getTexture("materials/asphalt"),0.3f);
	E->name = string()+"asphalt_"+(int)entities.size();
	makeScene2helper(start,size);
}

void obelisk(){
	
	//entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider)
	entity *E = box(vec3(0,0,3),vec3(1,1,8),getTexture("materials/brick2"),0.5f);
	//entity *E = new entity();
	//addComponent(E,position,vec3(0,0,3));
	//addComponent(E,texture,getTexture("materials/brick2"));
	//rmodel *rm = generateBox(vec3(1,1,5))->getRmodel();
	//addComponent(E,rmodel,rm);
	//collisionbody *body = new collisionbodyAABB(rm->toModel()->getAABB());
	//addComponent(E,collider,body);
	//octree_visitor *ov = new octree_visitor();
	//ov->curNode = octree_root;
	//ov->body = body;
	//addComponent(E,octree_visitor,ov);
	setLayer(layer3D);
	renderLayer *pylonLayer = addNewLayer();
	hookAdd(globalChannel, EVENT_FRAME, "movePylon",[=](eventKind event){
		float t = getGameTime();
		vec3 pos = vec3(10,10,3+7*sin(t));
		E->setPosition(pos);
		setLayer(pylonLayer);
		clearLayer(pylonLayer);
		setColor(vec3(64,64,255));
		AABB aabb = dynamic_cast<collisionbodyAABB*>(E->body)->aabb;
		drawBoxWireframe(aabb.moveBy(pos));
	});
}