#include "render.h"
#include "paint.h"
#include "renderLayer.h"
#include "simplemath.h"
#include "editmodel.h"
#include "main.h"
#include "model.h"
#include "simplemath.h"
#include "stringUtils.h"
#include "timer.h"
#include "renderLayer.h"
#include "window.h"
#include "renderLow.h"
#include "global_vars.h"
void renderOptions::apply(renderLayer *L){
	auto& currentLayer = Gb->gs_paint->g_currentLayer;
	if(!L){L = currentLayer;}
    renderLayer *oldLayer = currentLayer;
    currentLayer = L;
	setColoring(coloring);
	setTexturing(texturing);
	setLighting(lighting);
	setColor(color);
	setAlpha(alpha);
	if(wireframe){
		setRenderMode(2);
	}else{
        setRenderMode(renderMode);
    }
	setUvColoring(uvColoring);
	setNormalColoring(normalColoring);
	setScissoring(scissoring);
	setScissor(scissor);
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
    currentLayer = oldLayer;
}
void renderOptions::applyImmediate(){
    renderLayer *tempLayer = new renderLayer("renderOptions::applyImmediate",false,true);
    apply(tempLayer);
    tempLayer->render();
    delete tempLayer;
}
void renderableSprite::upload(){}
void renderableSprite::render(renderOptions *options){
	auto& camera = Gb->gs_camera->g_camera;
    setTexturing(true);
    setTransparency(true);
    setLighting(false);
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
    drawRect(R);
}
void renderableModel::upload(){
	auto& layer3D = Gb->gs_paint->g_layer3D;
	setLayer(layer3D);
	uploadRmodel(rm_default);
	uploadRmodel(rm_wireframe);
	if(t){uploadTexture(t);}
}
void renderableModel::render(renderOptions *options){
	setPosition(pos);
	setScale(vec3(1,1,1));
	setRotation(rot);
	setColor(color);
	if(t){
		setTexture(t);
	}
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
		(*I)->pos = pos+offsets[i];
		(*I)->render(options);
		i++;
	}
}
void renderablePlane::upload(){
	auto& layer3D = Gb->gs_paint->g_layer3D;
	setLayer(layer3D);
	if(!rm){
	}
	uploadRmodel(rm);
	if(t){uploadTexture(t);}
}
void renderablePlane::render(renderOptions *options){
	auto& camera = Gb->gs_camera->g_camera;
	float dist = dot(camera.pos,normal)-offset;
	vec3 v = camera.pos-normal*dist;
	vec3 vh = v-dot(v,normal);
	vec3 vright = normalizeSafe(cross(normal, vec3(0,0,1)));
	if(normal == vec3(0,0,1)){vright = vec3(1,0,0);}
	if(normal == vec3(0,0,-1)){vright = vec3(-1,0,0);}
	vec3 vup = normalizeSafe(cross(normal,vright));
	if(dot(normal,vec3(1,1,1))<0){vright = -vright;}
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
	setTexturePosition(vec2(0,0));
	setTextureScale(vec2(1,1));
}
void renderTick(){
	auto& layer3D = Gb->gs_paint->g_layer3D;
	auto& layer2D = Gb->gs_paint->g_layer2D;
	auto& loadLayer = Gb->gs_paint->g_loadLayer;
	auto& layerDebug = Gb->gs_paint->g_layerDebug;
	auto& layerDebug2D = Gb->gs_paint->g_layerDebug2D;
	auto& deleteLayer = Gb->gs_paint->g_deleteLayer;
	auto& m = Gb->gs_main->g_m;
	auto& GUI = Gb->gs_main->g_GUI;
	auto& fps = Gb->gs_main->g_fps;
	auto& frametime = Gb->gs_main->g_frametime;
	setLayer(layer2D);
	clearScreen();
	go2D();
	static float fps_filtered = 60.f;
	static float frametime_filtered = 0.015f;
	fps_filtered = mix2(fps_filtered,fps,1.0f/200.0f);
	frametime_filtered = mix2(frametime_filtered,frametime,1.0f/200.0f);
	if(GUI){GUI->renderLogic();}
	renderAllLayers();
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
	deleteLayer->clear();
	deleteLayer->reset();
	OpenGL_swap();
}
void renderInit(){
	auto& width = Gb->gs_window->g_width;
	auto& height = Gb->gs_window->g_height;
	g_renderLow = new renderLowKind();
	g_renderLow->renderLowInit();
	g_renderLow->setViewportSize(width, height);
	initLayers();
	printf("-------- render init done -----\n");
}
