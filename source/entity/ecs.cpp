#include "ecs.h"
#include "entity.h"
#include "renderLayer.h"
#include "paint.h"
#include "globals.h"
#include "collision.h"

#include "frameprinter.h"

ecs_kind ecs;
renderLayer *ecs_render_layer;
renderLayer *ecs_render_layer_2d;
bool physicsOn = true;
bool collisionOn = true;
bool texturingOn = true;

extern bool gamePaused;
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
	ecs_render_layer->resetLayer->clear();
	setLayer(ecs_render_layer->resetLayer);
	options.apply();
}

void ecs_render_system_kind::onEvent(eventKind event){
	if(event.type != EVENT_FRAME){return;}

	ecs_render_layer->clear();
	ecs_render_layer->reset();

	ecs_render_layer_2d->clear();
	ecs_render_layer_2d->reset();

	setLayer(ecs_render_layer);
	for(auto I = entities.begin(); I != entities.end(); I++){
		entity *E = *I;
		if(E->r /*&& E->body*/){
			/* setColor(E->r->color);
			//if we end up with renderables that don't have a position,
			//then this func will change.
			setPosition(E->body->pos);
			setScale(vec3(1,1,1));
			setRotation(vec3(0,0,0));
			setRenderMode(3);
			setLighting(true);
			if(texturingOn && E->r->t){
				setTexturing(true);
				setTexture(E->r->t);
			}else{
				setTexturing(false);
			}
			drawRmodel(E->r->rm); */
			//setLayer(ecs_render_layer);
			E->r->render(&options);
			//setLayer(layerDebug);
			//vec3 P = E->body->pos;
			//vec3 V = E->body->vel;
			//drawArrow(P, P+V, vec3(255,255,255));
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
	options.separate = true;
	options.resolve = true;
	globalChannel->addListener(this);
}

void ecs_collision_system_kind::onEvent(eventKind event){
	if(!collisionOn){return;}
	if(event.type != EVENT_FRAME){return;}
	extern octree_node* octree_root;
	//printf("\nBROADPHASE\n");
	broadphaseinfo *bp = checkCollisionBroadphase(octree_root);
	//printf("END BP\n");
	for(auto I = bp->pairs.begin(); I != bp->pairs.end(); I++){
		pairwiseCollisionCheck(I->first,I->second,options);
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
