//  333333
// 33    33
//       33
//     333
//       33
// 33    33
//  333333

// okay lets make this count
#include "gui/Gui3.h"
#include <list>
#include "util/globals.h"
#include "util/messenger.h"
#include "display/paint.h"
#include "main/control.h" //bad

GUI3base2::GUI3base2()
{
	rect = {0,0,0,0};
	render = NULL;
	think = NULL;
	mouseover = NULL;
	mouseclick = NULL;
	mousebuttons = NULL;
	mousemove = NULL;
	keyboardevent = NULL;
	anyevent = NULL;
}

//by convention, rects are startpos until endpos.
vec2i GUI3base2::posToLocal(vec2i pos)
{
	return {pos.x-rect.x1, pos.y-rect.y1};
}
vec2i GUI3base2::posToGlobal(vec2i pos)
{
	return {pos.x+rect.x1, pos.y+rect.y1};
}
vec4i GUI3base2::rectToLocal(vec4i rect2)
{
	return {rect2.x1-rect.x1, rect2.y1-rect.y1, rect2.x2-rect.x1, rect2.y2-rect.y1};
}
vec4i GUI3base2::rectToGlobal(vec4i rect2)
{
	return {rect2.x1+rect.x1, rect2.y1+rect.y1, rect2.x2+rect.x1, rect2.y2+rect.y1};
}
void GUI3base2::setRectGlobal(vec4i newrect)
{
	vec4i prevRect = rect;
	rect = newrect;
	// do children etc.
}
vec4i GUI3base2::getRectGlobal()
{
	return rect;
}
vec2i GUI3base2::getPosLocal()
{
	vec2i temp;
	temp.x = rect.x1;
	temp.y = rect.y1;
	if(parent)
		temp = parent->posToLocal(temp);
	return temp;
}
void GUI3base2::setPosLocal(vec2i newpos)
{
	if(parent)
		newpos = parent->posToGlobal(newpos);
	setRectGlobal({newpos.x,newpos.y,rect.x2,rect.y2});
}
vec2i GUI3base2::getPosGlobal()
{
	vec2i temp;
	temp.x = rect.x1;
	temp.y = rect.y1;
	return temp;
}
void GUI3base2::setPosGlobal(vec2i newpos)
{
	setRectGlobal({newpos.x, newpos.y, newpos.x+(rect.x2-rect.x1), 
					newpos.y+(rect.y2-rect.y1)});
}
vec2i GUI3base2::getSize()
{
	return {rect.x2-rect.x1, rect.y2-rect.y1};
}
void GUI3base2::setSize(vec2i newsize)
{
	setRectGlobal({rect.x1, rect.y1, rect.x1+newsize.x, rect.y1+newsize.y});
}
vec4i GUI3base2::getRectLocal()
{
	if(parent)
		return parent->rectToLocal(rect);
	else
		return rect;
}
void GUI3base2::setRectLocal(vec4i newrect)
{
	if(parent)
		setRectGlobal(parent->rectToGlobal(newrect));
	else
		setRectGlobal(newrect);
}
/*
struct G3parent
	{
		private:
		GUI3base2* p;
		public:
		GUI3base2* get(){return p;}
		void set(GUI3base2* newParent){p = newParent;}
	} parent;
	struct G3size
	{
		private:
		vec2i s;
		public:
		vec2i get(){return s;}
		void set(vec2i newsize){s = newsize;}
	}size;
	struct G3pos
	{
		private:
		vec2i posLocal;
		vec2i posGlobal;
		struct
		{
			vec2i get(){return posLocal;}
			vec4i getRect(){return {get().x, get().y, get().x+size.get().x, get().y+size.get().y};}
			void set(vec2i newpos)
			{
				posLocal = newpos;
				parent.get()? posGlobal = posLocal + parent.get()->pos.global.get() : posLocal;
			}
			void setRect(vec4i newRect)
			{
				set({newRect.x1, newRect.y1});
				size.set({newRect.x2 - newRect.x1, newRect.y2 - newRect.y1});
			}
		}local;
		struct
		{
			vec2i get(){return posGlobal;}
			vec4i getRect(){return {get().x, get().y, get().x+size.get().x, get().y+size.get().y};}
			void set(vec2i newpos)
			{
				posGlobal = newpos;
				parent.get()? posLocal = posGlobal - parent.get()->pos.local.get() : posGlobal;
			}
			void setRect(vec4i newRect)
			{
				set({newRect.x1, newRect.y1});
				size.set({newRect.x2 - newRect.x1, newRect.y2 - newRect.y1});
			}
		}global;
	}pos;
*/
/*
class GUI3base: public PSsubscriber //subscriber is only here temporarily, for testing it.
{
	private:
	vec2i posLocal;
	vec2i posGlobal;
	vec2i size;
	public:
	GUI3base *parent;
	list<GUI3base*> children;
	GUI3base()
	{
		printf("GUI3base created @ %p\n",this);
		render = NULL;
		parent = NULL;
	}
	void setParent(GUI3base *parent)
	{
		this->parent = parent;
		this->parent->children.push_back(this);
	}
	vec2i getPosLocal(){return posLocal;}
	void setPosLocal(vec2i newPos)
	{
		posLocal = newPos;
		posGlobal = parent? parent->getPosGlobal() + newPos : newPos;
		//update children positions
	}
	vec2i getPosGlobal(){return posGlobal;}
	void setPosGlobal(vec2i newPos)
	{
		posGlobal = newPos;
		posLocal = parent? newPos - parent->getPosGlobal() : newPos;
		//update children positions
	}
	void setSize(vec2i newSize)
	{
		size = newSize;
		//update children positions
	}
	void setPosRelative(vec2i newPos){setPosLocal({posLocal.x+newPos.x, posLocal.y+newPos.y});}
	vec4i getRectLocal(){return (vec4i){posLocal.x, posLocal.y, posLocal.x + size.x, posLocal.y + size.y};}
	vec4i getRectGlobal(){return (vec4i){posGlobal.x, posGlobal.y, posGlobal.x + size.x, posGlobal.y + size.y};}
	func_v_g3 render;
	func_v_g3 think;
	func_v_g3v2 mouseOver;
	void PSreceive(message newMsg)
	{
		if(newMsg.type == "stuff")
		{
			GUIbase *box = (GUIbase *)newMsg.pop<void *>();
			box->setPos(box->pos.x, box->pos.y+16);
			cout << toString(this) + " received msg: [" + newMsg.pop<string>() + "]\n";
		}
		else
		{
			string key = newMsg.pop<string>();
			cout << newMsg.type << " " << key << "\n";
			if((newMsg.type == "key_down") || (newMsg.type == "key_still_down"))
			{
				if(key == "downarrow"){setPosRelative({0,10});}
				if(key == "uparrow"){setPosRelative({0,-10});}
				if(key == "leftarrow"){setPosRelative({-10,0});}
				if(key == "rightarrow"){setPosRelative({10,0});}
			}
		}
	}
};
*/
/*
void G3stockRender1(GUI3base *me)
{
	vec4i p = me->getRectGlobal();
	glColor3f(0.5f, 0.9f, 0.9f);
	paintRect(p.x1, p.y1, p.x2 ,p.y2);
	glColor3f(0.0f, 0.0f, 0.0f);
	paintRectOutline(p.x1, p.y1, p.x2, p.y2);
}
*/

G3StockRender2::G3StockRender2()
{
	c = 0;
	bodyColor = {0,0,0};
	borderColor = {0,0,0};
}

void G3StockRender2::operator()(GUI3base2* me)
{
	c = c < 255 ? c+1: 0;
	vec4i p = me->getRectGlobal();
	glColor3ub(bodyColor.x, bodyColor.y, c);
	paintRect(p.x1, p.y1, p.x2, p.y2);
	glColor3ub(borderColor.x, borderColor.y, borderColor.z);
	paintRectOutline(p.x1, p.y1, p.x2, p.y2);
}
//Dear Princess Celestia,
//
//something about functions having to be defined out-of-line
//or else I get missing vtables, because needs at least 
//one vtable for non-pure-virtual function per class?... derps
//ended up putting an empty body into functor_v so virtual function is not pure.
//
//Vtables are gay.


void G3StockMouseover::operator()(GUI3base2* me, vec2i arg)
{
	if(me->getRectGlobal().contains(arg))
		((G3StockRender2*)(me->render))->bodyColor = {0,255,0};
	else
		((G3StockRender2*)(me->render))->bodyColor = {255,0,0};
}
#define FUNCALL(a, b, ...) (*(a->b))(a, __VA_ARGS__)
//#define FUNCALL(a, b, ...) ;

void G3StockEventHandler::operator()(GUI3base2* me, message msg)
{
	printf("G3SEH: %p received msg of type [%s]\n", me, msg.type.c_str());
	if(msg.type == "lmb_down"){if(owner->mouseclick) FUNCALL(owner, mouseclick, input.getMousePos());}
	if(msg.type == "lmb_down"
	|| msg.type == "lmb_up"
	|| msg.type == "rmb_down"
	|| msg.type == "rmb_up"){if(owner->mousebuttons) FUNCALL(owner, mousebuttons, input.getMousePos(), msg);}
	if(msg.type == "mouse_move"){if(owner->mousemove) FUNCALL(owner, mousemove, msg.data.v2i);}
	if(msg.type == "key_down"
	|| msg.type == "key_up"){if(owner->keyboardevent) FUNCALL(owner, keyboardevent, msg);}
}
void G3StockEventHandler::PSreceive(message msg)
{
	(*this)(owner, msg);
	//printf("G3SEH received...\n");
}
void G3StockEventHandler::subscribe(GUI3base2* owner, PSchannel *chan, string type)
{
	this->owner = owner;
	chan->subscribe(type, this);
	//printf("G3SEH subscribed...\n");
}
void G3StockEventHandler::unsubscribe(PSchannel *chan, string type)
{
	chan->unsubscribe(type, this);
}

G3StockMouseMove_drag::G3StockMouseMove_drag()
{
	isDragging = false;
}
void G3StockMouseMove_drag::operator()(GUI3base2* me, vec2i delta)
{
	if(isDragging)
		me->setPosGlobal(me->getPosGlobal()+delta);
}

void G3StockHandler_drag::operator()(GUI3base2* me, message msg)
{
	G3StockEventHandler::operator()(me, msg);
	if(msg.type == "lmb_down")
		((G3StockMouseMove_drag*)(me->mousemove))->isDragging = true;
	if(msg.type == "lmb_up")
		((G3StockMouseMove_drag*)(me->mousemove))->isDragging = false;
}
/*void G3StockRender2::operator()(GUI3base2* me)
{
	c = c < 1.0f ? c+0.01f: 0.0f;
	vec4i p = me->getRectGlobal();
	glColor3f(0.9f, 0.5f, c);
	paintRect(p.x1, p.y1, p.x2, p.y2);
	glColor3f(0.0f, 0.0f, 0.0f);
	paintRectOutline(p.x1, p.y1, p.x2, p.y2);
}
*/
/*
void G3stockMouseDragSize(GUI3base *me)
{
	//wait lets make input library first.
}

void G3stockMouseOver_Propagate(GUI3base *me, vec2i mousePos)
{
	for(list<GUI3base *>::iterator I = me->children.begin(); I != me->children.end(); I++)
	{
		vec2i localMousePos = mousePos - (**I).getPosLocal();
		if((**I).getRectLocal().contains(localMousePos))
		{
			if((**I).mouseOver)
				(**I).mouseOver(*I, localMousePos);
			return;
		}
	}
	// if above were put in a bool function, below would be things to do if
	// none of the elements were moused over.
}
*/
GUI3base2 *Root = NULL;
void GUI3start()
{
	printf("\nGUI3: hi\n");
	printf("\nSize of size_t is %u\n", sizeof(size_t));
	Root = new GUI3base2();
	Root->setPosGlobal({64,64});
	Root->setSize({64,64});
	Root->render = new G3StockRender2();
	((G3StockRender2*)(Root->render))->bodyColor = {255,0,0};
	Root->mouseover = new G3StockMouseover();
	Root->anyevent = new G3StockHandler_drag();
	((G3StockEventHandler*)(Root->anyevent))->subscribe(Root, &(input.channel), "");
	Root->mousemove = new G3StockMouseMove_drag();
	/*
	Root = new GUI3base();
	Root->setPosGlobal({64,64});
	Root->setSize({64,64});
	Root->render = &G3stockRender1;
	GUI_PS.subscribe("stuff", Root);
	input.channel.subscribe("", Root);
	*/
}


void GUI3rendertick()
{
	(*(Root->render))(Root);
	(*(Root->mouseover))(Root, input.getMousePos()); 
}