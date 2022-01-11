#include "render.h"
#include "paint.h"
#include "renderLayer.h"
#include "simplemath.h"

bool texturingOn;
renderLayer *currentLayer;

/*
void renderOptions::apply(renderLayer *L){
    if(!L){L = currentLayer;}
    renderLayer *oldLayer = currentLayer;
    currentLayer = L;
	//-----------
	setColoring(coloring);
	setTexturing(texturing);
	setLighting(lighting);
	setColor(color);//vec3(255,255,255));
	setAlpha(alpha);//255);
	if(wireframe){
		setRenderMode(2);
	}else{
		//setRenderMode(3);
        setRenderMode(renderMode);
    }
	setUvColoring(uvColoring);
	setNormalColoring(normalColoring);
	setScissoring(scissoring);
	setScissor(scissor);
    //---------------------
    setTransparency(transparency);
    setDepthMask(depthMask);
    setDepthTest(depthTest);
    setDebug(debug);
    if(cpj){setProjection(*cpj);}
    setPosition(pos);
    setRotation(rot);
    setScale(scale);
    if(t){setTexture(t);}
    if(fnt){setFont(fnt);}
    setTextPos(textPos);
    setPointSize(pointSize);
    setLineWidth(lineWidth);
    setSunPos(sunPos);
    setSunColor(sunLightColor);
    setAmbientColor(ambientLightColor);
	setTexturePosition(texturePos);
	setTextureScale(textureScale);
    //---------------------
    currentLayer = oldLayer;
}

void renderOptions::applyImmediate(){
    renderLayer *tempLayer = new renderLayer("renderOptions::applyImmediate",false,true);
    apply(tempLayer);
    tempLayer->render();
    delete tempLayer;
}

vector<renderOptions> renderOptionsStack;
*/
/*
void renderableSprite::upload(){}

void renderableSprite::render(renderOptions *options){
    

    setTexturing(true);
    setTransparency(true);
    setLighting(false);
    //setDepthTest(false);
    setPosition(pos);
    vec3 dv = camera.pos - pos;
    float ang = atan2(dv.y,dv.x);

    quat q1 = glm::angleAxis(-90*d2r,vec3(1,0,0));
    quat q2 = glm::angleAxis(ang+90*d2r,vec3(0,0,1));
    quat q3 = q2*q1;
    vec3 rot = axis(q3)*angle(q3);
    setRotation(rot);
    setScale(vec3(1,1,1)*0.01f);
    setTexture(t);
    setColor(color);
    //osetPosition(pos);
    drawRect(R);

}

void renderableModel::upload(){
	setLayer(layer3D);
	uploadRmodel(rm_default);
	uploadRmodel(rm_wireframe);
	if(t){uploadTexture(t);}
}

void renderableModel::render(renderOptions *options){
	setPosition(pos);
	setScale(vec3(1,1,1));
	//setRotation(vec3(0,0,0));
	setRotation(rot);
	setColor(color);
	//setRenderMode(3);			//renderable can supply render data but can't
	//setLighting(true);		//change render options
	if(t){
		//setTexturing(true);
		setTexture(t);
	}//else{
		//setTexturing(false);
	//}
	if(options && options->wireframe){
		drawRmodel(rm_wireframe);
	}else{
		drawRmodel(rm_default);
	}
}

void renderableMultimodel::upload(){
	for(auto I = parts.begin(); I != parts.end(); I++){
		offsets.push_back((*I)->pos - pos);
		(*I)->upload();
	}
}

void renderableMultimodel::render(renderOptions *options){
	int i = 0;
	for(auto I = parts.begin(); I != parts.end(); I++){
		//a hack
		(*I)->pos = pos+offsets[i];
		(*I)->render(options);
		i++;
	}
}
#include "modelprimitives.h"
#include "editmodel.h"
void renderablePlane::upload(){
	setLayer(layer3D);
	
	if(!rm){
		e_model *em = generatePlane(normal, 0.f, 1000.f*vec2(1.f,1.f), texscale);
		rmpack rms = em->getRmpack();
		rm = rms.rm_tris;
		delete em;
	}
	
	uploadRmodel(rm);
	if(t){uploadTexture(t);}
}

void renderablePlane::render(renderOptions *options){
	
	float dist = dot(camera.pos,normal)-offset;
	
	vec3 v = camera.pos-normal*dist;
	
	vec3 vh = v-dot(v,normal);
	vec3 vright = normalizeSafe(cross(normal, vec3(0,0,1)));
	if(normal == vec3(0,0,1)){vright = vec3(1,0,0);}
	if(normal == vec3(0,0,-1)){vright = vec3(-1,0,0);}
	
	
	vec3 vup = normalizeSafe(cross(normal,vright));
	if(dot(normal,vec3(1,1,1))<0){vright = -vright;}
	//if(vright == vec3(1,0,0)){vup = vec3(0,0,1);}
	//if(vup.z < 0){vup = -vup;}
	//if(side < 0){vright = -vright;}
	//if(dist<0){vright = -vright;}
	
	//printf("norm %s, right %s, up %s\n",toCString(normal), toCString(vright), toCString(vup));
	//printf("rplane normal %s, camfwd %s\n",toCString(normal),toCString(camera.forward()));
	//printf("rplane %p: vright %s, vup %s, dist %.1f\n",this,toCString(vright),toCString(vup),dist);
	//printf("\n");
	vec2 texPos = vec2(dot(vh,vright),-dot(vh,vup));
	
	setPosition(v);
	if(dist < 0){dist = -dist;}
	if(dist == 0){dist = 1.f;}
	vec3 s = dist*vec3(1,1,1);
	setScale(s);
	vec2 v2 = vec2(-texPos/dist);
	setTexturePosition(v2);
	setTextureScale(s);
	
	if(t){setTexture(t);}
	drawRmodel(rm);
	//restore previous options
	setTexturePosition(vec2(0,0));
	setTextureScale(vec2(1,1));
}

#include "main.h"
#include "model.h"
#include "frameprinter.h"
#include "simplemath.h"
#include "stringUtils.h"
#include "timer.h"
#include "inputController.h"
#include "renderLayer.h"
#include "window.h"
#include "renderLow.h"


void renderTick(){
	//glClearColor(0.3,0.7,0.9,1.0);
	//glClearColor(0.1,0.23,0.3,1.0);
	//========================= 3D ==============================

	setLayer(layer3D);
	go3D();
	clearScreen();
	drawTriangle(vec3(0,-0.67,-0.5),vec3(0,0,0.5),vec3(0,0.67,-0.5));
	setColoring(true);
	setTextPos(vec2(0,0));
	printText("Hello World [ ijk XYZ o_O ] ");

	setColoring(false);
	setTexturing(true);
	setTexture(m->t);
	drawRmodel(m->rm);
	renderComment("comment: layer3D done\n");
	//octreeRender(octree_root);

	//done in ecs_collision_system now
	//broadphaseinfo *bp = checkCollisionBroadphase(octree_root);
	//frameprint(fstring("bp: %p",bp));
	//if(bp){frameprint(fstring("%d bodies, %d pairs",bp->bodies.size(),bp->pairs.size()));}
	//broadphaseRender(bp);
	//delete bp;
	//test1render();
	//octreePrint(octree_root);

	//getComponent(E,position)->val = vec3(0,1,3+0.25*sin(t2));
	//why the fuck is this in render

	//========================= 2D ==============================
	//setColor({255,255,255});
	//drawRmodel(m->rm);
	//printw("Hello World!");
	//textPos = {0.0f,25.0f};//{100.0f,100.0f};
	//renderCmd(RCMD::DEBUG,b=true);
	//setColoring(false);
	//setTextPos(vec2(0,0));
	//printText("Hello World [ ijk XYZ o_O ] ");
	debugFloatingText(vec3(0,0,0),"test");
	setLayer(layer2D);
	go2D();
	frameprint("Hello World [ ijk XYZ o_O ] ");

	//setTextPos(vec2(0,20));
	static float fps_filtered = 60.f;
	static float frametime_filtered = 0.015f;
	fps_filtered = mix2(fps_filtered,fps,1.0f/200.0f);
	frametime_filtered = mix2(frametime_filtered,frametime,1.0f/200.0f);
	//printText(string("FPS: ")+ftoa(fps_filtered,1)+", frametime: "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	frameprint(string("FPS (filtered): ")+ftoa(fps_filtered,1)+", frametime (filtered): "+ftoa(1000*frametime_filtered,3)+"ms");//(int)(1000*round2(frametime_filtered,4))
	frameprint(string("FPS: ")+ftoa(fps,1)+", frametime: "+ftoa(1000*frametime,3)+"ms");
	//setTextPos(vec2(0,40));
	//printText(string("T1: ")+t);
	//setTextPos(vec2(0,60));
	//printText(string("cam speed: ")+inputController.velocity.length());
	frameprint(fstring("T1: %d",getGameTicks()));
	frameprint(string("cam speed: ")+length(inputController->velocity));
	if(GUI){GUI->renderLogic();} //should be event-driven


	// renderLayer(loadLayer); durrr, renderLayer(renderLayer)
	// renderLayer(layer3D);
	// renderLayer(layer2D);
	// renderLayer(deleteLayer);

	// clearLayer(loadLayer);
	// clearLayer(layer3D);
	// clearLayer(layer2D);
	// clearLayer(deleteLayer);
	renderAllLayers();
	//loadLayer->render();
	//layer3D->render();
	//layer2D->render();
	//deleteLayer->render();

	loadLayer->clear();
	loadLayer->reset();
	layer3D->clear();
	layer3D->reset();
	layerDebug->clear();
	layerDebug->reset();
	layer2D->clear();
	layer2D->reset();
	layerDebug2D->clear();
	layerDebug2D->reset();
	deleteLayer->clear();	//this should be either in paint or a layer's option
	deleteLayer->reset();

	OpenGL_swap();	//this should be moved to renderLow
}

void renderInit(){
	renderLow = new renderLowKind();
	renderLow->renderLowInit();
	initLayers();
	printf("-------- render init done -----\n");
}

//void renderable::upload(){
//	setLayer(layer3D);
//	uploadRmodel(rm);
//	if(t){uploadTexture(t);}
//}


// void renderTriangle(){
	// rmodel *rm = new rmodel();

	// rm->vertices->push_back(vec3(0,0,0));
	// rm->vertices->push_back(vec3(0.5,0,0));
	// rm->vertices->push_back(vec3(0.5,0.5,0));
	// rm->vertices->push_back(vec3(0.5,0,0));
	// rm->vertices->push_back(vec3(0.5,0.5,0));
	// rm->vertices->push_back(vec3(1,0.5,0));

	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(0,1,0));
	// rm->colors->push_back(vec3(0,0,1));
	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(0,1,0));
	// rm->colors->push_back(vec3(0,0,1));

	// rm->finalize();

	// loadLayer.uploadRmodel(rm);
	// layer3D.setColoring(true);
	// layer3D.setTexturing(false);
	// layer3D.renderRmodel(rm);
	// deleteLayer.deleteRmodel(rm);
// }
*/