#include "experimental/map_editor.h"
#include "math/geometry.h"
#include "math/simplemath.h"
#include "resource/texture.h"
#include "resource/resource.h"
#include "render/paint.h"
#include "render/renderLayer.h"
#include "GUI/GUI.h"
#include "input/input.h"
#include "input/mouse.h"
#include "util/global_vars_app.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"

void openMapEditor(){
	auto& mapeditor = Ga->gs_map_editor->g_mapeditor;
	if(mapeditor){delete mapeditor;}
	mapeditor = new map_editor_kind();
}
map_editor_kind::map_editor_kind(){
	auto& layer2D = Gr->gs_paint->g_layer2D;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& inputChannel = Gg->sysInput->inputChannel;//Gr->gs_input->g_inputChannel;
	auto& globalChannel = Gr->sysEvent->globalChannel;//Gr->gs_event->g_globalChannel;
	mainWindow = new GUIwindow();
	mainWindow->setSize(vec2(150,500));
	GUIbutton *btnCube = new GUIbutton();
	btnCube->setImage(getTexture("gui/cube2"));
	btnCube->setText("");
	btnCube->setSize(vec2(32,32));
	btnCube->moveTo(vec2(0,0));
	btnCube->setFunction([=](){
		this->mode = ME_MODE_BLOCK_PLACEMENT;
		this->submode = ME_SUBMODE_DEFAULT;
        printf("map editor: block mode\n");
	});
	mainWindow->addChild(btnCube);
	GUIlabel *lbl1 = new GUIlabel();
	lbl1->setText("place blocks");
	lbl1->moveTo(vec2(32,0));
	lbl1->sizeToContents();
	lbl1->setSize(vec2(lbl1->area.size.x,32));
	mainWindow->addChild(lbl1);
	GUIbutton *btnTerrain = new GUIbutton();
	btnTerrain->setImage(getTexture("gui/blanketDark"));
	btnTerrain->setText("");
	btnTerrain->setSize(vec2(32,32));
	btnTerrain->moveTo(vec2(0,32));
	btnTerrain->setFunction([=](){
		this->mode = ME_MODE_TERRAIN;
		this->submode = ME_SUBMODE_DEFAULT;
        printf("map editor: terrain mode\n");
	});
	mainWindow->addChild(btnTerrain);
	GUIlabel *lbl2 = new GUIlabel();
	lbl2->setText("place terrain");
	lbl2->moveTo(vec2(32,32));
	lbl2->sizeToContents();
	lbl2->setSize(vec2(lbl2->area.size.x,32));
	mainWindow->addChild(lbl2);
    GUIbutton *btnSelect = new GUIbutton();
    btnSelect->setImage(getTexture("gui/cursor"));
    btnSelect->setText("");
    btnSelect->setSize(vec2(32,32));
    btnSelect->moveTo(vec2(0,32*2));
    btnSelect->setFunction([=](){
        this->mode = ME_MODE_SELECT;
        this->submode = ME_SUBMODE_DEFAULT;
        printf("map editor: select mode\n");
    });
    mainWindow->addChild(btnSelect);
    GUIlabel *lbl3 = new GUIlabel();
    lbl3->setText("select");
    lbl3->moveTo(vec2(32,32*2));
    lbl3->sizeToContents();
    lbl3->setSize(vec2(lbl3->area.size.x,32));
    mainWindow->addChild(lbl3);
    GUIbutton *btnFlag = new GUIbutton();
    btnFlag->setImage(getTexture("gui/iconflag"));
    btnFlag->setText("");
    btnFlag->setSize(vec2(32,32));
    btnFlag->moveTo(vec2(0,32*3));
    btnFlag->setFunction([=](){
        this->mode = ME_MODE_PLACE;
        this->submode = ME_SUBMODE_PLACE_FLAG;
        printf("map editor: place flag mode\n");
    });
    mainWindow->addChild(btnFlag);
    GUIlabel *lbl4 = new GUIlabel();
    lbl4->setText("place flag");
    lbl4->moveTo(vec2(32,32*3));
    lbl4->sizeToContents();
    lbl4->setSize(vec2(lbl4->area.size.x,32));
    mainWindow->addChild(lbl4);
    GUIbutton *btnFoot = new GUIbutton();
    btnFoot->setImage(getTexture("gui/footprint_both"));
    btnFoot->setText("");
    btnFoot->setSize(vec2(32,32));
    btnFoot->moveTo(vec2(0,32*4));
    btnFoot->setFunction([=](){
        this->mode = ME_MODE_PATHFIND;
        this->submode = ME_SUBMODE_DEFAULT;
    });
    mainWindow->addChild(btnFoot);
    GUIlabel *lbl5 = new GUIlabel();
    lbl5->setText("pathfinding");
    lbl5->moveTo(vec2(32,32*4));
    lbl5->sizeToContents();
    lbl5->setSize(vec2(lbl5->area.size.x,32));
    mainWindow->addChild(lbl5);
	GUIbutton *btnProbe = new GUIbutton();
	btnProbe->setImage(getTexture("gui/dial"));
	btnProbe->setText("");
	btnProbe->setSize(vec2(32,32));
	btnProbe->moveTo(vec2(0,32*5));
	btnProbe->setFunction([=](){
		printf("probe mode\n");
		this->mode = ME_MODE_PROBE;
		this->submode = ME_SUBMODE_DEFAULT;
	});
	mainWindow->addChild(btnProbe);
	GUIlabel *lbl6 = new GUIlabel();
    lbl6->setText("surface probe");
    lbl6->moveTo(vec2(32,32*5));
    lbl6->sizeToContents();
    lbl6->setSize(vec2(lbl6->area.size.x,32));
    mainWindow->addChild(lbl6);
	GUIbutton *btnPlane = new GUIbutton();
	btnPlane->setImage(getTexture("gui/iconface"));
	btnPlane->setText("");
	btnPlane->setSize(vec2(32,32));
	btnPlane->moveTo(vec2(0,32*6));
	btnPlane->setFunction([=](){
		printf("plane mode\n");
		this->mode = ME_MODE_PLANE;
		this->submode = ME_SUBMODE_DEFAULT;
		this->points.clear();
	});
	mainWindow->addChild(btnPlane);
	GUIlabel *lbl7 = new GUIlabel();
	lbl7->setText("place a plane\n");
	lbl7->moveTo(vec2(32,32*6));
	lbl7->sizeToContents();
	lbl7->setSize(vec2(lbl7->area.size.x,32));
	mainWindow->addChild(lbl7);
	GUIbutton *btnTexture = new GUIbutton();
	btnTexture->setImage(getTexture("materials/building3"));
	btnTexture->setText("");
	btnTexture->setSize(vec2(32,32));
	btnTexture->moveTo(vec2(0,32*7));
	btnTexture->setFunction([=](){
		printf("activated texture browser");
	});
	mainWindow->addChild(btnTexture);
	GUI->addChild(mainWindow);
	layerMap3D = addNewLayer("map3d");
	renderLayer *lm3d_reset = addNewLayer("map3d.reset",true,true);
	layerMap3D->resetLayer = lm3d_reset;
	setLayer(lm3d_reset);
	setTexturing(false);
	setLighting(false);
	setPosition(vec3(0,0,0));
	drawLine(vec3(0,0,0),vec3(0,0,2));
	setAlpha(255);
    setLayer(layer2D);
    layerMap2D = addNewLayer("map2d");
    renderLayer *lm2d_reset = addNewLayer("map2d.reset",true,true);
    layerMap2D->resetLayer = lm2d_reset;
    setLayer(lm2d_reset);
    setTexturing(true);
    setLighting(false);
    setPosition(vec3(0,0,0));
    setAlpha(255);
    setColor(vec3(255,255,255));
	inputChannel.addListener(this);
	globalChannel.addListener(this);
}
float roundf(float x,float prec){return roundf(x/prec)*prec;}
float floorf(float x,float prec){return floorf(x/prec)*prec;}
float ceilf(float x,float prec){return ceilf(x/prec)*prec;}
void getMinMaxCoords(vec3 p1, vec3 p2,
	vec3 *a = 0, vec3 *b = 0, vec3 *c = 0, vec3 *d = 0,
	vec3 *e = 0, vec3 *f = 0, vec3 *g = 0, vec3 *h = 0)
{
	if(a){*a = vec3(min(p1.x,p2.x),min(p1.y,p2.y),min(p1.z,p2.z));}
	if(b){*b = vec3(max(p1.x,p2.x),min(p1.y,p2.y),min(p1.z,p2.z));}
	if(c){*c = vec3(max(p1.x,p2.x),max(p1.y,p2.y),min(p1.z,p2.z));}
	if(d){*d = vec3(min(p1.x,p2.x),max(p1.y,p2.y),min(p1.z,p2.z));}
	if(e){*e = vec3(min(p1.x,p2.x),min(p1.y,p2.y),max(p1.z,p2.z));}
	if(f){*f = vec3(max(p1.x,p2.x),min(p1.y,p2.y),max(p1.z,p2.z));}
	if(g){*g = vec3(max(p1.x,p2.x),max(p1.y,p2.y),max(p1.z,p2.z));}
	if(h){*h = vec3(min(p1.x,p2.x),max(p1.y,p2.y),max(p1.z,p2.z));}
}
bool mouseray(vec3 *pos, float z){
	auto& camera = Gr->gs_camera->g_camera;
	vec3 dir = camera.getMouseDir();
	vec3 plane[3];
	plane[0] = vec3(0,0,z);
	plane[1] = vec3(1,0,z);
	plane[2] = vec3(0,1,z);
	bool has_hit = ray_plane_intersection(camera.pos,dir,plane[0],plane[1],plane[2],pos);
	return has_hit;
}
typedef map_editor_kind::selectDataKind::dirKind DIR;
string toString(DIR d){
    switch(d){
        case(DIR::DIR_NONE): return "none";
        case(DIR::DIR_UP): return "up";
        case(DIR::DIR_DN): return "down";
        case(DIR::DIR_LEFT): return "left";
        case(DIR::DIR_RIGHT): return "right";
        case(DIR::DIR_FWD): return "forward";
        case(DIR::DIR_BACK): return "back";
    }
    return "error";
}
vec3 toVec3(DIR d){
    switch(d){
        case(DIR::DIR_NONE): return vec3(0,0,0);
        case(DIR::DIR_UP): return vec3(0,0,1);
        case(DIR::DIR_DN): return vec3(0,0,-1);
        case(DIR::DIR_LEFT): return vec3(1,0,0);
        case(DIR::DIR_RIGHT): return vec3(-1,0,0);
        case(DIR::DIR_FWD): return vec3(0,1,0);
        case(DIR::DIR_BACK): return vec3(0,-1,0);
    }
    return vec3(0,0,0);
}
void map_editor_kind::onEvent(eventKind event){
	auto& camera = Gr->gs_camera->g_camera;
	//auto& templayer = Gt->gs_map_editor->g_templayer;
	//auto& layerDebug = Gr->gs_paint->g_layerDebug;
	switch(mode){
		case ME_MODE_BLOCK_PLACEMENT:
		case ME_MODE_TERRAIN:
			if(event.type == EVENT_MOUSE_MOVE){
				vec3 p1;
				//bool has_ent = false;
			}
			if(event.type == EVENT_MOUSE_BUTTON_DOWN){
				if(event.mousebutton.button == MOUSE_LEFT){
					if(!is_selecting){
						sel_pos1 = mouseover_square;
						is_selecting = true;
						selectmode = 0;
						printf("LMB down\n");
					}
				}
				if(event.mousebutton.button == MOUSE_RIGHT){
					if(!has_mouseover){
						mode = ME_MODE_DEFAULT;	
					}else if(!is_selecting){
						sel_pos1 = mouseover_square;
						is_selecting = true;
						selectmode = 1;
						printf("RMB down\n");
					}
				}
			}
			if(event.type == EVENT_MOUSE_BUTTON_UP){
				if(is_selecting && event.mousebutton.button == MOUSE_LEFT){
					sel_pos2 = mouseover_square;
					is_selecting = false;
					printf("MB up\n");
					float z1 = sel_pos1.z;
					vec3 A,B,C,D;
					getMinMaxCoords(sel_pos1,sel_pos2,&A,&B,&C,&D);
					A += vec3(0,0,z1);
					B += vec3(gridsize,0,z1);
					C += vec3(gridsize,gridsize,z1);
					D += vec3(0,gridsize,z1);
					vec3 startCorner = A;
					vec3 endCorner = C;
					startCorner.z = z1;
					endCorner.z = z1+2.f;
					if(mode == ME_MODE_BLOCK_PLACEMENT){
					}
				}else if(is_selecting && event.mousebutton.button == MOUSE_RIGHT){
					is_selecting = false;	
				}
			}
			if(event.type == EVENT_FRAME){
				clearLayer(layerMap3D);
				resetLayer(layerMap3D);
				if(has_mouseover){
					float x = mouseover_square.x;
					float y = mouseover_square.y;
					float z1 = mouseover_square.z;
					vec3 A = vec3(x,y,z1);
					vec3 B = vec3(x+gridsize,y,z1);
					vec3 C = vec3(x+gridsize,y+gridsize,z1);
					vec3 D = vec3(x,y+gridsize,z1);
					setLayer(layerMap3D);
					setLineWidth(2);
					setColor(vec3(0,255,0));
					drawLine(A,B);
					drawLine(B,C);
					drawLine(C,D);
					drawLine(D,A);
					drawPoint(mouseover_pos);
					setLineWidth(1);
					setColor(vec3(0,255,255));
					drawLine(mouseover_pos,A);
				}
				if(is_selecting){
					vec3 A,B,C,D;
					getMinMaxCoords(sel_pos1,mouseover_square,&A,&B,&C,&D);
					A += vec3(0,0,0);
					B += vec3(gridsize,0,0);
					C += vec3(gridsize,gridsize,0);
					D += vec3(0,gridsize,0);
					setLineWidth(3);
					setColor(vec3(0,128,0));
					drawLine(A,B);
					drawLine(B,C);
					drawLine(C,D);
					drawLine(D,A);
					drawPoint(A,vec3(255,0,0));
					drawPoint(B,vec3(0,255,0));
					drawPoint(C,vec3(0,0,255));
					drawPoint(D,vec3(255,255,0));
				}
			}
		break;
        case ME_MODE_SELECT:
            if(event.type == EVENT_MOUSE_MOVE){
                vec3 pos;
            }
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
                switch(submode){
                    case ME_SUBMODE_DEFAULT:
                    break;
                    case ME_SUBMODE_MOVE:
                    {
                        float seldist = 18.f;
                        float distU, distD, distL, distR, distF, distB;
                        vec3 mousePos = vec3(getMousePos().x, getMousePos().y, 0);
                             if((distU = length(mousePos-setZ(sdata.ptUp2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_UP;
                                sdata.initialOffset = dot((mousePos-sdata.ptUp2D),sdata.dirAwayUp);
                             }
                        else if((distD = length(mousePos-setZ(sdata.ptDn2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_DN;
                                sdata.initialOffset = dot((mousePos-sdata.ptDn2D),sdata.dirAwayDn);
                             }
                        else if((distL = length(mousePos-setZ(sdata.ptLeft2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_LEFT;
                                sdata.initialOffset = dot((mousePos-sdata.ptLeft2D),sdata.dirAwayLeft);
                             }
                        else if((distR = length(mousePos-setZ(sdata.ptRight2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_RIGHT;
                                sdata.initialOffset = dot((mousePos-sdata.ptRight2D),sdata.dirAwayRight);
                             }
                        else if((distF = length(mousePos-setZ(sdata.ptFwd2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_FWD;
                                sdata.initialOffset = dot((mousePos-sdata.ptFwd2D),sdata.dirAwayFwd);
                             }
                        else if((distB = length(mousePos-setZ(sdata.ptBack2D,0))) < seldist){
                                sdata.dragging = true; sdata.dir = DIR::DIR_BACK;
                                sdata.initialOffset = dot((mousePos-sdata.ptBack2D),sdata.dirAwayBack);
                             }
                        else {submode = ME_SUBMODE_DEFAULT; sdata.dragging = false; sdata.dir = DIR::DIR_NONE;}
                    }
                    break;
                }
            }
            if(event.type == EVENT_MOUSE_BUTTON_UP){
                switch(submode){
                    case ME_SUBMODE_MOVE:
                        sdata.dragging = false;
                        sdata.dir = selectDataKind::dirKind::DIR_NONE;
                    break;
                }
            }
            if(event.type == EVENT_FRAME){
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);
                texture *t = getTexture("gui/arrow3");
                vec3 pt = vec3(0,0,0);
                vec3 pt2D = camera.worldToScreen(pt); pt2D.z = 0;
                vec2 iconSize = t->size();
                rect R = rect(-iconSize/2.f, iconSize/2.f);
                vec2 mpt = getMousePos();
                vec2 diff = mpt-vec2(pt2D);
                float angle = atan2(diff.y,diff.x);
                setLayer(layerMap2D);
                setTransparency(true);
                setTexture(t);
                setPosition(pt2D);
                setScale(vec3(1,1,1)*2.f);
                setRotation(vec3(0,0,1)*(angle+90*d2r));
                drawRect(R);
		   }
        break;
        case ME_MODE_PLACE:{
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
            }
            if(event.type == EVENT_FRAME){
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);
            }
        }
        break;
        case ME_MODE_PATHFIND:{
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
            }
            if(event.type == EVENT_FRAME){
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);
            }
        }
        break;
		case ME_MODE_PROBE:{            
			if(event.type == EVENT_MOUSE_BUTTON_DOWN){
				if(event.mousebutton.button == MOUSE_RIGHT){
					switch(submode){
						case ME_SUBMODE_DEFAULT: submode = ME_SUBMODE_POINT; printf("submode: point\n"); break;
						case ME_SUBMODE_POINT:   submode = ME_SUBMODE_CUBE; printf("submode: cube\n"); break;
						default: submode = ME_SUBMODE_DEFAULT; printf("submode: default\n"); break;
					}
				}
            }
            if(event.type == EVENT_FRAME){
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);
			}
		}
		break;
		case ME_MODE_PLANE:{
			if(event.type == EVENT_MOUSE_BUTTON_DOWN){
				if(event.mousebutton.button == MOUSE_LEFT){
					if(points.size() < 3){points.push_back(mouseover_pos);}
					else{
						points.clear();
					}
				}
				if(event.mousebutton.button == MOUSE_RIGHT){
					if(points.size()){
						points.clear();
					}else{
					}
				}
			}
			if(event.type == EVENT_FRAME){
				clearLayer(layerMap3D);
				resetLayer(layerMap3D);
				clearLayer(layerMap2D);
				resetLayer(layerMap2D);
				if(points.size()){
					setLayer(layerMap3D);
					setPointSize(4.f);
					auto drawPt = [&](vec3 pt, vec3 prevPt){
						drawPoint(pt);
						drawLine(prevPt,pt);
					};
					if(points.size() < 3){drawPt(points[0],points[0]);}
					else{				  drawPt(points[0],points[2]);}
					if(points.size() > 1){drawPt(points[1],points[0]);}
					if(points.size() > 2){drawPt(points[2],points[1]);}
				}
			}
		}break;
        default:
		break;
	}
}
