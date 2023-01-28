#include <cstdio>
#include <cstdlib>
#include <map>
#include "Ext/glm/glm.hpp"
#ifndef NO_GLEW
#include "Ext/GL/glew.h"
#endif
#include "render/renderLow.h"
#include "math/vec.h"
#include "resource/fonts.h"
#include "util/util.h"
#include "render/paint.h"
#include "util/globals_render.h"
#include "program/file.h"
#include "program/window.h"
#include "render/camera.h"
#include "render/printw.h"
#include "render/GPUdriver.h"
#include "resource/bitmap.h"
#include "resource/model.h"
#include "util/global_vars_render.h"
#include "util/global_vars_program.h"
#include "input/input.h" /// for sysInput->inputChannel

using glm::mat4;
using glm::vec3;
using glm::vec2;
using std::map;

extern model* g_mBox;

/// ------- renderLowKind ---------------

renderLowKind::renderLowKind() {}
renderLowKind::~renderLowKind() {}


/// --------- renderLow_SDL ----------------

renderLow_SDL::renderLow_SDL(){
    Gr = new GlobalsRender();

	GPUdriver = new GPUdriverKind();
    vec2 size = getScreenSize();
    setViewportSize((int)size.x,(int)size.y);

    auto& inputChannel = Gp->sysInput->inputChannel; /// for event_window_resize /// btw we should make individual events subscribable
    inputChannel.addListener(this);
}

renderLow_SDL::~renderLow_SDL() {}

void renderLow_SDL::renderLowInit(){
    GPUdriver->renderLowInit();
}

void renderLow_SDL::setViewportSize(int width, int height){
    GPUdriver->renderState.width = width;
    GPUdriver->renderState.height = height;
    ProgramResizeViewport(width, height);
}

void renderLow_SDL::onEvent(eventKind event)
{
    if (event.type == EVENT_WINDOW_RESIZE) {
        setViewportSize(event.windowresize.width, event.windowresize.height);
    }
}

void renderLow_SDL::renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L){

    int cmdNum = 0;
    renderCommand3 *rcmd = 0;
    try{
        for(auto I = rqueue->begin(); I != rqueue->end(); I++){
            rcmd = *I;
            parseCommand(*rcmd);
            cmdNum++;
        }
    }catch(exKind &e){
        static bool FRsaved = false;
        if(!FRsaved){
            printf("exception occurred during render, saving frame report\n");
            frameReport();
            FRsaved = true;
        }
        try{
        string extra = string()+"\nwhere layer = ["+(L? toString(L) : "<null>")+"], rcmd #"+cmdNum+" = ["+(rcmd? toString(rcmd) : "<null>")+"]";
        e.msg.msg += extra;
        }catch(...){}

        throw;
    }
}

void renderLow_SDL::parseCommand(const renderCommand3 &rcmd){
        bool passCommand = true;
       switch(rcmd.type){
            case(COLORING):
                options.coloring = rcmd.b;
            break;

            case(UVCOLORING):
                options.uvColoring = rcmd.b;
            break;

            case(NORMALCOLORING):
                options.normalColoring = rcmd.b;
            break;

            case(TRANSPARENCY):
                options.transparency = rcmd.b;
            break;

            case(DEPTHMASK):
                options.depthMask = rcmd.b;
            break;

            case(TEXTURING):
                options.texturing = rcmd.b;
            break;

            case(DEBUG):
                options.debug = rcmd.b;
            break;

            case(PROJECTION):
                options.cpj = rcmd.cam;
            break;

            case(POSITION):
                options.pos = rcmd.v3;
            break;

            case(ROTATION):
                options.rot = rcmd.v3;
            break;

            case(SCALE):
                options.scale = rcmd.v3;
            break;

            case(TEXTURE_SELECT):
                options.t = rcmd.t;
            break;

            case(SCISSORING):
                options.scissoring = rcmd.b;
            break;

            case(COLOR):
                options.color = rcmd.v3;
            break;

            case(ALPHA):
                options.alpha = rcmd.f;
            break;

            case(FONT_SELECT):
                options.fnt = rcmd.fnt;
            break;

            case(MODE_SELECT):
                options.renderMode = rcmd.i;
            break;

            case(TEXT_POS):
                options.textPos = rcmd.v2;
            break;

            case(SCISSOR):
                options.scissor = rcmd.r;
            break;

            case(POINTSIZE):
                options.pointSize = rcmd.f;
            break;

            case(LINEWIDTH):
                options.lineWidth = rcmd.f;
            break;

          

            case(COMMENT):
                if(options.debug){printf("%s", rcmd.s->c_str());}
                 //should have some way of adding/removing commands i.e. stream goes in, different stream comes out
                passCommand = false;
            break;

            case(DEPTH_TEST):
                options.depthTest = rcmd.b;
            break;

            case(LIGHTING):
                options.lighting = rcmd.b;
            break;
            case(SUN_POS):{
                options.sunPos = rcmd.v3;
            }break;

            case(SUN_LIGHT_COLOR):
                options.sunLightColor = rcmd.v3;
            break;

            case(AMBIENT_LIGHT_COLOR):
                options.ambientLightColor = rcmd.v3;
            break;

            case(PUSH_OPTIONS):
                optionStack.push_back(options);
            break;

            case(POP_OPTIONS):
                options = optionStack.back();
                optionStack.pop_back();
            break;
		}
        if(passCommand){
            GPUdriver->parseCommand(rcmd);
        }
}

/// --------------- renderLow_Soft



renderLow_Soft::renderLow_Soft() {
}

renderLow_Soft::~renderLow_Soft() {}

void renderLow_Soft::renderLowInit() {
}

void renderLow_Soft::setViewportSize(int width, int height) {
}

void renderLow_Soft::onEvent(eventKind event)
{
    if (event.type == EVENT_WINDOW_RESIZE) {
        setViewportSize(event.windowresize.width, event.windowresize.height);
    }
}

void renderLow_Soft::renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L) {

}

void renderLow_Soft::parseCommand(const renderCommand3& rcmd) {

}

/// ---------- renderLow_Splitter --------------------

renderLow_Splitter::renderLow_Splitter() {}
renderLow_Splitter::~renderLow_Splitter() {}

void renderLow_Splitter::renderLowInit() {
    for (auto& child : children) { child->renderLowInit(); }
}

void renderLow_Splitter::setViewportSize(int width, int height) {
    for (auto& child : children) { child->renderLowInit(); }
}

void renderLow_Splitter::onEvent(eventKind event)
{
    for (auto& child : children) { child->onEvent(event); }
}

void renderLow_Splitter::renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L) {
    for (auto& child : children) { child->renderParseQueue(rqueue, L); }
}

void renderLow_Splitter::parseCommand(const renderCommand3& rcmd) {
    for (auto& child : children) { child->parseCommand(rcmd); }
}
