#ifndef GUI3_GUARD
#define GUI3_GUARD
#include <list>
#include "../util/globals.h"
#include "../util/messenger.h"
class GUI3base;

typedef void (*func_v_g3)(GUI3base *arg);
typedef void (*func_v_g3v2)(GUI3base *arg, vec2i arg2);
class GUI3base2;
typedef class{public: virtual void operator()(GUI3base2* me){}} 							functor_v;
typedef class{public: virtual void operator()(GUI3base2* me, vec2i arg){}} 					functor_v2i;
typedef class{public: virtual void operator()(GUI3base2* me, vec2i arg1, message *arg2){}} 	functor_v2i_msg;
typedef class{public: virtual void operator()(GUI3base2* me, vec2i arg1, vec2i arg2){}} 	functor_v2i_v2i;
typedef class{public: virtual void operator()(GUI3base2* me, message *arg){}} 				functor_msg;
typedef class: public functor_msg, public messageReceiver{} 								functor_msg_mR;

class GUI3base2
{
	private:
	GUI3base2* parent;
	vec4i rect;
	public:
	functor_v *render; //called on every re-draw, but only when visible.
	functor_v *think;  //called every so-often.
	functor_v2i *mouseover; //called when cursor is hovering over the element.
	functor_v2i *mouseclick; //called for left-click only.
	functor_v2i_msg *mousebuttons; //called for every mouse button event, if has mouse focus.
	functor_v2i *mousemove; //called when mouse is moved, if has mouse focus.
	functor_msg *keyboardevent; //called for every keyboard event.
	functor_msg_mR *anyevent; //all events go here. This one is responsible for sending events elsewhere.

	
	GUI3base2();
	vec2i getPosLocal();
	void setPosLocal(vec2i newpos);
	vec2i getPosGlobal();
	void setPosGlobal(vec2i newpos);
	vec2i getSize();
	void setSize(vec2i newsize);
	vec4i getRectLocal();
	void setRectLocal(vec4i newrect);
	vec4i getRectGlobal();
	void setRectGlobal(vec4i newrect);
	vec2i posToLocal(vec2i pos);
	vec2i posToGlobal(vec2i pos);
	vec4i rectToLocal(vec4i rect2);
	vec4i rectToGlobal(vec4i rect2);
};

class G3StockRender2: public functor_v
{
	public:
	int c;
	vec3i bodyColor;
	vec3i borderColor;
	G3StockRender2();
	
	void operator()(GUI3base2* me);
};

class G3StockMouseover: public functor_v2i
{
	public:
	void operator()(GUI3base2* me, vec2i arg);
};
#define FUNCALL(a, b, ...) (*(a->b))(a, __VA_ARGS__)
//#define FUNCALL(a, b, ...) ;

class G3StockEventHandler: public functor_msg_mR
{
	public:
	GUI3base2* owner;
	void operator()(GUI3base2* me, message *msg);
	void receiveMessage(message *msg);
	void subscribe(GUI3base2* owner, MessageChannel *chan, string type);
	void unsubscribe(MessageChannel *chan, string type);
};

class G3StockMouseMove_drag: public functor_v2i
{
	public:
	bool isDragging;
	G3StockMouseMove_drag();
	void operator()(GUI3base2* me, vec2i delta);
};

class G3StockHandler_drag: public G3StockEventHandler
{
	public:
	void operator()(GUI3base2* me, message *msg);
};

extern GUI3base2 *Root;
void GUI3start();


void GUI3rendertick();
#endif