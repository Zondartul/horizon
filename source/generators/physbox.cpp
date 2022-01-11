#include "physbox.h"
#include "entity.h"
#include "stringUtils.h"
#include "modelPrimitives.h"
#include "modelcomposite.h"
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
#include "geometry.h"
extern octree_node *octree_root;

//should really figure out texscale on it's own
entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider,bool is_static){
	entity *E = new entity();
	E->name = string()+"box_"+(int)entities.size();

	renderableModel *r = new renderableModel();
	e_model *em = generateBox(end-start,texscale);
	rmpack rms = em->getRmpack();
	//rmodel *rm = em->getRmodel();
	r->rm_default = rms.rm_tris;
	r->rm_wireframe = rms.rm_wire;
	r->color = vec3(255,255,255);
	r->t = t;
	r->upload();
	E->r = r;

	collisionbody *body = new collisionbodyAABB(rms.rm_tris->toModel()->getAABB());
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
	E->setFriction(0.5f);
	addEntity(E);
	//if(hascollider){
	//	body->type = BODY_STATIC;
	//}else{
	//	body->type = BODY_NOCOLLIDE;
	//}

	return E;
}

entity *physplane(vec3 A, vec3 B, vec3 C, texture *t, float texscale){
	entity *E = new entity();
	E->name = "physplane";
	collisionbodyPlane *cbp = new collisionbodyPlane(A,B,C);
	cbp->bodyname = "physplane.body";
	cbp->E = E;
	E->body = cbp;
	
	renderablePlane *rp = new renderablePlane();
	rp->normal = triangle_normal(A,B,C);
	rp->offset = dot(A,rp->normal);
	rp->texscale = texscale;
	rp->t = t;
	rp->upload();
	E->r = rp;
	E->setMass(0.0f);
	E->setBouncyness(1.0f);
	E->setFriction(0.9f);
	addEntity(E);
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
entity *physbox(string tex, float size, float mass, float bouncyness, float friction){
	entity *E = new entity();
	E->name = string()+"physbox_"+(int)entities.size();

	renderableModel *r = new renderableModel();
	rmpack rms = generateBox(size*vec3(1,1,1))->getRmpack();
	r->rm_default = rms.rm_tris;
	r->rm_wireframe = rms.rm_wire;
	r->color = vec3(255,255,255);
	r->t = getTexture(tex);
	r->upload();
	E->r = r;

	collisionbody *body = new collisionbodyAABB(rms.rm_tris->toModel()->getAABB());
	E->body = body;
	body->E = E;
	body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
	body->vel = camera.forward()*0.2f;
	body->gravity = vec3(0,0,-0.25);
	body->mass = mass;
	body->restitution = bouncyness;
	body->friction = friction;

	body->type = BODY_DYNAMIC;
	addEntity(E);
	return E;
	//ov is handled automatically by collisionbody
	//body->ov = new octree_visitor(octree_root,body,body->pos);
	//ov->curNode = octree_root;
	//ov->body = body;
	//octree_root->addVisitor(ov);
	//ov->moveTo(body->pos);
}


renderableMultimodel* comboToRenderable(vector<physprim> combo){
    renderableMultimodel *rmm = new renderableMultimodel();
	int j = 0;
	renderableModel *r;
	//for(auto I = combo.begin(); I != combo.end(); I++){
	for(int I = 0; I < combo.size(); I++){
		//printf("j = %d\n",j++);
		r = new renderableModel();

		rmpack rms;
		//printf("deref\n");
		physprim p = combo[I];
		//printf("switch\n");
		switch(p.type){
			case(PRIM_BOX):
				rms = generateBox(vec3(p.args[0],p.args[1],p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CYL):
				rms = generateCyllinder(p.args[0],p.args[1],round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CONE):
				rms = generateCone(p.args[0],p.args[1],round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_SPHERE):
				rms = generateCone(p.args[0],round(p.args[1]),round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			default:
				assert(!"unknown primitive type");
				break;
		};
		r->rm_default = rms.rm_tris;
		r->rm_wireframe = rms.rm_wire;
		r->t = getTexture(p.texname);
		r->pos = p.pos;
		rmm->parts.push_back(r);
	}
	rmm->upload();
	return rmm;
}

void physcombo(vector<physprim> combo){

	printf("combo.size = %d\n",combo.size());
	for(int I = 0; I < combo.size(); I++){
		printf("combo[%d]:\n",I);
		physprim p = combo[I];
		printf("\ttype = %d\n", p.type);
		printf("\ttexname = [%s]\n",p.texname.c_str());
		printf("\tpos = [%s]\n",toCString(p.pos));
		printf("\targs = {%f,%f,%f,%f}\n",p.args[0],p.args[1],p.args[2],p.args[3]);
	}

	entity *E = new entity();
	E->name = string()+"combo_"+(int)entities.size();

	renderableMultimodel *rmm = new renderableMultimodel();
	int j = 0;
	renderableModel *r;
	//for(auto I = combo.begin(); I != combo.end(); I++){
	for(int I = 0; I < combo.size(); I++){
		printf("j = %d\n",j++);
		r = new renderableModel();

		rmpack rms;
		printf("deref\n");
		physprim p = combo[I];
		printf("switch\n");
		switch(p.type){
			case(PRIM_BOX):
				rms = generateBox(vec3(p.args[0],p.args[1],p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CYL):
				rms = generateCyllinder(p.args[0],p.args[1],round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CONE):
				rms = generateCone(p.args[0],p.args[1],round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_SPHERE):
				rms = generateCone(p.args[0],round(p.args[1]),round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			default:
				assert(!"unknown primitive type");
				break;
		};
		r->rm_default = rms.rm_tris;
		r->rm_wireframe = rms.rm_wire;
		r->t = getTexture(p.texname);
		r->pos = p.pos;
		rmm->parts.push_back(r);
	}
	rmm->upload();
	E->r = rmm;

	//put this into a separate func.

	//----- bounding box combining
	assert(rmm->parts.size() > 0);
	assert(rmm->parts[0]->rm_default != 0);
	AABB aabb = rmm->parts[0]->rm_default->toModel()->getAABB();
	aabb = aabb.moveBy(combo[0].pos);
	printf("aabb[%d] = %s\n",0,toCString(aabb));


	for(int I = 1; I < combo.size(); I++){
		AABB aabb2 = rmm->parts[I]->rm_default->toModel()->getAABB();
		aabb2 = aabb2.moveBy(combo[I].pos);
		printf("aabb[%d] = %s\n",I,toCString(aabb2));
		aabb.start = vec3_min(aabb.start,aabb2.start);
		aabb.end = vec3_max(aabb.end,aabb2.end);
		aabb.size = aabb.end-aabb.start;
	}
	printf("aabb final = %s\n",toCString(aabb));
	//-----
	collisionbody *body = new collisionbodyAABB(aabb);//rm->toModel()->getAABB());
	E->body = body;
	body->E = E;
	body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
	body->vel = camera.forward()*0.2f;
	body->gravity = vec3(0,0,-0.25);
	body->mass = 1.f;
	body->restitution = 0.f;
	body->friction = 0.f;

	body->type = BODY_DYNAMIC;
	addEntity(E);
}

void phystree(float mass, float bouncyness, float friction){
	entity *E = new entity();
	E->name = string()+"tree_"+(int)entities.size();

	float tr = 3.f*0.1f;
	float th = 3.f*0.75f;

	renderableMultimodel *rmm = new renderableMultimodel();
	renderableModel *r[3];
	rmpack rms;

	r[0] = new renderableModel();
	rms = generateCyllinder(0.75f*tr,th/2.f)->getRmpack();
	r[0]->rm_default = rms.rm_tris;
	r[0]->rm_wireframe = rms.rm_wire;
	r[0]->t = getTexture("materials/log_wall");
	r[0]->pos = vec3(0,0,0);
	rmm->parts.push_back(r[0]);

	r[1] = new renderableModel();
	rms = generateCone(2.5f*tr,th)->getRmpack();
	r[1]->rm_default = rms.rm_tris;
	r[1]->rm_wireframe = rms.rm_wire;
	r[1]->t = getTexture("materials/leaves_fir");
	r[1]->pos = vec3(0,0,th/2.f);
	rmm->parts.push_back(r[1]);

	r[2] = new renderableModel();
	rms = generateCone(2.f*tr,th)->getRmpack();
	r[2]->rm_default = rms.rm_tris;
	r[2]->rm_wireframe = rms.rm_wire;
	r[2]->t = getTexture("materials/leaves_fir");
	r[2]->pos = vec3(0,0,th);
	rmm->parts.push_back(r[2]);

	rmm->upload();

	//r1->color = vec3(255,255,255);
	//r1->t = getTexture(tex);
	//r1->upload();



	E->r = rmm;


	//todo: make AABB from other AABBs and also from node graphs.
	collisionbody *body = new collisionbodyAABB(rms.rm_tris->toModel()->getAABB());
	E->body = body;
	body->E = E;
	body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
	body->vel = camera.forward()*0.2f;
	body->gravity = vec3(0,0,-0.25);
	body->mass = mass;
	body->restitution = bouncyness;
	body->friction = friction;

	body->type = BODY_DYNAMIC;
	addEntity(E);
	//ov is handled automatically by collisionbody
	//body->ov = new octree_visitor(octree_root,body,body->pos);
	//ov->curNode = octree_root;
	//ov->body = body;
	//octree_root->addVisitor(ov);
	//ov->moveTo(body->pos);
}

e_selection selectVertsCircle(vec3 pos, float dist, e_model *M,float hardness){
	e_selection sel(M);
	int j = 0;
	int k = 0;
	for(auto I = M->verts.begin(); I != M->verts.end(); I++){

		vec3 pos2 = (*I)->pos;
		float dist2 = length(pos2-pos);
		//printf("dist2 = %f, pos2 = %s\n",dist2,toCString(pos2));
		float distCoeff = dist2/dist;
		float x = distCoeff;
		//if(j == 0){printf("at(0,0) dist = %f, dist2 = %f, distCoeff = %f\n",dist,dist2,distCoeff);}
		if(dist2 < dist){k++;}
		/*
		if(distCoeff < 1.0f){
			sel.verts.push_back(*I);
			(*I)->selection_weight = pow(1.0f-distCoeff,hardness);
		}
		*/

		sel.verts.push_back(*I);
		float weight = 0.f;
		float e = 2.718f;

		//bell curve (always pointy)
		//weight = pow(e,-hardness*x);

		weight = pow(e,((1-2*x)*6)*hardness)/(pow(e,((1-2*x)*6)*hardness)+1);

		//printf("weight = %f\n",weight);
		(*I)->selection_weight = weight;
		//if((weight > 0.45f) && (weight < 0.55f)){printf("w 0.5 at (%d,%d)\n",j%16,(j-j%16)/16);}
	}
	printf("num inside verts = %d\n",k);
	return sel;
}

void makeSheet(vec3 start, vec3 end){
	printf("makesheet-----------\n");
	entity *E = new entity();
	E->name = string("sheet_")+(int)entities.size();
	vec3 offset = start;
	vec3 scale = end-start;
	printf("scale = %s\n",toCString(scale));

	int numx = 8; int numy = 8;
	e_model *em_sheet = generateSheet(numx,numy,scale);

	vec3 center = (start+end)/2.f;
	//model manipulation here
	vec3 selPos = center-start;//vec3(1,0.5,0);
	selPos += vec3(0,0.5,0);
	selPos.z = 0;
	float selSize = scale.x*0.5f;//length(vec3(scale.x,scale.y,0))*0.75f;
	//printf("selSize = %f\n",selSize);
	float selHardness = 0.75f;//1/3.14f;
	//e_selection sel = selectVertsCircle(vec3(1,0.5,0),0.4,em_sheet,0.5);
	printf("selectVertsCircle(\n\tselPos = %s,\n\tselSize = %s\n\tscale = %s\n\tselHardness = %f\n",
		toCString(selPos),toCString(selSize),toCString(scale),selHardness);
	e_selection sel = selectVertsCircle(selPos,selSize,em_sheet,selHardness);
	//sel.move(vec3(0,0,0.2));
	sel.move(vec3(0,0,scale.z));
	sel = em_sheet->selectAll();
	sel.recalculateNormalsSmooth();
	//end model manip

	rmpack rm_sheet = em_sheet->getRmpack();
	//model *m = rm_sheet->toModel();
	//m->recalculateNormals();
	//delete rm_sheet;
	//rm_sheet = new rmodel(m);
	renderableModel *RM = new renderableModel();
	RM->rm_default = rm_sheet.rm_tris;
	RM->rm_wireframe = rm_sheet.rm_wire;
	RM->t = getTexture("materials/grass2");
	//RM->pos = offset;
	RM->upload();
	E->r = RM;

	collisionbodyTerrain *body = new collisionbodyTerrain(AABB(scale));
	body->E = E;
	body->pos = offset;
	body->em = em_sheet;
	body->generateGridFromModel(numx,numy);
	//body->updateGridFromModel();
	E->body = body;


	addEntity(E);

	/*
	hookAdd(globalChannel, EVENT_FRAME, "showSheet",[=](eventKind event){
		setLayer(layerDebug);
		setColor(vec3(0,255,0));
		vec3 A = offset;
		vec3 B = offset+vec3(1,0,0)*scale;
		vec3 C = offset+vec3(1,1,0)*scale;
		vec3 D = offset+vec3(0,1,0)*scale;
		drawLine(A,B);
		drawLine(B,C);
		drawLine(C,D);
		drawLine(D,A);
	});
	*/

	printf("end makesheet------\n");
}

void makeScene1(vec3 offset){
	floor(offset+vec3(-10,-10,-2),offset+vec3(10,10,-1)); lastEntity()->group = "scene1";
	wall(offset+vec3(-6,-3,-1),offset+vec3(-5,10,1)); lastEntity()->group = "scene1";
	wall(offset+vec3(-6,-10,-1),offset+vec3(-5,-4,1)); lastEntity()->group = "scene1";

	wall(offset+vec3(-3,-6,-1),offset+vec3(4,-5,1)); lastEntity()->group = "scene1";
	wall(offset+vec3(3,-5,-1),offset+vec3(4,3,1)); lastEntity()->group = "scene1";

	wall(offset+vec3(1,-4,-1.0),offset+vec3(2,-3,-0.5)); lastEntity()->group = "scene1";
	wall(offset+vec3(-1,-3,-1.5),offset+vec3(1,-1,-0.5)); lastEntity()->group = "scene1";
	
	vec3 A = vec3(0,0,0)+vec3(0,0,-1.5)+offset;
	vec3 B = vec3(1,0,0)+vec3(0,0,-1.5)+offset;
	vec3 C = vec3(0,1,0)+vec3(0,0,-1.5)+offset;
	
	physplane(C,B,A,getTexture("materials/grass1")); lastEntity()->group = "scene1";
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
	vec3 off = vec3(quarter.x, quarter.y, 0);
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
		//printf("p1 = %s, p2 = %s\n",toCString(p1),toCString(p2));
		if(floor){
			entity *E = box(p1,p2,getTexture("materials/grass1"),1.f);
			E->name = string()+"grass_"+(int)entities.size();
		}else{
			entity *E = box(p1,p2,getTexture("materials/building2"),0.1f);
			E->name = string()+"building_"+(int)entities.size();
		}
		lastEntity()->group = "scene2";
	}
}

void makeScene2(vec3 offset){
	vec3 start = offset+vec3(10,10,0);
	vec3 size = vec3(200,200,1);
	//vec3 p1 = offset+vec3(start.x,start.y,-2);
	//vec3 p2 = offset+vec3(start.x+size.x,start.y+size.y,-1);
	vec3 floorheight = vec3(0,0,1);
	entity *E = box(start-floorheight,start+size-floorheight,/*p1,p2,*/getTexture("materials/asphalt"),0.3f);
	lastEntity()->group = "scene2";
	E->name = string()+"asphalt_"+(int)entities.size();
	makeScene2helper(start,size);
}

void makeTile(vec3 offset, float sidelen, bool Nside, bool Sside, bool Eside, bool Wside){
    vec3 vh = vec3(0,0,0.5)*sidelen;
    floor(offset-vec3(0,0,0.1),           offset+vec3(1.0,1.0,0)*sidelen);            lastEntity()->group = "scene3";
//----------------------- E -------------------------------
    wall(offset,                        offset+vec3(0.1,0.3,0)*sidelen+vh);            lastEntity()->group = "scene3";
  if(Eside){
    wall(offset+vec3(0,0.3,0)*sidelen,  offset+vec3(0.1,0.7,0)*sidelen+vh);            lastEntity()->group = "scene3";
  }
    wall(offset+vec3(0,0.7,0)*sidelen,  offset+vec3(0.1,1.0,0)*sidelen+vh);            lastEntity()->group = "scene3";
//----------------------------------------------------------

//----------------------- N -------------------------------
    wall(offset,                        offset+vec3(0.3,0.1,0)*sidelen+vh);            lastEntity()->group = "scene3";
  if(Nside){
    wall(offset+vec3(0.3,0,0)*sidelen,  offset+vec3(0.7,0.1,0)*sidelen+vh);            lastEntity()->group = "scene3";
  }
    wall(offset+vec3(0.7,0,0)*sidelen,  offset+vec3(1.0,0.1,0)*sidelen+vh);            lastEntity()->group = "scene3";
//----------------------------------------------------------
//----------------------- W -------------------------------
    wall(offset+vec3(0.9,0,0)*sidelen,    offset+vec3(1.0,0.3,0)*sidelen+vh);            lastEntity()->group = "scene3";
  if(Wside){
    wall(offset+vec3(0.9,0.3,0)*sidelen,  offset+vec3(1.0,0.7,0)*sidelen+vh);            lastEntity()->group = "scene3";
  }
    wall(offset+vec3(0.9,0.7,0)*sidelen,  offset+vec3(1.0,1.0,0)*sidelen+vh);            lastEntity()->group = "scene3";
//----------------------------------------------------------

//----------------------- S -------------------------------
    wall(offset+vec3(0,0.9,0)*sidelen,  offset+vec3(0.3,1.0,0)*sidelen+vh);            lastEntity()->group = "scene3";
  if(Sside){
    wall(offset+vec3(0.3,0.9,0)*sidelen,  offset+vec3(0.7,1.0,0)*sidelen+vh);            lastEntity()->group = "scene3";
  }
    wall(offset+vec3(0.7,0.9,0)*sidelen,  offset+vec3(1.0,1.0,0)*sidelen+vh);            lastEntity()->group = "scene3";
//----------------------------------------------------------


}

void makeScene3(vec3 offset){
    int tilesx = 6;
    int tilesy = 6;
    float sidelen = 4.f;
    float height = 0.75f;
    for(int Ix = 0; Ix < tilesx; Ix++){
        for(int Iy = 0; Iy < tilesy; Iy++){
            bool Nside = randint(0,1);
            bool Sside = 0;//randint(0,1);
            bool Eside = randint(0,1);
            bool Wside = 0;//randint(0,1);
            makeTile(offset+vec3(Ix,Iy,0)*sidelen, sidelen, Nside,Sside,Eside,Wside);
        }
    }
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
	renderLayer *pylonLayer = addNewLayer("pylon");
	hookAdd(globalChannel, EVENT_FRAME, "movePylon",[=](eventKind event){
		float t = getGameTime();
		vec3 pos = vec3(10,10,3+7*sin(t));
		E->setPosition(pos);
		setLayer(pylonLayer);
		clearLayer(pylonLayer);
		setColor(vec3(64,64,255));
		AABB aabb = dynamic_cast<collisionbodyAABB*>(E->body)->getAABB();
		drawBoxWireframe(aabb.moveBy(pos));
	});
}

entity *ent_flag = 0;
timer *timer_flag = 0;

void spawnFlag(vec3 pos){
    if(ent_flag){
        delete timer_flag;
        delete ent_flag;
        timer_flag = 0;
        ent_flag = 0;
    }

    entity *E = new entity();
    E->name = "flag";
    E->body = new collisionbodyPoint();
    E->body->pos = pos;
    E->body->E = E;

    /* tree:
    		{PRIM_CYL,	"materials/log_wall",
			vec3(0,0,0),
			{0.075f,0.375,16,2.0f}},
		{PRIM_CONE,	"materials/leaves_fir",
			vec3(0,0,0.375),
			{0.25f,0.75,16,-4.0f}},
    */
    float scale = 0.3f;
    int numsegments = 16;
    float sidelen = 3.0f*scale/numsegments;
    vector<physprim> combo;
    combo.push_back({PRIM_CYL, "materials/steelUnlit", vec3(0,0,0)*scale,    {0.05f*scale, 10.f*scale, 16, 1.f}});
    for(int I = 0; I < numsegments; I++){
        combo.push_back({PRIM_BOX, "materials/redlines", vec3(0,(sidelen/2.f)*(2*I+1),9)*scale, {0.01f*scale, sidelen, 2.f*scale, 1.f}});
    }

    renderableMultimodel *rmdl = comboToRenderable(combo);
    E->r = rmdl;

    addEntity(E);

    struct timerData{
        vector<vec3> offsets;
        float len;
        float freq;
        float wavenumber;
        float fps;
    };
    timerData *td = new timerData;
    td->len = sidelen;
    td->freq = 1.f/3.f;
    td->fps = 20.f;
    td->wavenumber = 1.5/numsegments;

    float pi = 180*d2r;
    for(int I = 0; I < rmdl->parts.size(); I++){
        td->offsets.push_back(rmdl->parts[I]->pos);
    }

    ent_flag = E;

    timer *T = new timer(0, 1, 1, 0, 0);
    timer_flag = T;
    T->F = [=](timer *T)
    {
        //if(E){ todo: figure out if the ent is alive, use elastic_ptr?
            //printf("tick\n");
            int t = getGameTicks();
            renderableMultimodel *rmdl = static_cast<renderableMultimodel*>(E->r);
            //printf("pos00 = %s\n", toCString(td->initialPos[0]));
            float dx = 0; float dy = 0;
            int n = td->offsets.size();
            for(int I = 1; I < td->offsets.size(); I++){
                renderableModel *r = rmdl->parts[I];
                float omega = td->freq/td->fps;
                float fps = td->fps;
                float k = 2*pi*td->wavenumber;

                //forward wave
                float ang1 = 30*d2r*sin(k*I-omega*t);
                //backward wave (commented out cause then it looks springy)
                float ang2 = 0;//30*d2r*sin(k*I+omega*t);
                //interference pattern
                float ang = ang1+ang2;

                dx += -td->len*sin(ang)/2.f;
                dy += td->len*cos(ang)/2.f;
                //printf("ang %d = %f\n",I,ang);
                r->rot = toVec3Angle(glm::angleAxis(ang,vec3(0,0,1)));
                vec3 pos = td->offsets[I];
                //printf("pos0 = %s, ", toCString(pos));

                rmdl->offsets[I] = vec3(dx,dy,pos.z);
                dx += -td->len*sin(ang)/2.f;
                dy += td->len*cos(ang)/2.f;
                //printf("pos1 = %s\n", toCString(r->pos));
            }
            //printf("dx = %f, dy = %f\n",dx,dy);
        //}else{delete T;}
    };
    T->run = true;
}








