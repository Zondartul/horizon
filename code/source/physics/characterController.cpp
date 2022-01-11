#include "characterController.h"
#include "debug.h"
#include "physbox.h"
#include "simplemath.h"
#include "frameprinter.h"
#include "stringUtils.h"
#include "camera.h"
#include "resource.h"
#include "model.h"

extern eventChannel *inputChannel;
extern eventChannel *globalChannel;

/*
characterController::characterController():
characterController(physbox("materials/APPULS",size,1.f,0.2f,0.0f/60.f))
{
	printf("charController %p constructor()\n",this);
}
*/
characterController::characterController(entity *newE):E(newE){
	//if(!E){E = physbox("materials/APPULS",size,1.f,/*0.2f,0.5f*/0.2f,0.0f);}

	if(!E){
        E = new entity();

        //-------------- renderable -----------------------
        renderableModel *rmdl = new renderableModel();
        E->r = rmdl;

        model *m = getModel("character/char1");
        e_model *em = m->toEmodel();
        e_selection selAll = em->selectAll();
        selAll.removeDuplicates();
        selAll.rotate(vec3(0,0,0),vec3(1,0,0),90);
        AABB aabb = selAll.getAABB();
        //figure out transform to bring the center of AABB to origin
        vec3 mcenter = (aabb.end+aabb.start)/2.f;
        vec3 msize = aabb.size;
        //printf("model aabb start = %s, end = %s, size = %s\n", toCString(aabb.start), toCString(aabb.end), toCString(aabb.size));
        float targetHeight = 1.8f;
        float sizeRatio = targetHeight/msize.z;
        //printf("sizeRatio = %f\n",sizeRatio);
        selAll.move(-mcenter);
        selAll.scale(vec3(0,0,0),sizeRatio);

        aabb = selAll.getAABB();
        //aabb = AABB(-msize*sizeRatio/2.f,msize*sizeRatio/2.f);
        //printf("final aabb start = %s, end = %s, size = %s\n", toCString(aabb.start), toCString(aabb.end), toCString(aabb.size));
        selAll.recalculateNormalsSmooth();

        rmpack rms = em->getRmpack();
        rmdl->rm_default = rms.rm_tris;
        rmdl->rm_wireframe = rms.rm_wire;
        //rmdl->t = getTexture("materials/APPULS");
        rmdl->t = m->t;
        rmdl->upload();
        //------------------ collisionbody ---------------------------
        //make him slimmer
        float r = 0.1f;
        aabb = aabb.setStart(vec3(-r,-r,aabb.start.z)).setEnd(vec3(r,r,aabb.end.z));

        E->body = new collisionbodyAABB(aabb);
        E->body->mass = 1.f;
        E->body->friction = 0.0f;
        E->body->restitution = 0.2f;
        E->body->type = BODY_DYNAMIC;
        //to spawn him from camera
        E->body->pos = camera.pos-vec3(0.5,0.5,0.5)+camera.forward()*2.f;
        E->body->vel = camera.forward()*0.2f;
        E->body->gravity = vec3(0,0,-0.25);
        E->body->E = E;


        addEntity(E);
	}

	if(!E->body){error("can't assign a char controller to an entity without a body\n");}
	E->name = string("char_")+toString((void*)this);
	printf("charController %p constructor(E = %p)\n",this,newE);
	inputChannel->addListener(this);
	globalChannel->addListenerFront(this);
}
characterController::~characterController(){
	printf("charController %p destructor\n",this);
	inputChannel->removeListener(this);
	globalChannel->removeListener(this);
}
void characterController::onEvent(eventKind event){
	//printf("charController %p onEvent\n",this);
	if (event.type == EVENT_FRAME){
		think();
	}
}
#include "paint.h"
void characterController::think(){
	if(!E){delete this; return;}
	auto &body = E->body;
	float mixratio = 0.1f;// 0.5f/60.f;

	bool onGround = false;

	collisioninfo *CI;
	//vector<entity*> ignore;
	//ignore.push_back(E);
	vec3 size = E->body->getAABB().size;

	CI = raytrace(body->pos, vec3(0,0,-1),{E});//&ignore);
	if(CI){
		float dist = CI->c_to_c.depth;
		//delete CI;
		string E2name = "<error>";
		
		//if you have to check, then something has gone
		//terribly wrong somewhere else
		if(CI->body2){
			if(CI->body2->E){
				E2name = CI->body2->E->name;
				//printf("char: CI->body2->E->name = [%s] (ok)\n", E2name.c_str());
			}else{printf("char: no E in CI->body2\n");}
		}else{printf("char: no body2 in CI\n");}
		delete CI;
		//else this should be safe (body2 always has an entity)
		//string E2name = CI->body2->E->name;
		
		//frameprint(fstring("character %p to (%s): %f meters\n",this,E2name.c_str(),dist));
		//printf("size.z/2.f = %f, dist = %f ",size.z/2.f,dist);
		if(dist < (size.z/2.f+0.1f)){onGround = true;}
		//printf("onGround = %d\n",onGround);
	}else{
		//frameprint(fstring("character %p to (null): 0 meters\n",this));
	}

	E->r->rot = toVec3Angle(slerp(toQuat(E->r->rot),toQuat(targetDir),0.1f));//mix(E->r->rot,targetDir,0.1f);
	if(fly){body->vel.z = mix(body->vel.z,targetVelocity.z,mixratio);}
	else{
		if(onGround){
			body->vel.x = targetVelocity.x;//mix(body->vel.x,targetVelocity.x,mixratio);
			body->vel.y = targetVelocity.y;//mix(body->vel.y,targetVelocity.y,mixratio);
			if(jump){body->vel.z = targetVelocity.z;}
			else{body->vel.z = 0;}
		}else{
			float airControl = 0.025f;
			body->vel.x = mix(body->vel.x,targetVelocity.x,airControl);
			body->vel.y = mix(body->vel.y,targetVelocity.y,airControl);
		}
	}
	//printf("[CC %p]: \"tv = %s, dv = %s\"\n",this,toCString(targetVelocity),toCString(body->vel));
	//setLayer(layerDebug);

	//drawArrow(vec3(0,0,0),body->pos,vec3(0,0,0));
}
