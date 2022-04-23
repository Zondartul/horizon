#include "gui_editor.h"


void drawBorders(GUIbase *B){
	//borders
	GUI_border_rects border = B->getBorders();
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
			
			drawBorders(subject);
			
			//indicators
			/*
			GUI_border_type borderState = subject->testBorders(mousePos);
			vec2 v = mousePos + vec2(0,-5);
			rainbowIndicator(v, vector<bool>{
				true,
				(borderState & GB_TOP),		
				(borderState & GB_BOTTOM),		
				(borderState & GB_LEFT),		
				(borderState & GB_RIGHT),		
				(borderState & GB_CORNER_TL),
				(borderState & GB_CORNER_TR),
				(borderState & GB_CORNER_BL),
				(borderState & GB_CORNER_BR)}
			);
			*/
			//drawClock(v+vec2(0,-10));
			//if(stage == GEMT_END){
			//	setColor(COL_DBLUE);
			//	drawLine(toVec3(grabPos), toVec3(mousePos));
			//	drawGrid(subject, gridStep);
			//}
		}
	}
	
	setPointSize(1.0f);
	setColor(vec3(0,0,0));
	drawPoint(toVec3(mousePos));
	//magnifyingGlass(mousePos,vec2(8,-8),32,128);
}
void gui_editor_tool_edit::ldown(){
	gui_editor_tool::ldown();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	
	if(mouseover_element){
		printf("edit ldown 1\n");
		
		if(mouseover_element && (mouseover_element != subject)){
			if(isValidSubject(mouseover_element)){//(mouseover_element != workWindow){
				subject = mouseover_element;
				stage = GEMT_SUBJECT;
			}else{
				subject = 0;
				stage = GEMT_START;
			}
			return; //first click only selects the subject
		}
		
		if(stage == GEMT_SUBJECT){
			vec2 mousePos = getMousePos();
			GUIe_border borderState = subject->testBorders(mousePos);
			
			printf("edit ldown 2\n");			
			oldArea = subject->area;
			
			//vec2 corner = subject->parent->thisToWorld(subject->parent->clientArea).start;
			vec2 gpos = snapToGridToWorld(subject->parent, mousePos, gridStep);//+corner;
			
			grabPos = gpos;//mousePos;
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
	//tool_finished();
	switch(stage){
		case GEMT_END: stage = GEMT_SUBJECT; break;
	}
}


int g_numDDMs = 0;

void gui_editor_tool_edit::rdown(){
	gui_editor_tool::rdown();
	if(mouseover_element && (mouseover_element != subject)){
		if(isValidSubject(mouseover_element)){//(mouseover_element != workWindow){
			subject = mouseover_element;
			stage = GEMT_SUBJECT;
		}else{
			subject = 0;
			stage = GEMT_START;
		}
		return; //first click only selects the subject
	}
}

void gui_editor_tool_edit::rup(){
	gui_editor_tool::rup();
	GUIwindow *workWindow = 0;
	EPCAST(Ed->elWorkWindow, workWindow) else return;
	printf("edit rclick\n");
	
	if(subject && (subject != workWindow)){
		vec2 mousePos = getMousePos();
		
		//if(elDDM){delete (GUIbase*)(elDDM);}
		if(elDDM){elDDM->close(); elDDM = 0;}
		GUIdropdownMenu *ddm = new GUIdropdownMenu();
		elDDM = ddm;
		
		ddm->name = string()+"menu"+toString(g_numDDMs++);
		
		ddm->addItem("properties",[=](){
			if(!subject){return;}
			GUIpropertyTable tab = subject->getPropertyTable();
			//printf("properties:\n");
			//for(auto I = tab.table.begin(); I != tab.table.end(); I++){
			//	auto k = I->first;
			//	auto v = I->second;
			//	printf(" [%s]:[%s]\n",k.c_str(),v.c_str());
			//}
			
			GUItable *table = new GUItable(tab);
			table->moveTo(mousePos);
			table->setSize(vec2(350,200));
			table->invalidate();
			auto subj = subject;
			table->setFunction([subj](string key,string val){
				printf("GUItable on edit called with (%s,%s):\n",key.c_str(),val.c_str());
				if(!subj){printf("no subject\n");}else{printf("has subject(%p)\n",subj);}
				subj->setProperty(key,val);
			});
			g_GUI->addChild(table);
			ddm->close();
		});
		//ddm->addItem("hello");
		//ddm->addItem("test");
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
		  //ddm_edit->addItem("hai");
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
			//delete subject;
			subject->close();
			subject = 0;
			ddm->close();
		  });
		
		//ddm->addItem("test2");
		
		ddm->moveTo(mousePos);
		g_GUI->addChild(ddm);
		/*
		vec2 mousePos = getMousePos();
		
		*/
	}else{Ed->tool_cancel();}
}

void gui_editor_tool_edit::scan(){
	gui_editor_tool::scan();
	//GUIwindow *workWindow = 0;
	//EPCAST(elWorkWindow, workWindow) else return;
	//printf("edit mm\n");
	//mouseover_element = workWindow->getMouseoverElement();
	switch(stage){
		case GEMT_START: printf("stage=start\n");  break;
		case GEMT_END:{
			printf("stage=end\n");
			vec2 mousePos = getMousePos();
			vec2 corner = subject->parent->thisToWorld(subject->parent->clientArea).start;
			vec2 gpos = snapToGridToWorld(subject->parent, mousePos, gridStep);//+corner;
			//printf("\n-----------------\n"
			//"mousePos = %s, gpos = %s\n"
			//"gpos+corn = %s, gr\n"
			//"------------------\n",
			//	toCString(mousePos),
			//	toCString(gpos-corner),
			//	toCString(gpos));
			
			vec2 dpos = gpos-grabPos;
			//printf("grab = [%s]\n",toCString(grabState));
			switch(grabState){
				case GUIb::None: 
					//printf("grab = none\n"); 
					subject->area = oldArea.moveBy(dpos); 
				break;
				case GUIb::Right: 
					//printf("grab = right\n"); 
					subject->area = oldArea.setSize(oldArea.size+vec2(dpos.x,0));
				break;
				case GUIb::Bottom: 
					//printf("grab = bottom\n"); 
					subject->area = oldArea.setSize(oldArea.size + vec2(0,dpos.y));
				break;
				case GUIb::Corner_BR: 
					//printf("grab = corner bottom right\n");
					subject->area = oldArea.setSize(oldArea.size+dpos);
				break;
				case GUIb::Top:
					//printf("grab = top\n");
					subject->area = oldArea.setSize(oldArea.size + vec2(0,-dpos.y)).moveBy(vec2(0,dpos.y));
				break;
				case GUIb::Left:
					//printf("grab = left\n");
					subject->area = oldArea.setSize(oldArea.size + vec2(-dpos.x,0)).moveBy(vec2(dpos.x,0));
				break;
				case GUIb::Corner_TL:
					//printf("grab = corner top left\n");
					subject->area = oldArea.setSize(oldArea.size - dpos).moveBy(dpos);
				break;
				case GUIb::Corner_TR:
					//printf("grab = corner top right\n");
					subject->area = oldArea.setSize(oldArea.size + vec2(dpos.x, -dpos.y)).moveBy(vec2(0,dpos.y));
				break;
				case GUIb::Corner_BL:
					//printf("grab = corner bottom left\n");
					subject->area = oldArea.setSize(oldArea.size + vec2(-dpos.x, dpos.y)).moveBy(vec2(dpos.x,0));
				default: printf("grab = xxx\n"); break;
			}
			rect sar = subject->area;
			rect cl_sar = subject->parent->clientToWorld(sar);
			subject->area = snapToGrid(subject->parent, cl_sar, gridStep);
			//subject->area = rect(snapToGrid(subject->parent, subject->area.start+corner, gridStep),
			//					 snapToGrid(subject->parent, subject->area.end+corner, gridStep));
			subject->invalidate();
		}break;
	}
}
