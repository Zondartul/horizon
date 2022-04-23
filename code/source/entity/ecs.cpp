#include "ecs.h"
#include "entity.h"
#include "renderLayer.h"
#include "paint.h"
#include "globals.h"
#include "collision.h"

#include "frameprinter.h"

ecs_kind g_ecs;
renderLayer *g_ecs_render_layer;
renderLayer *g_ecs_render_layer_2d;
bool g_physicsOn = true;
bool g_collisionOn = true;
bool g_texturingOn = true;

extern bool g_gamePaused;
//---------------- RENDER ---------------------------------------
ecs_render_system_kind::ecs_render_system_kind(){
	options.wireframe = false;
	options.texturing = true;
	options.lighting = true;
	options.coloring = false;
    options.color = vec3(255,255,255);
    options.alpha = 255;
    options.sunPos = vec3(0.5,0.75,1);
	options.sunLightColor = 0.9f*vec3(1,1,1);
	options.ambientLightColor = 0.3f*vec3(1,1,1);

	g_ecs_render_layer = new renderLayer("ecs");
	g_ecs_render_layer->resetLayer = new renderLayer("ecs.reset",true,true);
	applyRenderOptions();
	addLayer(g_layer3D, g_ecs_render_layer);

	g_ecs_render_layer_2d = new renderLayer("ecs2d");
	g_ecs_render_layer_2d->resetLayer = new renderLayer("ecs2d.reset",true,true);
	addLayer(g_layer2D, g_ecs_render_layer_2d);

	options.layer_3d = g_ecs_render_layer;
	options.layer_2d = g_ecs_render_layer_2d;

	g_globalChannel->addListener(this);
}

void ecs_render_system_kind::applyRenderOptions(){
	g_ecs_render_layer->resetLayer->clear();
	setLayer(g_ecs_render_layer->resetLayer);
	options.apply();
}

void ecs_render_system_kind::onEvent(eventKind event){
	if(event.type != EVENT_FRAME){return;}

	g_ecs_render_layer->clear();
	g_ecs_render_layer->reset();

	g_ecs_render_layer_2d->clear();
	g_ecs_render_layer_2d->reset();

	setLayer(g_ecs_render_layer);
	for(auto I = g_entities.begin(); I != g_entities.end(); I++){
		entity *E = *I;
		if(E->r){

			E->r->render(&options);
		}
	}

	setTexturing(false);
	if(options.boundingBoxes){
		setColor({0,255,0});
		setRenderMode(2);
		for(auto I = g_entities.begin(); I != g_entities.end(); I++){
			entity *E = *I;
			if(E->body){
				E->body->render(&options);
			}
		}
	}

}
//---------------- PHYSICS --------------------------------------
ecs_physics_system_kind::ecs_physics_system_kind(){
	options.gravity = true;
	g_globalChannel->addListener(this);
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
	if(!g_physicsOn){return;}
	if(event.type != EVENT_FRAME){return;}
	if(g_gamePaused){return;}
	for(auto I = g_entities.begin(); I != g_entities.end(); I++){
		entity *E = *I;
		if(E->body){
			if(options.gravity){applyGravity(*I);}
			applyVelocity(*I);
		}
	}
	for(auto I = g_entities.begin(); I != g_entities.end();){
		if(!isSane(*I)){
			delete *I;
			I = g_entities.erase(I);
		}else{
			I++;
		}
	}
}

//---------------- COLLISION ------------------------------------
ecs_collision_system_kind::ecs_collision_system_kind(){
	options.separate = true;
	options.resolve = true;
	g_globalChannel->addListener(this);
}

void ecs_collision_system_kind::onEvent(eventKind event){
	if(!g_collisionOn){return;}
	if(event.type != EVENT_FRAME){return;}
	extern octree_node* g_octree_root;
	broadphaseinfo *bp = checkCollisionBroadphase(g_octree_root);
	for(auto I = bp->pairs.begin(); I != bp->pairs.end(); I++){
		pairwiseCollisionCheck(I->first,I->second,options);
	}
	int nbodies, npairs, efficiency;
	nbodies = bp->bodies.size();
	npairs = bp->pairs.size();
	if(nbodies){
		efficiency = 100*npairs/(nbodies*nbodies);
	}else{
		efficiency = 0;
	}
	if(bp){frameprint(fstring("%d bodies, %d pairs (%d%% of N^2)",nbodies,npairs,efficiency));}
	delete bp;
}
