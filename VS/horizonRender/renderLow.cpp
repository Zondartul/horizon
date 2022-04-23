#include "renderLow.h"
#include "globals.h"
#include "vec.h"
#include "stdio.h"
#include "stdlib.h"

#ifndef NO_GLEW
#include "GL/glew.h"
#endif
#include "file.h"
#include "window.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "printw.h"
#include "GPUdriver.h"
using glm::mat4;
using glm::vec3;
using glm::vec2;
#include "model.h"
extern model *g_mBox;
#include "bitmap.h"
#include <map>
using std::map;

#include "fonts.h"
#include "util.h"
#include "paint.h"

renderLowKind *g_renderLow;
renderLowKind::renderLowKind(){
	GPUdriver = new GPUdriverKind();
    vec2 size = getScreenSize();
    setViewportSize((int)size.x,(int)size.y);
}


void renderLowKind::renderLowInit(){
    GPUdriver->renderLowInit();
}

void renderLowKind::setViewportSize(int width, int height){
    GPUdriver->renderState.width = width;
    GPUdriver->renderState.height = height;
}


void renderLowKind::renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L){

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

void renderLowKind::parseCommand(const renderCommand3 &rcmd){
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
		}
        if(passCommand){
            GPUdriver->parseCommand(rcmd);
        }
}


bool g_debug = false;

