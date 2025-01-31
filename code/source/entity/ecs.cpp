#include "ecs.h"
#include "entity.h"
#include "renderLayer.h"
#include "paint.h"
#include "globals.h"
#include "collision.h"
#include "frameprinter.h"
#include "main.h" //g_gamePaused

//ecs_kind g_ecs;
//renderLayer *g_ecs_render_layer;
//renderLayer *g_ecs_render_layer_2d;

//bool g_physicsOn = true;
//bool g_collisionOn = true;
//bool g_texturingOn = true;

//extern bool g_gamePaused;
//---------------- RENDER ---------------------------------------
ecs_render_system_kind::ecs_render_system_kind(){
	auto& ecs_render_layer = G->gs_ecs->g_ecs_render_layer;
	auto& ecs_render_layer_2d = G->gs_ecs->g_ecs_render_layer_2d;
	auto& layer3D = G->gs_paint->g_layer3D;
	auto& layer2D = G->gs_paint->g_layer2D;
	auto& globalChannel = G->gs_event->g_globalChannel;

	options.wireframe = false;
	options.texturing = true;
	options.lighting = true;
	options.coloring = false;
    options.color = vec3(255,255,255);
    options.alpha = 255;
    options.sunPos = vec3(0.5,0.75,1);
	options.sunLightColor = 0.9f*vec3(1,1,1);
	options.ambientLightColor = 0.3f*vec3(1,1,1);

	ecs_render_layer = new renderLayer("ecs");
	ecs_render_layer->resetLayer = new renderLayer("ecs.reset",true,true);
	applyRenderOptions();
	addLayer(layer3D, ecs_render_layer);

	ecs_render_layer_2d = new renderLayer("ecs2d");
	ecs_render_layer_2d->resetLayer = new renderLayer("ecs2d.reset",true,true);
	addLayer(layer2D, ecs_render_layer_2d);

	options.layer_3d = ecs_render_layer;
	options.layer_2d = ecs_render_layer_2d;

	globalChannel->addListener(this);
}

void ecs_render_system_kind::applyRenderOptions(){
	auto& ecs_render_layer = G->gs_ecs->g_ecs_render_layer;

	ecs_render_layer->resetLayer->clear();
	setLayer(ecs_render_layer->resetLayer);
	options.apply();
}

void ecs_render_system_kind::onEvent(eventKind event){
	auto& ecs_render_layer = G->gs_ecs->g_ecs_render_layer;
	auto& ecs_render_layer_2d = G->gs_ecs->g_ecs_render_layer_2d;
	auto& entities = G->gs_entity->g_entities;

	if(event.type != EVENT_FRAME){return;}

	ecs_render_layer->clear();
	ecs_render_layer->reset();

	ecs_render_layer_2d->clear();
	ecs_render_layer_2d->reset();

	setLayer(ecs_render_layer);
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->r){

			E->r->render(&options);
		}
	}

	setTexturing(false);
	if(options.boundingBoxes){
		setColor({0,255,0});
		setRenderMode(2);
		for(auto I = entities.begin(); I != entities.end(); I++){
			entity *E = *I;
			if(E->body){
				E->body->render(&options);
			}
		}
	}

}
//---------------- PHYSICS --------------------------------------
ecs_physics_system_kind::ecs_physics_system_kind(){
	auto& globalChannel = G->gs_event->g_globalChannel;
	options.gravity = true;
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
	auto& physicsOn = G->gs_ecs->g_physicsOn;
	auto& gamePaused = G->gs_main->g_gamePaused;
	auto& entities = G->gs_entity->g_entities;

	if(!physicsOn){return;}
	if(event.type != EVENT_FRAME){return;}
	if(gamePaused){return;}
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->body){
			if(options.gravity){applyGravity(*I);}
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
	auto& globalChannel = G->gs_event->g_globalChannel;

	options.separate = true;
	options.resolve = true;
	globalChannel->addListener(this);
}

void ecs_collision_system_kind::onEvent(eventKind event){
	auto& collisionOn = G->gs_ecs->g_collisionOn;
	auto& octree_root = G->gs_main->g_octree_root;

	if(!collisionOn){return;}
	if(event.type != EVENT_FRAME){return;}
	//extern octree_node* g_octree_root;
	broadphaseinfo *bp = checkCollisionBroadphase(octree_root);
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
