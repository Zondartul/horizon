#include "GUI/GUI_internal.h"
#include "render/paint.h"

GUIrenderer_default::
GUIrenderer_default(){name = "default";}
GUIrenderer_default::
~GUIrenderer_default(){}
void GUIrenderer_default::
render(GUIbase *el, string type){
	auto& GUIoptions = Gg->gs_GUI_internal->g_GUIoptions;
	if(!el){return;}
	if(type == ""){type = el->getType();}
	rect R = el->worldArea();
	if(type == "GUbase"){}
	else if(type == "GUIbutton"){
		GUIbutton *elBtn = dynamic_cast<GUIbutton*>(el); if(!elBtn){return;}
		if(GUIoptions.push){pushRenderOptions();}
		vec3 oldColor = elBtn->bgColor;
		if(!el->mouseover){elBtn->pressed = false;}
		if(el->mouseover){setAlpha(196); elBtn->bgColor = elBtn->hoverColor;}
		if(elBtn->pressed){setAlpha(255); elBtn->bgColor = elBtn->pressedColor;}
		render(el, "GUIframe");
		elBtn->bgColor = oldColor;
		if(elBtn->image){render(el, "GUIimage");}
		render(el, "GUIlabel");
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIframe"){
		GUIframe *elFrame = dynamic_cast<GUIframe*>(el); if(!elFrame){return;}
		if(!elFrame->noframe){
			renderComment(fstring("frame %p begin\n",this));
			if(GUIoptions.push){pushRenderOptions();}
			setColor(elFrame->bgColor);
			setAlpha(128);
			drawRect(el->worldArea());
			setAlpha(255);
			setColor(elFrame->borderColor);
			drawRectOutline(el->worldArea());
			if(GUIoptions.push){popRenderOptions();}
			renderComment(fstring("frame %p end\n",this));
		}
	}
	else if(type == "GUIgrid"){
		GUIgrid *elGD = dynamic_cast<GUIgrid*>(el); if(!elGD){return;}
		if(!elGD->bSizeToParent){
			rect R = el->worldArea();
			setColor(vec3(0,0,0));
			drawRectOutline(R);
		}
	}
	else if(type == "GUIimage"){
		GUIimage *elImg = dynamic_cast<GUIimage*>(el); if(!elImg){return;}
		if(GUIoptions.push){pushRenderOptions();}
		render(el, "GUIframe");
		setColor({255,255,255});
		setTexture(elImg->image);
		setTexturing(true);
		drawRect(el->worldArea());
		setTexturing(false);
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIlabel"){
		if(GUIoptions.push){pushRenderOptions();}
		GUIlabel *elLbl = dynamic_cast<GUIlabel*>(el); if(!elLbl){return;}
		setColor(elLbl->textColor);
		setFont(elLbl->textfont);
		rect tRect = elLbl->getTextRect();
		vec2 tp = getTextCentering(el->worldArea(),tRect,elLbl->alignment_vertical,elLbl->alignment_horizontal,elLbl->const_height,elLbl->textfont);
		setTextPos(vec2(tp.x,tp.y)+elLbl->textOffset);
		setScissor(el->visibleArea());
		printw("%s",elLbl->text.c_str());
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIscrollbarBar"){
		if(GUIoptions.push){pushRenderOptions();}
		GUIscrollbarBar *elScbb = dynamic_cast<GUIscrollbarBar*>(el); if(!elScbb){return;}
		vec2 pos = getMousePos();		
		vec3 oldColor = elScbb->bgColor;
		if(el->mouseover){setAlpha(196); elScbb->bgColor = elScbb->hoverColor;}
		if(elScbb->pressed){setAlpha(255); elScbb->bgColor = elScbb->pressedColor;}
		render(el, "GUIframe");
		elScbb->bgColor = oldColor;
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIscrollbar"){
		GUIscrollbar *elScb = dynamic_cast<GUIscrollbar*>(el); if(!elScb){return;}
		if(GUIoptions.push){pushRenderOptions();}
		render(el, "GUIframe");
		setAlpha(128);
		setColor(elScb->bgColor*0.9f);
		if(elScb->vertical){
			rect R = elScb->vtrackRect().setStart(elScb->vtrackRect().start-vec2{0,21})\
						.setEnd(elScb->vtrackRect().end+vec2{0,21});
			drawRect(elScb->thisToWorld(R));
		}
		if(elScb->horizontal){
			rect R = elScb->htrackRect().setStart(elScb->htrackRect().start-vec2{21,0})\
						.setEnd(elScb->htrackRect().end+vec2{21,0});
			drawRect(elScb->thisToWorld(R));
		}
		if(!elScb->bSizeToParent){
			setColor(vec3(0,0,64));
			drawRectOutline(elScb->worldArea());
		}
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUItextEntry"){
		GUItextEntry *elTE = dynamic_cast<GUItextEntry*>(el); if(!elTE){return;}
		if(GUIoptions.push){pushRenderOptions();}
		vec3 oldColor1 = elTE->bgColor;
		vec3 oldColor2 = elTE->hoverColor;
		if(elTE->hasfocus){elTE->hoverColor = elTE->bgColor = elTE->focusedColor;}
		render(el, "GUIbutton");
		if(elTE->hasfocus){
			rect TR;
			rect AR = elTE->worldArea();
			if(elTE->cursorPos){
				TR = elTE->getTextRect(elTE->text.substr(0,elTE->cursorPos));
			}else{
				TR = elTE->getTextRect("l");
				TR = TR.setSize(vec2(0,TR.size.y));
			}
			vec2 caretPos = AR.start+TR.end+elTE->textOffset;
			float diffRight = caretPos.x-AR.end.x+12.f;
			if(diffRight > 0){
				elTE->textOffset.x += -diffRight;
			}
			float diffLeft = AR.start.x+12.f-caretPos.x;
			if(diffLeft > 0){
				elTE->textOffset.x += diffLeft;
			}
			if(elTE->textOffset.x > 0){elTE->textOffset.x = 0;}
			rect CR = elTE->tcaret->getRect();
			caretPos.y = (AR.start.y+AR.end.y)/2.f-CR.size.y/2;
			caretPos.x -= CR.size.x*0.5f;
			rect IR = CR.moveBy(caretPos);
			setColor(vec3(255,255,255));
			drawImage(elTE->tcaret,IR);
			setTexturing(false);
		}else{
			elTE->textOffset = vec2(0,0);
		}
		elTE->bgColor = oldColor1;
		elTE->hoverColor = oldColor2;
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIwindow"){
		GUIwindow *elWin = dynamic_cast<GUIwindow*>(el); if(!elWin){return;}
		if(GUIoptions.push){pushRenderOptions();}
		vec2 pos = getMousePos();
		setColor(elWin->borderColor);
		drawRect(elWin->thisToWorld(rect(el->area.size.x,23)));
		render(el, "GUIframe");
		if(GUIoptions.push){popRenderOptions();}
	}
	else if(type == "GUIselectionGroup"){
		GUIselectionGroup *elSG = dynamic_cast<GUIselectionGroup*>(el); if(!elSG){return;}
		if(!elSG->bSizeToParent){
			setColor(vec3(0,0,64));
			drawRectOutline(elSG->worldArea());
		}
	}
	else if(type == "GUIselectable"){
		GUIselectable *elSel = dynamic_cast<GUIselectable*>(el); if(!elSel){return;}
		render(el, "GUIframe");
		if(elSel->selected){
			rect R = el->worldArea();
			R = R.setStart(R.start-vec2(1,1)).setEnd(R.end+vec2(1,1));
			setColor(vec3(255,255,255));
			drawRectOutline(R);
			R = R.setStart(R.start-vec2(1,1)).setEnd(R.end+vec2(1,1));
			setColor(vec3(0,0,0));
			drawRectOutline(R);
		}
	}
	else if(type == "GUIgrid"){
		GUIgrid *elGrid = dynamic_cast<GUIgrid*>(el); if(!elGrid){return;}
		rect R = el->worldArea();
		setColor(vec3(255,0,0));
		drawRectOutline(R);
	}
}
;