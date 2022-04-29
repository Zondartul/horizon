#include "physbox.h"
#include "entity.h"
#include "stringUtils.h"
#include "modelprimitives.h"
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
//extern octree_node *g_octree_root;

//should really figure out texscale on it's own
entity *box(vec3 start, vec3 end, texture *t, float texscale,bool hascollider,bool is_static){
	auto& entities = G->gs_entity->g_entities;

	entity *E = new entity();
	E->name = string()+"box_"+(int)entities.size();

	renderableModel *r = new renderableModel();
	e_model *em = generateBox(end-start,texscale);
	rmpack rms = em->getRmpack();
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

	E->setPosition((start+end)/2.0f);
	E->setVelocity(vec3(0,0,0));
	E->setGravity(vec3(0,0,0));
	E->setMass(1.0f);
	E->setBouncyness(1.0f);
	E->setFriction(0.5f);
	addEntity(E);
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
	auto& entities = G->gs_entity->g_entities;

	entity *E = box(start,end,getTexture("materials/brick2"),scale);
	E->name = string()+"wall_"+(int)entities.size();
}

void floor(vec3 start, vec3 end){
	auto& entities = G->gs_entity->g_entities;

	entity *E = box(start,end,getTexture("materials/brick3"),1/2.0f,true);
	E->name = string()+"floor_"+(int)entities.size();
}

//creates a physbox in front of the camera
entity *physbox(string tex, float size, float mass, float bouncyness, float friction){
	auto& entities = G->gs_entity->g_entities;
	auto& camera = G->gs_camera->g_camera;

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
}


renderableMultimodel* comboToRenderable(vector<physprim> combo){
    renderableMultimodel *rmm = new renderableMultimodel();
	int j = 0;
	renderableModel *r;
	for(unsigned int I = 0; I < combo.size(); I++){
		r = new renderableModel();

		rmpack rms;
		physprim p = combo[I];
		switch(p.type){
			case(PRIM_BOX):
				rms = generateBox(vec3(p.args[0],p.args[1],p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CYL):
				rms = generateCyllinder(p.args[0],p.args[1],(int)round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CONE):
				rms = generateCone(p.args[0],p.args[1],(int)round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_SPHERE):
				rms = generateCone(p.args[0],round(p.args[1]),(int)round(p.args[2]),p.args[3])
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
	auto& entities = G->gs_entity->g_entities;
	auto& camera = G->gs_camera->g_camera;

	printf("combo.size = %d\n",combo.size());
	for(unsigned int I = 0; I < combo.size(); I++){
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
	for(unsigned int I = 0; I < combo.size(); I++){
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
				rms = generateCyllinder(p.args[0],p.args[1],(int)round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_CONE):
				rms = generateCone(p.args[0],p.args[1],(int)round(p.args[2]),p.args[3])
				->getRmpack();
			break;
			case(PRIM_SPHERE):
				rms = generateCone(p.args[0],round(p.args[1]),(int)round(p.args[2]),p.args[3])
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


	for(unsigned int I = 1; I < combo.size(); I++){
		AABB aabb2 = rmm->parts[I]->rm_default->toModel()->getAABB();
		aabb2 = aabb2.moveBy(combo[I].pos);
		printf("aabb[%d] = %s\n",I,toCString(aabb2));
		aabb.start = vec3_min(aabb.start,aabb2.start);
		aabb.end = vec3_max(aabb.end,aabb2.end);
		aabb.size = aabb.end-aabb.start;
	}
	printf("aabb final = %s\n",toCString(aabb));
	//-----
	collisionbody *body = new collisionbodyAABB(aabb);
	E->body = body;
	body->E = E;
	body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
	body->vel = camera.forward()*0.2f;
	body->gravity = vec3(0,0,-0.25);
	body->mass = 1.f;
	body->restitution = 0.1f;
	body->friction = 1.f;

	body->type = BODY_DYNAMIC;
	addEntity(E);
}

void phystree(float mass, float bouncyness, float friction){
	auto& entities = G->gs_entity->g_entities;
	auto& camera = G->gs_camera->g_camera;

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
}

e_selection selectVertsCircle(vec3 pos, float dist, e_model *M,float hardness){
	e_selection sel(M);
	int j = 0;
	int k = 0;
	for(auto I = M->verts.begin(); I != M->verts.end(); I++){

		vec3 pos2 = (*I)->pos;
		float dist2 = length(pos2-pos);
		float distCoeff = dist2/dist;
		float x = distCoeff;
		if(dist2 < dist){k++;}
		
		sel.verts.push_back(*I);
		float weight = 0.f;
		float e = 2.718f;

		weight = pow(e,((1-2*x)*6)*hardness)/(pow(e,((1-2*x)*6)*hardness)+1);

		(*I)->selection_weight = weight;
	}
	printf("num inside verts = %d\n",k);
	return sel;
}

void makeSheet(vec3 start, vec3 end){
	auto& entities = G->gs_entity->g_entities;

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
	vec3 selPos = center-start;
	selPos += vec3(0,0.5,0);
	selPos.z = 0;
	float selSize = scale.x*0.5f;
	float selHardness = 0.75f;
	printf("selectVertsCircle(\n\tselPos = %s,\n\tselSize = %s\n\tscale = %s\n\tselHardness = %f\n",
		toCString(selPos),toCString(selSize),toCString(scale),selHardness);
	e_selection sel = selectVertsCircle(selPos,selSize,em_sheet,selHardness);
	sel.move(vec3(0,0,scale.z));
	sel = em_sheet->selectAll();
	sel.recalculateNormalsSmooth();
	//end model manip

	rmpack rm_sheet = em_sheet->getRmpack();
	renderableModel *RM = new renderableModel();
	RM->rm_default = rm_sheet.rm_tris;
	RM->rm_wireframe = rm_sheet.rm_wire;
	RM->t = getTexture("materials/grass2");
	RM->upload();
	E->r = RM;

	collisionbodyTerrain *body = new collisionbodyTerrain(AABB(scale));
	body->E = E;
	body->pos = offset;
	body->em = em_sheet;
	body->generateGridFromModel(numx,numy);
	E->body = body;


	addEntity(E);

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
	auto& entities = G->gs_entity->g_entities;

	bool xdiv;	//split across x
	bool ydiv;	//split across y

	float Rh = random(0,1);	//height
	float Rf = random(0,1);	//is floor
	float height = 2.f*pow(1.5f,Rh*Rh*7);
	bool floor = Rf > 0.9f;
	if(floor){height = 0.1f;}

	bool too_big = size.x > 40 || size.y > 40 || size.x*size.y*height > 2000;
	bool too_small_x = size.x < 16;
	bool too_small_y = size.y < 16;
	float Ra = random(-1,1); //desired aspect
	float aspect = (size.x-size.y)/(size.x+size.y); //actual aspect (-1..1)
	if(fabs(Ra-aspect) < 0.3f){	//kinda squarish
		xdiv = too_big && ! too_small_x;
		ydiv = too_big && ! too_small_y;
	}else if(Ra > aspect && ! too_small_y){
		xdiv = false;
		ydiv = true;
	}else if(Ra < aspect && ! too_small_x){
		xdiv = true;
		ydiv = false;
	}else{
		xdiv = false;
		ydiv = false;
	}

	vec3 halfw = vec3(size.x/2,size.y,size.z);
	vec3 halfh = vec3(size.x,size.y/2,size.z);
	vec3 quarter = vec3(size.x/2.f,size.y/2.f,size.z);
	vec3 offw = vec3(halfw.x,0,0);
	vec3 offh = vec3(0,halfh.y,0);
	vec3 off = vec3(quarter.x, quarter.y, 0);
	if(xdiv && ydiv){
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
	auto& entities = G->gs_entity->g_entities;

	vec3 start = offset+vec3(10,10,0);
	vec3 size = vec3(200,200,1);
	vec3 floorheight = vec3(0,0,1);
	entity *E = box(start-floorheight,start+size-floorheight, getTexture("materials/asphalt"),0.3f);
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
            bool Sside = 0;
            bool Eside = randint(0,1);
            bool Wside = 0;
            makeTile(offset+vec3(Ix,Iy,0)*sidelen, sidelen, Nside,Sside,Eside,Wside);
        }
    }
}

void obelisk(){
	auto& layer3D = G->gs_paint->g_layer3D;
	auto& globalChannel = G->gs_event->g_globalChannel;

	entity *E = box(vec3(0,0,3),vec3(1,1,8),getTexture("materials/brick2"),0.5f);

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

//entity *g_ent_flag = 0;
//timer *g_timer_flag = 0;

void spawnFlag(vec3 pos){
	auto& ent_flag = G->gs_physbox->g_ent_flag;
	auto& timer_flag = G->gs_physbox->g_timer_flag;


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
    td->wavenumber = 1.5f/numsegments;

    float pi = 180*d2r;
    for(unsigned int I = 0; I < rmdl->parts.size(); I++){
        td->offsets.push_back(rmdl->parts[I]->pos);
    }

    ent_flag = E;

    timer *T = new timer(0, 1, 1, 0, 0);
    timer_flag = T;
    T->F = [=](timer *T)
    {
        int t = getGameTicks();
        renderableMultimodel *rmdl = static_cast<renderableMultimodel*>(E->r);
        float dx = 0; float dy = 0;
        int n = td->offsets.size();
        for(unsigned int I = 1; I < td->offsets.size(); I++){
            renderableModel *r = rmdl->parts[I];
            float omega = td->freq/td->fps;
            float fps = td->fps;
            float k = 2*pi*td->wavenumber;

            //forward wave
            float ang1 = 30*d2r*sin(k*I-omega*t);
            //backward wave (commented out cause then it looks springy)
            float ang2 = 0;
            //interference pattern
            float ang = ang1+ang2;

            dx += -td->len*sin(ang)/2.f;
            dy += td->len*cos(ang)/2.f;
            r->rot = toVec3Angle(glm::angleAxis(ang,vec3(0,0,1)));
            vec3 pos = td->offsets[I];

            rmdl->offsets[I] = vec3(dx,dy,pos.z);
            dx += -td->len*sin(ang)/2.f;
            dy += td->len*cos(ang)/2.f;
        }
    };
    T->run = true;
}








