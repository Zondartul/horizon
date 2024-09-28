#include "main/main_render.h"
#include "main/main_resource.h"
#include "util/global_vars_render.h"
#include "render/camera.h"
#include "render/renderLayer.h"
//for renderInit
#include "util/global_vars_program.h"
#include "program/window.h"
#include "render/renderLow.h"
#include "render/paint.h"
#include <iostream>
using namespace std;

void initHorizonRender() {
	static bool initialized = false;
	if (initialized) { return; }
	initialized = true;

	initHorizonResource();

	Gr = new GlobalsRender();

	renderInit();
	setupLayers();

	auto& camera = Gr->gs_camera->g_camera;
	camera.setPos({ -0.5,0,0 });
}

void renderInit() {
	auto& width = Gp->gs_window->g_width;
	auto& height = Gp->gs_window->g_height;
	//g_renderLow = new renderLowKind();
	bool debug_split = false;
	if (debug_split) {
		auto* splitter = new renderLow_Splitter();
		splitter->children.push_back(new renderLow_SDL());
		splitter->children.push_back(new renderLow_Soft());
		g_renderLow = splitter;
	}
	else {
		g_renderLow = new renderLow_SDL();
	}
	cout << "--- render init put renderLow_Soft here ---" << endl;
	g_renderLow->renderLowInit();
	g_renderLow->setViewportSize(width, height);
	//g_renderLow->resetOptions(); /// causes [bug 16]
	initLayers();
	//printf("-------- render init done -----\n");
	cout << "-------- render init done -----" << endl;
}
