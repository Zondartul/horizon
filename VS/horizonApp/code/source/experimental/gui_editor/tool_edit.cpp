#include "experimental/gui_editor.h"
#include "input/mouse.h"
#include "render/paint.h"
#include "util/global_vars_app.h"
#include "util/global_vars_gui.h"

void drawBorders(const GUIbase& B){
	GUI_border_rects border = B.getBorders();
	setColor(vec3(0,64,0));
	drawRectOutline(border.Rtop);
	drawRectOutline(border.Rbottom);
	drawRectOutline(border.Rleft);
	drawRectOutline(border.Rright);
	setColor(vec3(0,128,0));
	drawRectOutline(border.Rctl);
	drawRectOutline(border.Rctr);
	drawRectOutline(border.Rcbl);
	drawRectOutline(border.Rcbr);
}
gui_editor_tool_edit::gui_editor_tool_edit(gui_editor_kind *Ed):gui_editor_tool(Ed){}
bool gui_editor_tool_edit::isValidSubject(GUIbase *el){
	GUIbase *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return false;
	if(el && (el != workWindow)){
		if(el->parent == workWindow){
			if(el->isClient){return true;}
		}else{
			return true;
		}
	}
	return false;
}
void gui_editor_tool_edit::draw(){
	gui_editor_tool::draw();
	vec2 mousePos = getMousePos();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	if(subject){
		if(subject != workWindow){
			drawBorders(*subject);
		}
	}
	setPointSize(1.0f);
	setColor(vec3(0,0,0));
	drawPoint(toVec3(mousePos));
}

void gui_editor_tool_edit::select(GUIbase *node){
	subject = node;
	stage = GEMT_SUBJECT;
}

void gui_editor_tool_edit::deselect(){
	subject = 0;
	stage = GEMT_START;
}

void gui_editor_tool_edit::ldown(){
	gui_editor_tool::ldown();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	if(mouseover_element){
		printf("edit ldown 1\n");
		if(mouseover_element && !(mouseover_element == subject)){
			if(isValidSubject(mouseover_element)){
				select(mouseover_element);
				//subject = mouseover_element;
				//stage = GEMT_SUBJECT;
			}else{
				deselect();
				//subject = 0;
				//stage = GEMT_START;
			}
			return; 
		}
		if(stage == GEMT_SUBJECT){
			vec2 mousePos = getMousePos();
			GUIe_border borderState = subject->testBorders(mousePos);
			printf("edit ldown 2\n");			
			oldArea = subject->area;
			vec2 gpos = snapToGridToWorld(subject->parent, mousePos, (float)gridStep);
			grabPos = gpos;
			grabState = borderState;
			if(grabState != GUIb::None){
				printf(" has borderState\n");
				printf("state = [%s]\n",toCString(grabState));
			}
			stage = GEMT_END;
		}
	}else{
		printf("edit ldown / no subj\n");
	}
}
void gui_editor_tool_edit::lup(){
	gui_editor_tool::lup();
	printf("edit lup\n");
	switch(stage){
		case GEMT_END: stage = GEMT_SUBJECT; break;
	}
}
void gui_editor_tool_edit::rdown(){
	gui_editor_tool::rdown();
	if(mouseover_element && !(mouseover_element == subject)){
		if(isValidSubject(mouseover_element)){
			subject = mouseover_element;
			stage = GEMT_SUBJECT;
		}else{
			subject = 0;
			stage = GEMT_START;
		}
		return; 
	}
}
void gui_editor_tool_edit::rup(){
	auto &numDDMs = Ga->gs_tool_edit->g_numDDMs;
	auto& GUI = Gg->gs_GUI->g_GUI;
	gui_editor_tool::rup();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	printf("edit rclick\n");
	if(subject && (subject != workWindow)){
		vec2 mousePos = getMousePos();
		if(elDDM){elDDM->close(); elDDM = 0;}
		GUIdropdownMenu *ddm = new GUIdropdownMenu();
		elDDM = ddm;
		ddm->name = string()+"menu"+toString(numDDMs++);
		ddm->addItem("properties",[=](){
			if(!subject){return;}
			GUIpropertyTable tab = subject->getPropertyTable();
			GUItable *table = new GUItable(tab);
			table->moveTo(mousePos);
			table->setSize(vec2(350,200));
			table->invalidate();
			table->setFunction([&](string key,string val){
				printf("GUItable on edit called with (%s,%s):\n",key.c_str(),val.c_str());
				if(!subject){printf("no subject\n");}else{printf("has subject(%p)\n",&*subject);}
				subject->setProperty(key,val);
			});
			GUI->addChild(table);
			ddm->close();
		});
		GUIdropdownMenu *ddm_edit = ddm->addSubmenu("edit");
		  ddm_edit->addItem("cut", [=](){
			if(subject && subject->parent){
				if(copyBuffer){copyBuffer->close(); copyBuffer = 0;}
				copyBuffer = subject;
				subject->parent->removeChild(subject);
				subject->parent = 0;
				ddm->close();
				subject = 0;
			}
		  });
		  ddm_edit->addItem("copy",[=](){
			if(subject){this->copyBuffer = subject;}
			ddm->close();
		  });
		  ddm_edit->addItem("paste",[=](){
			if(subject && copyBuffer){
				rect R = subject->worldClientArea();
				vec2 localpos = mousePos-R.start;
				if(!copyBuffer->parent){
					copyBuffer->moveTo(localpos);
					subject->addChild(copyBuffer);
					ddm->close();
				}else{
					auto P = copyBuffer->getCompoundProperty();
					GUIbase *copied = P.instantiate();
					copied->moveTo(localpos);
					subject->addChild(copied);
					ddm->close();
				}
			}
		  });
		  ddm_edit->addItem("delete",[=](){
			subject->close();
			subject = 0;
			ddm->close();
		  });
		ddm->moveTo(mousePos);
		GUI->addChild(ddm);
	}else{Ed->tool_cancel();}
}
void gui_editor_tool_edit::scan(){
	gui_editor_tool::scan();
	switch(stage){
	case GEMT_START: break;//printf("stage=start\n");  break;
		case GEMT_END:{
			//printf("stage=end\n");
			vec2 mousePos = getMousePos();
			vec2 corner = subject->parent->thisToWorld(subject->parent->clientArea).start;
			vec2 gpos = snapToGridToWorld(subject->parent, mousePos, (float)gridStep);
			vec2 dpos = gpos-grabPos;
			switch(grabState){
				case GUIb::None: 
					subject->area = oldArea.moveBy(dpos); 
				break;
				case GUIb::Right: 
					subject->area = oldArea.setSize(oldArea.size+vec2(dpos.x,0));
				break;
				case GUIb::Bottom: 
					subject->area = oldArea.setSize(oldArea.size + vec2(0,dpos.y));
				break;
				case GUIb::Corner_BR: 
					subject->area = oldArea.setSize(oldArea.size+dpos);
				break;
				case GUIb::Top:
					subject->area = oldArea.setSize(oldArea.size + vec2(0,-dpos.y)).moveBy(vec2(0,dpos.y));
				break;
				case GUIb::Left:
					subject->area = oldArea.setSize(oldArea.size + vec2(-dpos.x,0)).moveBy(vec2(dpos.x,0));
				break;
				case GUIb::Corner_TL:
					subject->area = oldArea.setSize(oldArea.size - dpos).moveBy(dpos);
				break;
				case GUIb::Corner_TR:
					subject->area = oldArea.setSize(oldArea.size + vec2(dpos.x, -dpos.y)).moveBy(vec2(0,dpos.y));
				break;
				case GUIb::Corner_BL:
					subject->area = oldArea.setSize(oldArea.size + vec2(-dpos.x, dpos.y)).moveBy(vec2(dpos.x,0));
				default: printf("grab = xxx\n"); break;
			}
			rect sar = subject->area;
			rect cl_sar = subject->parent->clientToWorld(sar);
			subject->area = snapToGrid(subject->parent, cl_sar, (float)gridStep);
			subject->invalidate();
		}break;
	}
}
