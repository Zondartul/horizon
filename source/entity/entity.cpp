#include "entity.h"
#include "globals.h"
#include "paint.h"
list<entity*> entities;
ecs_render_system_kind ecs_render_system;

imp_component0(vec3, position);
imp_component1(rmodel*, rmodel);
imp_component1(texture*, texture);

entity::entity(){
	entities.push_back(this);
}	//lol no delete

extern renderLayer *layer3D;
void component_rmodel::setup(){setLayer(layer3D);uploadRmodel(val);}
void component_texture::setup(){setLayer(layer3D);uploadTexture(val);}

void ecs_render_system_kind::render(){
	setLayer(layer3D);
	for(auto I = entities.begin(); I != entities.end(); I++){
		if(hasComponent(*I,rmodel)){
			setPosition(getComponent(*I,position)->val);
			if(hasComponent(*I,texture)){
				setTexturing(true);
				setTexture(getComponent(*I,texture)->val);
			}else{
				setTexturing(false);
			}
			drawRmodel(getComponent(*I,rmodel)->val);
		}
	}
}
