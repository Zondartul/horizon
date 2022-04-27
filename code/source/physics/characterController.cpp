#include "characterController.h"
#include "debug.h"
#include "physbox.h"
#include "simplemath.h"
#include "frameprinter.h"
#include "stringUtils.h"
#include "camera.h"
#include "resource.h"
#include "model.h"

//extern eventChannel *g_inputChannel;
//extern eventChannel *g_globalChannel;


characterController::characterController(entity *newE):E(newE){
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
        float targetHeight = 1.8f;
        float sizeRatio = targetHeight/msize.z;
        selAll.move(-mcenter);
        selAll.scale(vec3(0,0,0),sizeRatio);

        aabb = selAll.getAABB();
        selAll.recalculateNormalsSmooth();

        rmpack rms = em->getRmpack();
        rmdl->rm_default = rms.rm_tris;
        rmdl->rm_wireframe = rms.rm_wire;
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
        E->body->pos = g_camera.pos-vec3(0.5,0.5,0.5)+g_camera.forward()*2.f;
        E->body->vel = g_camera.forward()*0.2f;
        E->body->gravity = vec3(0,0,-0.25);
        E->body->E = E;


        addEntity(E);
	}

	if(!E->body){error("can't assign a char controller to an entity without a body\n");}
	E->name = string("char_")+toString((void*)this);
	printf("charController %p constructor(E = %p)\n",this,newE);
	g_inputChannel->addListener(this);
	g_globalChannel->addListenerFront(this);
}
characterController::~characterController(){
	printf("charController %p destructor\n",this);
	g_inputChannel->removeListener(this);
	g_globalChannel->removeListener(this);
}
void characterController::onEvent(eventKind event){
	if (event.type == EVENT_FRAME){
		think();
	}
}
#include "paint.h"
void characterController::think(){
	if(!E){delete this; return;}
	auto &body = E->body;
	float mixratio = 0.1f;

	bool onGround = false;

	collisioninfo *CI;
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
			}else{printf("char: no E in CI->body2\n");}
		}else{printf("char: no body2 in CI\n");}
		delete CI;
		if(dist < (size.z/2.f+0.1f)){onGround = true;}
	}else{
	}

	E->r->rot = toVec3Angle(slerp(toQuat(E->r->rot),toQuat(targetDir),0.1f));
	if(fly){body->vel.z = mix(body->vel.z,targetVelocity.z,mixratio);}
	else{
		if(onGround){
			body->vel.x = targetVelocity.x;
			body->vel.y = targetVelocity.y;
			if(jump){body->vel.z = targetVelocity.z;}
			else{body->vel.z = 0;}
		}else{
			float airControl = 0.025f;
			body->vel.x = mix(body->vel.x,targetVelocity.x,airControl);
			body->vel.y = mix(body->vel.y,targetVelocity.y,airControl);
		}
	}
}
