#include "map_editor.h"
#include "geometry.h"
#include "simplemath.h"
#include "node_graph.h"

//map_editor_kind *g_mapeditor;
//extern renderLayer *g_ecs_render_layer;

void openMapEditor(){
	auto& mapeditor = G->gs_map_editor->g_mapeditor;

	if(mapeditor){delete mapeditor;}
	mapeditor = new map_editor_kind();
}

map_editor_kind::map_editor_kind(){
	//--------- GUI -----------------------
	mainWindow = new GUIwindow();
	mainWindow->setSize(vec2(150,500));

	//---[ ] place blocks---
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
	//---[ ] place terrain---
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

	//---[ ] select ----
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

	//---[ ] place flag ---------
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

	//---[ ] pathfind mode ------
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

	//---[ ] surface probe ------ 
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
	//---[ ] place a plane --------
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
	//---[ ] texture browser ----------
	GUIbutton *btnTexture = new GUIbutton();
	btnTexture->setImage(getTexture("materials/building3"));
	btnTexture->setText("");
	btnTexture->setSize(vec2(32,32));
	btnTexture->moveTo(vec2(0,32*7));
	btnTexture->setFunction([=](){
		printf("activated texture browser");
	});
	mainWindow->addChild(btnTexture);
	//---------

	g_GUI->addChild(mainWindow);
	//---------- 3D render -----------------
	setLayer(g_ecs_render_layer);
	layerMap3D = addNewLayer("map3d");
	renderLayer *lm3d_reset = addNewLayer("map3d.reset",true,true);
	layerMap3D->resetLayer = lm3d_reset;

	setLayer(lm3d_reset);
	setTexturing(false);
	setLighting(false);
	setPosition(vec3(0,0,0));
	drawLine(vec3(0,0,0),vec3(0,0,2));
	setAlpha(255);
    //----------- 2D render -----------------
    setLayer(g_layer2D);
    layerMap2D = addNewLayer("map2d");
    renderLayer *lm2d_reset = addNewLayer("map2d.reset",true,true);
    layerMap2D->resetLayer = lm2d_reset;

    setLayer(lm2d_reset);
    setTexturing(true);
    setLighting(false);
    setPosition(vec3(0,0,0));
    setAlpha(255);
    setColor(vec3(255,255,255));
	//--------- events ----------------------
	g_inputChannel->addListener(this);
	g_globalChannel->addListener(this);
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
	vec3 dir = g_camera.getMouseDir();
	vec3 plane[3];
	plane[0] = vec3(0,0,z);
	plane[1] = vec3(1,0,z);
	plane[2] = vec3(0,1,z);
	bool has_hit = ray_plane_intersection(g_camera.pos,dir,plane[0],plane[1],plane[2],pos);
	return has_hit;
}

bool mouseray(vec3 *pos, collisioninfo **col){
	*col = g_camera.eyetrace();
	if(*col){
		*pos = (*col)->c_to_c.pos;
		return true;
	}
	return false;
}

//renderLayer *g_templayer = 0;

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

//vector<entity*> g_planes;

void map_editor_kind::onEvent(eventKind event){
	auto& camera = G->gs_camera->g_camera;
	auto& planes = G->gs_map_editor->g_planes;
	auto& templayer = G->gs_map_editor->g_templayer;
	auto& layerDebug = G->gs_paint->g_layerDebug;

	switch(mode){
		case ME_MODE_BLOCK_PLACEMENT:
		case ME_MODE_TERRAIN:
			if(event.type == EVENT_MOUSE_MOVE){
				vec3 p1;
				collisioninfo *col = 0;

				bool has_ent = false;
				bool has_hit = mouseray(&p1, &col);
				if(!has_hit){has_hit = mouseray(&p1,1.f); mouseover_ent = 0;}
				else
				{
					//also later:
					//gotta make an entity ID system for reasons of:
					//1) networking (can't send reference to another computer)
					//2) saving/loading (can't save references)
					//3) memory management (no zombie entities / dangling pointers)
											//also we got a dangling pointer on body1 (prolly to ray)

					// entity id (signed int32_t)
					//
					// logical bit meaning:
					// MSB	7 bits	24 bits
					// 0	1:7		8:31
					// sign	id-type	id
					//		1-128	1-16777215
					// (16 million objects, 128 types)
					//DIR
					// forbidden IDs:
					// id =  0	(0)
					// idt = 0	(0x0..0xFFFFFF)
					// idi = 0  (0x??00 0000)
					// id < 0   (> 0x8000 0000)

					has_ent = true;
					assert(col != 0);
					
					mouseover_ent = col->body2->E;
				}

				if(has_hit){

					if(templayer == 0){
						templayer = new renderLayer("black_dots_layer");
						addLayer(layerDebug, templayer);
					}
					setLayer(templayer);
					setColor(vec3(64,0,0));


					static int once = 1;
					if(once){

						setColor(vec3(64,64,64));
						if(col){
							collisionbodyTerrain *body = dynamic_cast<collisionbodyTerrain*>(col->body2);
							if(body){
								vec3 pos = body->pos;
								once = 0;
								setColor(vec3(0,64,0));
								for(auto I = body->em->tris.begin(); I != body->em->tris.end(); I++){

									e_triangle &tri = **I;
									e_vertex &vertA = *(tri.definition.verts[0]);
									e_vertex &vertB = *(tri.definition.verts[1]);
									e_vertex &vertC = *(tri.definition.verts[2]);

									vec3 &A = vertA.pos;
									vec3 &B = vertB.pos;
									vec3 &C = vertC.pos;
								}

							}
						}
					}

					has_mouseover = true;
					mouseover_pos = p1;
					float x = p1.x;
					float y = p1.y;
					float z = p1.z;
					z = floorf(z+gridsize*0.5f,gridsize);

					x = floorf(x,gridsize);
					y = floorf(y,gridsize);

					mouseover_square = vec3(x,y,z);
				}else{
					has_mouseover = false;
				}
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
						mode = ME_MODE_DEFAULT;	//right click away = cancel
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
					//create block action
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
						wall(startCorner, endCorner);
					}
					if(mode == ME_MODE_TERRAIN){makeSheet(startCorner, endCorner);}
				}else if(is_selecting && event.mousebutton.button == MOUSE_RIGHT){
					//carve block action
					is_selecting = false;
					if(!mouseover_ent){return;}
					//todo: calculate un-carved parts
					//right now just delete the block
					//delete mouseover_ent;
					removeEntity(mouseover_ent);
					mouseover_ent = 0;
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
                collisioninfo *col = 0;
                vec3 pos;
                if(mouseray(&pos,&col)){
                    entity *E = col->body2->E;
                    if(E != mouseover_ent){/* un-select entity */}
                    mouseover_ent = E;
                }
            }
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
                switch(submode){
                    case ME_SUBMODE_DEFAULT:
                        if(selected_ent != mouseover_ent){
                            selected_ent = mouseover_ent;
                        }else{
                            submode = ME_SUBMODE_MOVE;
                        }
                    break;
                    case ME_SUBMODE_MOVE:
                        //first, find which pt we're touching if any
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

                //generic test stuff
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
                //end test stuff
                //real stuff
                if(mouseover_ent){
                    if(mouseover_ent->body){
                        collisionbody &body = *mouseover_ent->body;
                        vec3 pos = body.pos;
                        AABB aabb = body.getAABB().moveBy(pos);

                        setLayer(layerMap3D);
                        setDepthTest(true);
                        setColor(vec3(0,128,0));
                        setLineWidth(2.f);
                        drawBoxWireframe(aabb);
                    }
                }
                if(selected_ent){
                    if(selected_ent->body){
                        collisionbody &body = *selected_ent->body;
                        vec3 pos = body.pos;
                        AABB aabb = body.getAABB().moveBy(pos);

                        setLayer(layerMap3D);
                        setDepthTest(false);
                        setColor(vec3(0,255,0));
                        setLineWidth(2.f);
                        drawBoxWireframe(aabb);

                        switch(submode){
                            case(ME_SUBMODE_MOVE):
                                //draw some arrows (2d)
                                float sepr = 1.f;
                                vec3 size = aabb.end-aabb.start;
                                sdata.ptUp = pos+vec3(0,0,sepr/2.f)*size.z;
                                sdata.ptDn = pos+vec3(0,0,-sepr/2.f)*size.z;
                                sdata.ptLeft = pos+vec3(sepr/2.f,0,0)*size.x;
                                sdata.ptRight = pos+vec3(-sepr/2.f,0,0)*size.x;
                                sdata.ptFwd = pos+vec3(0,sepr/2.f,0)*size.y;
                                sdata.ptBack = pos+vec3(0,-sepr/2.f,0)*size.y;

                                sdata.ptUp2D = camera.worldToScreen(sdata.ptUp);
                                sdata.ptDn2D = camera.worldToScreen(sdata.ptDn);
                                sdata.ptLeft2D = camera.worldToScreen(sdata.ptLeft);
                                sdata.ptRight2D = camera.worldToScreen(sdata.ptRight);
                                sdata.ptFwd2D = camera.worldToScreen(sdata.ptFwd);
                                sdata.ptBack2D = camera.worldToScreen(sdata.ptBack);

                                sdata.dirAwayUp = camera.worldToScreen(sdata.ptUp+vec3(0,0,0.01))-sdata.ptUp2D;
                                sdata.dirAwayDn = camera.worldToScreen(sdata.ptDn+vec3(0,0,-0.01))-sdata.ptDn2D;
                                sdata.dirAwayLeft = camera.worldToScreen(sdata.ptLeft+vec3(0.01,0,0))-sdata.ptLeft2D;
                                sdata.dirAwayRight = camera.worldToScreen(sdata.ptRight+vec3(-0.01,0,0))-sdata.ptRight2D;
                                sdata.dirAwayFwd = camera.worldToScreen(sdata.ptFwd+vec3(0,0.01,0))-sdata.ptFwd2D;
                                sdata.dirAwayBack = camera.worldToScreen(sdata.ptBack+vec3(0,-0.01,0))-sdata.ptBack2D;

                                sdata.rotUp = atan2(sdata.dirAwayUp.y,sdata.dirAwayUp.x);
                                sdata.rotDn = atan2(sdata.dirAwayDn.y,sdata.dirAwayDn.x);
                                sdata.rotLeft = atan2(sdata.dirAwayLeft.y,sdata.dirAwayLeft.x);
                                sdata.rotRight = atan2(sdata.dirAwayRight.y,sdata.dirAwayRight.x);
                                sdata.rotFwd = atan2(sdata.dirAwayFwd.y,sdata.dirAwayFwd.x);
                                sdata.rotBack = atan2(sdata.dirAwayBack.y,sdata.dirAwayBack.x);

                                //arrow mouseover detection
                                if(!sdata.dragging){
                                    float seldist = 18.f;
                                    float distU, distD, distL, distR, distF, distB;
                                    vec3 mousePos = vec3(getMousePos().x, getMousePos().y, 0);
                                         if((distU = length(mousePos-setZ(sdata.ptUp2D,0))) < seldist){sdata.dir = DIR::DIR_UP;}
                                    else if((distD = length(mousePos-setZ(sdata.ptDn2D,0))) < seldist){sdata.dir = DIR::DIR_DN;}
                                    else if((distL = length(mousePos-setZ(sdata.ptLeft2D,0))) < seldist){sdata.dir = DIR::DIR_LEFT;}
                                    else if((distR = length(mousePos-setZ(sdata.ptRight2D,0))) < seldist){sdata.dir = DIR::DIR_RIGHT;}
                                    else if((distF = length(mousePos-setZ(sdata.ptFwd2D,0))) < seldist){sdata.dir = DIR::DIR_FWD;}
                                    else if((distB = length(mousePos-setZ(sdata.ptBack2D,0))) < seldist){sdata.dir = DIR::DIR_BACK;}
                                    else {sdata.dir = DIR::DIR_NONE;}
                                }
                                //oving this here to ensure synchronisation
                                //drag distance calculation
                                if(selected_ent && selected_ent->body){
                                    if(sdata.dragging){
                                        collisionbody &body = *selected_ent->body;
                                        vec3 mousePos = vec3(getMousePos().x, getMousePos().y, 0);
                                        //float offset;
                                        vec3 dv3d;
                                        float dist3D;
                                        switch(sdata.dir){
                                            case(DIR::DIR_UP):{
                                                float axisOffset = dot((mousePos-sdata.ptUp2D),sdata.dirAwayUp);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                                //we don't just take mousepos because we need to
                                                                //re-align the axes in 2D and then in 3D too
                                                vec3 finalPt2D = sdata.ptUp2D+normalizeSafe(sdata.dirAwayUp)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptUp),toVec3(sdata.dir));
                                            }break;
                                            case(DIR::DIR_DN):{
                                                float axisOffset = dot((mousePos-sdata.ptDn2D),sdata.dirAwayDn);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                vec3 finalPt2D = sdata.ptDn2D+normalizeSafe(sdata.dirAwayDn)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptDn),toVec3(sdata.dir));
                                            }break;
                                            case(DIR::DIR_LEFT):{
                                                float axisOffset = dot((mousePos-sdata.ptLeft2D),sdata.dirAwayLeft);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                vec3 finalPt2D = sdata.ptLeft2D+normalizeSafe(sdata.dirAwayLeft)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptLeft),toVec3(sdata.dir));
                                            }break;
                                            case(DIR::DIR_RIGHT):{
                                                float axisOffset = dot((mousePos-sdata.ptRight2D),sdata.dirAwayRight);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                vec3 finalPt2D = sdata.ptRight2D+normalizeSafe(sdata.dirAwayRight)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptRight),toVec3(sdata.dir));
                                            }break;
                                            case(DIR::DIR_FWD):{
                                                float axisOffset = dot((mousePos-sdata.ptFwd2D),sdata.dirAwayFwd);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                vec3 finalPt2D = sdata.ptFwd2D+normalizeSafe(sdata.dirAwayFwd)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptFwd),toVec3(sdata.dir));
                                            }break;
                                            case(DIR::DIR_BACK):{
                                                float axisOffset = dot((mousePos-sdata.ptBack2D),sdata.dirAwayBack);
                                                float pixelsMoved = axisOffset-sdata.initialOffset;
                                                vec3 finalPt2D = sdata.ptBack2D+normalizeSafe(sdata.dirAwayBack)*pixelsMoved;
                                                vec3 finalPt3D = camera.screenToWorld(finalPt2D);
                                                dist3D = dot((finalPt3D-sdata.ptBack),toVec3(sdata.dir));
                                            }break;
                                        }
                                        float smoothCoeff = 0.9f;
                                        dv3d = toVec3(sdata.dir)*dist3D*smoothCoeff;
                                        body.pos += dv3d;
                                    }
                                }

                                setLayer(layerMap2D);
                                texture *t = getTexture("gui/arrow4");
                                setTexture(t);
                                setTransparency(true);
                                setDepthTest(false);
                                vec2 iconSize = t->size();
                                rect R = rect(-iconSize/2.f,iconSize/2.f);
                                setScale(vec3(1,1,1));

                                setRotation(vec3(0,0,1)*(sdata.rotUp+90.f*d2r));
                                setPosition(vec3(sdata.ptUp2D.x,sdata.ptUp2D.y,0));
                                sdata.dir == DIR::DIR_UP? setColor(vec3(255,255,255)) : setColor(vec3(128,128,255));
                                drawRect(R);

                                setRotation(vec3(0,0,1)*(sdata.rotDn+90.f*d2r));
                                setPosition(vec3(sdata.ptDn2D.x,sdata.ptDn2D.y,0));
                                sdata.dir == DIR::DIR_DN? setColor(vec3(255,255,255)) : setColor(vec3(128,128,255));
                                drawRect(R);

                                setRotation(vec3(0,0,1)*(sdata.rotLeft+90.f*d2r));
                                setPosition(vec3(sdata.ptLeft2D.x,sdata.ptLeft2D.y,0));
                                sdata.dir == DIR::DIR_LEFT? setColor(vec3(255,255,255)) : setColor(vec3(255,128,128));
                                drawRect(R);

                                setRotation(vec3(0,0,1)*(sdata.rotRight+90.f*d2r));
                                setPosition(vec3(sdata.ptRight2D.x,sdata.ptRight2D.y,0));
                                sdata.dir == DIR::DIR_RIGHT? setColor(vec3(255,255,255)) : setColor(vec3(255,128,128));
                                drawRect(R);

                                setRotation(vec3(0,0,1)*(sdata.rotFwd+90.f*d2r));
                                setPosition(vec3(sdata.ptFwd2D.x,sdata.ptFwd2D.y,0));
                                sdata.dir == DIR::DIR_FWD? setColor(vec3(255,255,255)) : setColor(vec3(128,255,128));
                                drawRect(R);

                                setRotation(vec3(0,0,1)*(sdata.rotBack+90.f*d2r));
                                setPosition(vec3(sdata.ptBack2D.x,sdata.ptBack2D.y,0));
                                sdata.dir == DIR::DIR_BACK? setColor(vec3(255,255,255)) : setColor(vec3(128,255,128));
                                drawRect(R);

                            break;
                        }

                    }
                }
            }
        break;
        case ME_MODE_PLACE:{
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
                spawnFlag(mouseover_pos);
            }
            if(event.type == EVENT_FRAME){
                collisioninfo *col;
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);

                if(mouseray(&mouseover_pos, &col)){
                    setLayer(layerMap3D);
                    setColor(vec3(0,255,0));
                    float size = 0.1f;
                    vec3 v1 = vec3(1,1,1);
                    drawBoxWireframe(AABB(-v1*size/2.f,v1*size/2.f).moveBy(mouseover_pos));
                }
            }
        }
        break;
        case ME_MODE_PATHFIND:{
            if(event.type == EVENT_MOUSE_BUTTON_DOWN){
                pathfinding_test(mouseover_pos);
            }
            if(event.type == EVENT_FRAME){
                collisioninfo *col;
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);

                if(mouseray(&mouseover_pos, &col)){
                    setLayer(layerMap3D);
                    setColor(vec3(0,255,0));
                    float size = 0.1f;
                    vec3 v1 = vec3(1,1,1);
                    drawBoxWireframe(AABB(-v1*size/2.f,v1*size/2.f).moveBy(mouseover_pos));
                }
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
                collisioninfo *col;
                clearLayer(layerMap3D);
                resetLayer(layerMap3D);
                clearLayer(layerMap2D);
                resetLayer(layerMap2D);

                if(mouseray(&mouseover_pos, &col)){
					vec3 dir = camera.getMouseDir();
					vec3 probe_pos = mouseover_pos+dir*0.05f;
                    setLayer(layerMap3D);
                    setColor(vec3(255,255,0));
					collisionbody *cb = 0;
					collisioninfo *col2;
					switch(submode){
						case ME_SUBMODE_POINT:{
							if(col->body2){
								cb = new collisionbodyPoint();
								cb->bodyname = "probe";
								cb->pos = probe_pos;
								cb->type = BODY_TRIGGER;
								col2 = collisionCheckDispatch(cb, col->body2);
								if(col2){
									delete col; 
									col = col2; 
									setLayer(layerMap3D);
									setColor(vec3(0,255,0)); 
									mouseover_pos = col->c_to_c.pos;}
							}else{
								setLayer(layerMap3D);
								setColor(vec3(255,0,0));
							}
						}
						break;
						case ME_SUBMODE_CUBE:{
							if(col->body2){
								cb = new collisionbodyAABB(AABB(vec3(-0.01f,-0.01f,-0.01f), vec3(0.01f,0.01f,0.01f)));
								cb->bodyname = "probe";
								cb->pos = probe_pos;
								cb->type = BODY_TRIGGER;
								col2 = collisionCheckDispatch(cb, col->body2);
								printf("col2 = %p, pos = %s\n",col2,toCString(cb->pos));
								if(col2){
									delete col; 
									col = col2; 
									printf("body1 = %s\n",col->body1->bodyname.c_str());
									setLayer(layerMap3D); 
									setColor(vec3(0,255,0)); 
									mouseover_pos = col->c_to_c.pos;}
							}else{
								setLayer(layerMap3D);
								setColor(vec3(255,0,0));
							}
						}
						break;
						default: setLayer(layerMap3D); setColor(vec3(0,255,0));
					}
					
                    float size = 0.1f;
                    vec3 v1 = vec3(1,1,1);
                    setLayer(layerMap3D);
                    drawBoxWireframe(AABB(-v1*size/2.f,v1*size/2.f).moveBy(mouseover_pos));
					setColor(vec3(0,255,0));

					vec3 C = col->c_to_c.pos;
					vec3 N = col->c_to_c.normal;
					vec3 P = col->c_to_c.penetration;
					printf("col.pos = %s, col.normal = %s\n",toCString(C), toCString(N));
					drawArrow(C,C+N,vec3(0,255,0));
					drawArrow(C,C+P,vec3(0,0,255));
					if(cb){delete cb;}
					delete col;
				}
            }
		}
		break;
		case ME_MODE_PLANE:{
			if(event.type == EVENT_MOUSE_BUTTON_DOWN){
				if(event.mousebutton.button == MOUSE_LEFT){
					if(points.size() < 3){points.push_back(mouseover_pos);}
					else{
						entity *E = physplane(points[0],points[1],points[2], getTexture("materials/grass1"));
						planes.push_back(E);
						points.clear();
					}
				}
				if(event.mousebutton.button == MOUSE_RIGHT){
					if(points.size()){
						points.clear();
					}else{
						for(unsigned int i = 0; i < planes.size(); i++){
							removeEntity(planes[i]);
						}
						planes.clear();
					}
				}
			}
			if(event.type == EVENT_FRAME){
				collisioninfo *col;
				clearLayer(layerMap3D);
				resetLayer(layerMap3D);
				clearLayer(layerMap2D);
				resetLayer(layerMap2D);

				if(mouseray(&mouseover_pos, &col)){
					setLayer(layerMap3D);
					vec3 v = vec3(1,1,1)*0.1f;
					setColor(vec3(0,255,0));
					drawBoxWireframe(AABB(-v,v).moveBy(mouseover_pos));
				}
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
