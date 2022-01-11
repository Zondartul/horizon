#include "ecs.h"
#include "entity.h"
#include "renderLayer.h"
#include "paint.h"
#include "globals.h"
#include "collision.h"
#include "frameprinter.h"
//---------------- RENDER ---------------------------------------
ecs_render_system_kind::ecs_render_system_kind(){
	ecs_render_layer = new renderLayer();
	addLayer(layer3D, ecs_render_layer);
	globalChannel->addListener(this);
}

void ecs_render_system_kind::onEvent(eventKind event){
	if(!event.type == EVENT_FRAME){return;}
	setLayer(ecs_render_layer);
	ecs_render_layer->clear();
	ecs_render_layer->reset();
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->r && E->body){
			setColor(E->r->color);
			//if we end up with renderables that don't have a position,
			//then this func will change.
			setPosition(E->body->pos);
			setScale(vec3(1,1,1));
			setRotation(vec3(0,0,0));
			setRenderMode(3);
			setLighting(true);
			if(E->r->t){
				setTexturing(true);
				setTexture(E->r->t);
			}else{
				setTexturing(false);
			}
			drawRmodel(E->r->rm);
		}
	}
}
//---------------- PHYSICS --------------------------------------
ecs_physics_system_kind::ecs_physics_system_kind(){
	globalChannel->addListener(this);
}

void applyGravity(entity *E){
	E->setVelocity(E->getVelocity()+E->getGravity()/60.0f);
}

void applyVelocity(entity *E){
	E->setPosition(E->getPosition() + E->getVelocity());
}

bool isSane(entity *E){
	if(E->body){
		if(E->body->vel != E->body->vel){printf("broken velocity on entity %s, removing\n",E->name.c_str()); return false;}
		if(E->body->pos != E->body->pos){printf("broken position on entity %s, removing\n",E->name.c_str()); return false;}
		if(E->body->pos.z < -500.f){printf("entity %s fell into the void\n",E->name.c_str()); return false;}
	}
	return true;
}

void ecs_physics_system_kind::onEvent(eventKind event){
	if(!physicsOn){return;}
	if(!event.type == EVENT_FRAME){return;}
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->body){
			applyGravity(*I);
			applyVelocity(*I);
		}
	}
	for(auto I = entities.begin(); I != entities.end();){
		if(!isSane(*I)){
			delete *I;
			I = entities.erase(I); 
		}else{
			I++;
		}
	}
}

//---------------- COLLISION ------------------------------------
ecs_collision_system_kind::ecs_collision_system_kind(){
	globalChannel->addListener(this);
}

void ecs_collision_system_kind::onEvent(eventKind event){
	if(!physicsOn){return;}
	if(!event.type == EVENT_FRAME){return;}
	extern octree_node* octree_root;
	//printf("\nBROADPHASE\n");
	broadphaseinfo *bp = checkCollisionBroadphase(octree_root);
	//printf("END BP\n");
	for(auto I = bp->pairs.begin(); I != bp->pairs.end(); I++){
		pairwiseCollisionCheck(I->first,I->second);
	}
	//frameprint(fstring("bp: %p",bp));
	int nbodies, npairs, efficiency;
	nbodies = bp->bodies.size();
	npairs = bp->pairs.size();
	if(nbodies){
		efficiency = 100*npairs/(nbodies*nbodies);
	}else{
		efficiency = 0;
	}
	if(bp){frameprint(fstring("%d bodies, %d pairs (%d%% of N^2)",nbodies,npairs,efficiency));}
	//broadphaseRender(bp);
	delete bp;
}