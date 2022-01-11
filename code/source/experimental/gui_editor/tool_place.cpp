#include "gui_editor.h"

gui_editor_tool_place::gui_editor_tool_place(gui_editor_kind *Ed, submodeKind mode):
	gui_editor_tool(Ed),
	submode(mode){}

void gui_editor_tool_place::scan(){
	gui_editor_tool::scan();
	//GUIwindow *workWindow = 0;
	//EPCAST(Ed->elWorkWindow, workWindow) else return;
	//GUIbase *mEl = workWindow->getMouseoverElement();
	if(stage == GEMT_START){
		//printf("switch subject\n");
		subject = mouseover_element;//mEl;
	}
	if(subject){
		vec2 mpos = getMousePos();
		vec2 gpos = snapToGrid(subject, mpos, gridStep);
		if(/*mEl*/mouseover_element == subject){
			gposCursor = gpos;
		}
		drawVertCursor = true;
	}else{
		drawVertCursor = false;
	}
}

void gui_editor_tool_place::lup(){
	gui_editor_tool::lup();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow,workWindow) else return;
	if(workWindow->mouseover){
		if(stage == GEMT_START){
			printf("mouse up during place_start\n");
			gpos1 = gposCursor;
			drawVert1 = true;
			stage = GEMT_END;
		}
		else if(stage == GEMT_END){
			printf("mouse up during place_end\n");
			GUIbase *mEl = workWindow->getMouseoverElement();
				if(subject){
					printf("has subject\n");
					GUIbase *nEl = 0;
					vec2 gpos2 = gposCursor;
					switch(submode){
						case GEMS_FRAME:	nEl = new GUIframe();	break;
						case GEMS_WINDOW:	nEl = new GUIwindow();	break;
						case GEMS_BUTTON:	nEl = new GUIbutton();	break;
						//case GEMS_CHECKBOX:	subject = new GUIcheckbox();break; checkbox gui widget not implemented?
						case GEMS_IMAGE:{auto img = new GUIimage(); img->setImage(getTexture("other/crate32"));	nEl = img;}	break;
						case GEMS_LABEL:	nEl = new GUIlabel();	break;
						case GEMS_TEXTENTRY:nEl = new GUItextEntry();break;
						case GEMS_SCROLLBAR:nEl = new GUIscrollbar();break;
						case GEMS_SELECTIONGROUP:nEl = new GUIselectionGroup(); break;
						case GEMS_SELECTABLE:nEl = new GUIselectable(); break;
						case GEMS_GRID:		nEl = new GUIgrid();	break;
						//case GEMS_TABS:		subject = new GUItab(); break; tabs also not implemented.
					}
					if(nEl){
						printf("has nEl\n");
						if(gpos2.x < gpos1.x){float x = gpos2.x; gpos2.x = gpos1.x; gpos1.x = x;}
						if(gpos2.y < gpos1.y){float y = gpos2.y; gpos2.y = gpos1.y; gpos1.y = y;}
						vec2 size = gpos2-gpos1;
						nEl->setSize(size);
						nEl->moveTo(gpos1);
						subject->addChild(nEl);
						printf("element added\n");
					}else{
						printf("no nEl\n");
					}
				}else{
					printf("no subject\n");
				}
			//drawVert1 = false;
			//mode = GEM_IDLE;
			Ed->tool_finished();
		}
	}
}

void gui_editor_tool_place::rup(){
	gui_editor_tool::rup();
	Ed->tool_cancel();
}

void gui_editor_tool_place::draw(){
	gui_editor_tool::draw();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	if(subject){
		rect R = subject->thisToWorld(subject->clientArea);
		vec2 gridCorner = R.start;
	
		//if((mode == GEM_PLACE_START) || (mode == GEM_PLACE_END)){
			//snap grid for the client area
			drawGrid(subject, gridStep);
			
			//red outline for the client area
			if(subject && (subject != workWindow)){
				setColor(vec3(255,0,0));
				setLineWidth(3.f);
				drawRectOutline(R);
				setLineWidth(1.f);
			}
			//cursor reticle
			if(drawVertCursor){
				vec2 wpos = gridCorner + gposCursor;	
				setColor(vec3(0,0,68));
				drawImage(getTexture("gui/iconvertexwhite"),rect(wpos-vec2(16,16),wpos+vec2(16,16)));
			}
			//first selected point reticle
			if(drawVert1){
				vec2 wpos = gridCorner + gpos1;
				setColor(vec3(0,0,68));
				drawImage(getTexture("gui/iconvertexwhite"),rect(wpos-vec2(16,16),wpos+vec2(16,16)));
			}
			
			//green two-point rectangle for the new widget 
			if(stage == GEMT_END){
				vec2 wpos1 = gridCorner + gpos1;
				vec2 wpos2 = gridCorner + gposCursor;
				
				setLineWidth(3.f);
				setColor(vec3(0,255,0));
				drawRectOutline(rect(wpos1,wpos2));
				setLineWidth(1.f);
			}
		//}
	}
}

void gui_editor_tool_place::cleanup(){
	drawVert1 = false;
	drawVertCursor = false;
	stage = GEMT_START;
}