#include "console.h"
#include "renderLayer.h"
#include "paint.h"
#include "keybinds.h"
#include "hook.h"
#include "window.h"
#include "resource.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

bool consoleOn = false;
vector<string> consoleStrings;
renderLayer *consoleLayer;

void initConsole(){
	consoleLayer = new renderLayer();
	consoleLayer->resetLayer = duplicateLayer(layer2D->resetLayer);
	addLayer(layer2D,consoleLayer);
	keybinds->binds["`"].keyDown = [](eventKind event){consoleOn = !consoleOn;};
	
	hookAdd(globalChannel, EVENT_FRAME, "drawConsole",
		[](eventKind event){
			//setLayer(layer3D);
			//setColor(vec3(0,0,0));
			//drawPoint(camera.pos);
			//printf("consoleOn = %d\n",consoleOn);
			consoleLayer->clear();
			consoleLayer->reset();
			if(consoleOn){
				setLayer(consoleLayer);
				setTransparency(true);
				// camera.go2D();
				// setProjection(camera.mProjection*camera.mView);
				// setPosition(vec3(0,0,0));
				// setScale(vec3(1,1,1));
				// setColoring(false);
				// setTransparency(false);
				// setTexturing(false);
				// setScissoring(false);
				// vec2 scr = getScreenSize();
				// setScissor(rect(vec2(0,0),scr));
				// setDepthTest(false);
				// setLighting(false);
				
				//setPosition(vec3(0,0,0));
				setColor(vec3(64,128,64));
				setAlpha(196.f);
				
				vec2 end = getScreenSize();
				//setScissor(rect(vec2(0,0),end));
				end.y = end.y*1/3;
				rect R = rect(vec2(0,0),end);
				//printf("R = %s\n",toString(R).c_str());
				drawRect(R);
				
				setColor(vec3(128,255,128));
				setAlpha(255.f);
				setFont(getFont("cour 14"));
				//printText("Hello World!");
				int numstr = consoleStrings.size();
				for(int I = 0; I < numstr; I++){
					setTextPos(vec2(0,end.y-(numstr-1-I)*14));
					printText(consoleStrings[I]);
				}
			}
		}
	);
}


void cprint(const char *format, ...){
	string str;
	printfify(format,str);
	consoleStrings.push_back(str);
}
