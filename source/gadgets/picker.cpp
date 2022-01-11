#include "picker.h"
#include "GUI.h"
#include "entity.h"
#include "stringUtils.h"
#include "renderLayer.h"
#include "paint.h"
#include "keybinds.h"
#include "mouse.h"
#include "camera.h"
#include "rmodel.h"
#include "window.h"

GUIwindow *entWindow = 0;
extern GUIbase *GUI;
extern cameraKind camera;

void closeEntityOptionsWindow(){
	//if(entWindow){entWindow->close(); delete entWindow; entWindow = 0;}
	if(entWindow){delete entWindow; entWindow = 0;}
}

void openEntityOptionsWindow(entity *E){
	closeEntityOptionsWindow();
	entWindow = new GUIwindow();
	entWindow->setSize({200,300});
	entWindow->moveTo({100,20});
	
	float windY = 10;
	
	GUIlabel *entName = new GUIlabel();
	entName->setText(E->name);
	entName->sizeToContents();
	entName->moveTo({0,windY});
	entWindow->addChild(entName);
	
	windY += entName->area.size.y;
	
	if(E->body){//hasComponent(E,position)){
		vec3 pos = E->body->pos;//getComponent(E,position)->val;
		
		GUIlabel *lblProp = new GUIlabel();
		lblProp->setText("pos");
		lblProp->sizeToContents();
		lblProp->moveTo({0,windY});
		entWindow->addChild(lblProp);
		
		GUIlabel *lblVal = new GUIlabel();
		lblVal->setText(toString(pos));
		lblVal->sizeToContents();
		lblVal->moveTo({50,windY});
		entWindow->addChild(lblVal);
		
		windY += lblProp->area.size.y;
	}
	if(GUI){GUI->addChild(entWindow);}else{printf("no gui\n");}
}

void pickerSelect(entity *E){
	if(E->r){//hasComponent(E,renderable)){
		//renderable *r = getComponent(E,renderable)->val;
		E->r->color = vec3(0,255,0);
	}
}
void pickerDeselect(entity *E){
	if(E->r){//hasComponent(E,renderable)){
		//renderable *r = getComponent(E,renderable)->val;
		E->r->color = vec3(255,255,255);
	}
}

entity *pickedEntity = 0;
void initPicker(){
	renderLayer *pickerLayer = new renderLayer();
	addLayer(layer3D,pickerLayer);
	setLayer(pickerLayer);
	setPosition(vec3(0,0,0));
	setScale(vec3(1,1,1));
	setColoring(false);
	setTexturing(false);
	setLighting(false);
	setPointSize(3);
	
	keybinds->binds["LMB"].keyDown = [=](eventKind event){	
		event.maskEvent();
		
		setLayer(pickerLayer);
		setColor(vec3(0,255,0));
		setPointSize(3);
		
		vec2 mousepos = getMousePos();
		vec3 mousedv = camera.screenToWorld(vec3(mousepos.x,mousepos.y,1));
		vec3 dv = normalizeSafe(mousedv - camera.pos);
		printf("LMB: %s, dv: %s\n",toString(mousepos).c_str(),toString(dv).c_str());
		collisioninfo *col = raytrace(camera.pos,dv);
		if(col){
			float depth = col->c_to_c.depth;
			vec3 hitpos = col->c_to_c.pos;
			//entity *E1 = col->cp[0].ent;
			entity *E2 = col->body2->E;//col->cp[1].ent;
			printf("hit: dist = %f, pt = %s\n",depth,toString(hitpos).c_str());
			printf("entity: [%s]\n",E2->name.c_str());
			
			if(pickedEntity){pickerDeselect(pickedEntity);}
			pickedEntity = E2;
			pickerSelect(E2);
			openEntityOptionsWindow(E2);
			
			
			//drawPoint(camera.pos);
			//drawPoint(hitpos);
			//drawLine(camera.pos,hitpos);
		}else{
			printf("no hit\n");
			if(pickedEntity){pickerDeselect(pickedEntity);}
			pickedEntity = 0;
			closeEntityOptionsWindow();
		}
	};
	keybinds->binds["RMB"].keyDown = [=](eventKind event){
		event.maskEvent();
		
		setLayer(pickerLayer);
		setPointSize(1);
		rmodel *rm1 = new rmodel();
		rmodel *rm2 = new rmodel();
		
		vec2 scr = getScreenSize();
		//printf("scr size: %s\n",toString(scr).c_str());
		int div = 50;
		for(int I = 0; div*I < scr.x; I++){
			for(int J = 0; div*J < scr.y; J++){
				vec3 dv = camera.screenToWorld(vec3(div*I,div*J,1));
				dv = normalizeSafe(dv - camera.pos);
				collisioninfo *col = raytrace(camera.pos,dv);
				if(col){
					vec3 hitpos = col->c_to_c.pos;
					rm1->vertices->push_back(hitpos);
					vec3 hitpos2 = col->cpts[0].pos;
					rm2->vertices->push_back(hitpos2);
				}
			}
		}
		rm1->finalize();
		rm2->finalize();
		setLayer(loadLayer);
		uploadRmodel(rm1);
		uploadRmodel(rm2);
		setLayer(pickerLayer);
		setRenderMode(1);
		setColor(vec3(255,0,0));
		drawRmodel(rm1);
		setColor(vec3(0,255,0));
		drawRmodel(rm2);
	};
}