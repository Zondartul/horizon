#include "paint.h"
#include <vector>
using std::vector;
#include "GL/gl.h"
#include "globals.h" //firstn() macro
#include "selfaware.h"
renderOptions currentRenderOptions;
vector<renderOptions> renderOptionsStack;

void pushRenderOptions(){
	//return;
	renderOptionsStack.push_back(currentRenderOptions);
	//firstn(20,printf("alpha = %d\n",currentRenderOptions.alpha));
	//firstn(20,printf("push RO: %d\n",renderOptionsStack.size()));
}
void popRenderOptions(){
	//return;
	//firstn(20,printf("alpha = %d\n",currentRenderOptions.alpha));
	//firstn(20,printf("pop RO: %d\n",renderOptionsStack.size()));
	if(renderOptionsStack.size()){
		currentRenderOptions = renderOptionsStack.back();
		if(currentRenderOptions.scissorEnabled){setScissor(currentRenderOptions.scissor);}
		else{clearScissor();}
		
		glLineWidth(currentRenderOptions.thickness);
		renderOptionsStack.pop_back();
	}
	//firstn(20,printf("alpha = %d\n",currentRenderOptions.alpha));
}

void initRenderOptions(){
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	//glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_SCISSOR_TEST);
	//glEnable(GL_SCISSOR_TEST);
	
	currentRenderOptions.color1 = {0,0,0};
	currentRenderOptions.color2 = {0,0,0};
	currentRenderOptions.color3 = {0,0,0};
	currentRenderOptions.color4 = {0,0,0};
	currentRenderOptions.alpha = 255;
	//currentRenderOptions.thickness = 1;
	setThickness(1);
	currentRenderOptions.scissorEnabled = false;
	currentRenderOptions.textMaxX = -1;
	currentRenderOptions.textMaxY = -1;
	currentRenderOptions.textPos = {0,0};
	currentRenderOptions.infiniteLine = false;
	currentRenderOptions.infinitePlane = false;
	currentRenderOptions.tex = texture();
	currentRenderOptions.f = font();
	currentRenderOptions.textScale = 1;
}
void setColor(vec3ub color){
	currentRenderOptions.color1 = color;
	currentRenderOptions.color2 = color;
	currentRenderOptions.color3 = color;
	currentRenderOptions.color4 = color;
}
void setColor(vec3ub color1, vec3ub color2){
	currentRenderOptions.color1 = color1;
	currentRenderOptions.color2 = color2;
	currentRenderOptions.color3 = color1;
	currentRenderOptions.color4 = color2;
}
void setColor(vec3ub color1, vec3ub color2, vec3ub color3){
	currentRenderOptions.color1 = color1;
	currentRenderOptions.color2 = color2;
	currentRenderOptions.color3 = color3;
	currentRenderOptions.color4 = color1;
}
void setColor(vec3ub color1, vec3ub color2, vec3ub color3, vec3ub color4){
	currentRenderOptions.color1 = color1;
	currentRenderOptions.color2 = color2;
	currentRenderOptions.color3 = color3;
	currentRenderOptions.color4 = color4;
}
void setAlpha(unsigned char alpha){
	currentRenderOptions.alpha = alpha;
}
void setThickness(float thickness){
	currentRenderOptions.thickness = thickness;
	glLineWidth(thickness);
	glPointSize(thickness);
}
void setScissor(rect scissor){
	currentRenderOptions.scissorEnabled = true;
	glEnable(GL_SCISSOR_TEST);
	currentRenderOptions.scissor = scissor;
	vec2i scr = getScreenSize();
	glScissor(scissor.start.x-1,scr.y-scissor.end.y-1,scissor.size.x+1,scissor.size.y+1);
}
void cropScissor(rect scissor){
	if(!currentRenderOptions.scissorEnabled){
		vec2i scr = getScreenSize();
		setScissor(rect(scr));
	}
	setScissor(currentRenderOptions.scissor.clamp(scissor));
	
}
void clearScissor(){
	currentRenderOptions.scissorEnabled = false;
	glDisable(GL_SCISSOR_TEST);
}
void setTextMaxX(int textMaxX){currentRenderOptions.textMaxX = textMaxX;}
void setTextMaxY(int textMaxY){currentRenderOptions.textMaxY = textMaxY;}
void setTextPos(vec2i textPos){currentRenderOptions.textPos = textPos;}
void setInfiniteLine(bool infiniteLine){currentRenderOptions.infiniteLine = infiniteLine;}
void setInfinitePlane(bool infinitePlane){currentRenderOptions.infinitePlane = infinitePlane;}
void setTexture(texture t){currentRenderOptions.tex = t;}
void setFont(font f){currentRenderOptions.f = f;}
void setModel(model m){currentRenderOptions.m = m;}
void setTextScale(float scale){currentRenderOptions.textScale = scale;}

void clearScreen(){
	vec3ub color1 = currentRenderOptions.color1;
	glClearColor(color1.r/255.0,
				color1.g/255.0,
				color1.b/255.0,
				1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void drawPoint2D(vec2i pos){
	vec3ub color1 = currentRenderOptions.color1;
	unsigned char alpha = currentRenderOptions.alpha;
	glBegin(GL_POINTS);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(pos.x,pos.y);
	glEnd();
}
void drawLine2D(vec2i start, vec2i end){
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	unsigned char alpha = currentRenderOptions.alpha;
	glBegin(GL_LINES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(start.x,start.y);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex2i(end.x,end.y);
	glEnd();
}
void drawRect(rect R){
	vec2i a,b,c,d;
	a = R.start;
	b = {R.end.x,R.start.y};
	c = R.end;
	d = {R.start.x,R.end.y};
	
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	vec3ub color3 = currentRenderOptions.color3;
	vec3ub color4 = currentRenderOptions.color4;
	unsigned char alpha = currentRenderOptions.alpha;
	
	glBegin(GL_TRIANGLES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(a.x,a.y);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex2i(b.x,b.y);
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glVertex2i(c.x,c.y);
		
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glVertex2i(c.x,c.y);
		glColor4ub(color3.r,color3.g,color3.b,alpha);
		glVertex2i(d.x,d.y);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(a.x,a.y);
	glEnd();
}
/* void drawTexturedRect(rect R){
	vec2i a,b,c,d;
	a = R.start;
	b = {R.end.x,R.start.y};
	c = R.end;
	d = {R.start.x,R.end.y};
	
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	vec3ub color3 = currentRenderOptions.color3;
	vec3ub color4 = currentRenderOptions.color4;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, currentRenderOptions.tex.handle.t);
	
	glBegin(GL_TRIANGLES);
		glColor4ub(color1.r,color1.g,color1.b);
		glTexCoord2f(0,0);
		glVertex2i(a.x,a.y);
		glColor4ub(color2.r,color2.g,color2.b);
		glTexCoord2f(1,0);
		glVertex2i(b.x,b.y);
		glColor4ub(color4.r,color4.g,color4.b);
		glTexCoord2f(1,1);
		glVertex2i(c.x,c.y);
		
		glColor4ub(color4.r,color4.g,color4.b);
		glTexCoord2f(1,1);
		glVertex2i(c.x,c.y);
		glColor4ub(color3.r,color3.g,color3.b);
		glTexCoord2f(0,1);
		glVertex2i(d.x,d.y);
		glColor4ub(color1.r,color1.g,color1.b);
		glTexCoord2f(0,0);
		glVertex2i(a.x,a.y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
} */
void drawTexturedRect(rect R){drawTexturedRectUV(R,rectf(1,1));}
void drawTexturedRectUV(rect R, rectf UV){
	vec2i a,b,c,d;
	a = R.start;
	b = {R.end.x,R.start.y};
	c = R.end;
	d = {R.start.x,R.end.y};
	
	vec2f ua,ub,uc,ud;
	ua = UV.start;
	ub = {UV.end.x,UV.start.y};
	uc = UV.end;
	ud = {UV.start.x,UV.end.y};
	
	/* int hscale = currentRenderOptions.tex.handle.BMP.height;
	int wscale = currentRenderOptions.tex.handle.BMP.width;
	ua.x = ua.x / wscale;
	ua.y = ua.y / hscale;
	ub.x = ub.x / wscale;
	ub.y = ub.y / hscale;
	uc.x = uc.x / wscale;
	uc.y = uc.y / hscale;
	ud.x = ud.x / wscale;
	ud.y = ud.y / hscale; */
	
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	vec3ub color3 = currentRenderOptions.color3;
	vec3ub color4 = currentRenderOptions.color4;
	unsigned char alpha = currentRenderOptions.alpha;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, currentRenderOptions.tex.handle.t);
	
	glBegin(GL_TRIANGLES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glTexCoord2f(ua.x,ua.y);
		glVertex2i(a.x,a.y);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glTexCoord2f(ub.x,ub.y);
		glVertex2i(b.x,b.y);
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glTexCoord2f(uc.x,uc.y);
		glVertex2i(c.x,c.y);
		
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glTexCoord2f(uc.x,uc.y);
		glVertex2i(c.x,c.y);
		glColor4ub(color3.r,color3.g,color3.b,alpha);
		glTexCoord2f(ud.x,ud.y);
		glVertex2i(d.x,d.y);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glTexCoord2f(ua.x,ua.y);
		glVertex2i(a.x,a.y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void drawRectOutline(rect R){
	vec2i a,b,c,d;
	a = R.start;
	b = {R.end.x,R.start.y};
	c = R.end;
	d = {R.start.x,R.end.y};
	
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	vec3ub color3 = currentRenderOptions.color3;
	vec3ub color4 = currentRenderOptions.color4;
	unsigned char alpha = currentRenderOptions.alpha;
	
	glBegin(GL_LINES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(a.x,a.y);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex2i(b.x,b.y);
		
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex2i(b.x,b.y);
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glVertex2i(c.x,c.y);
		
		glColor4ub(color4.r,color4.g,color4.b,alpha);
		glVertex2i(c.x,c.y);
		glColor4ub(color3.r,color3.g,color3.b,alpha);
		glVertex2i(d.x,d.y);
		
		glColor4ub(color3.r,color3.g,color3.b,alpha);
		glVertex2i(d.x,d.y);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex2i(a.x,a.y);
	glEnd();
}

void drawPoint3D(vec3f pos){
	vec3ub color1 = currentRenderOptions.color1;
	unsigned char alpha = currentRenderOptions.alpha;
	glBegin(GL_POINTS);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex3f(pos.x,pos.y,pos.z);
	glEnd();
}
void drawLine3D(vec3f start, vec3f end){
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	unsigned char alpha = currentRenderOptions.alpha;
	glBegin(GL_LINES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex3f(start.x,start.y,start.z);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex3f(end.x,end.y,end.z);
	glEnd();
}
void drawTriangle3D(vec3f A, vec3f B, vec3f C){
	vec3ub color1 = currentRenderOptions.color1;
	vec3ub color2 = currentRenderOptions.color2;
	vec3ub color3 = currentRenderOptions.color3;
	unsigned char alpha = currentRenderOptions.alpha;
	glBegin(GL_TRIANGLES);
		glColor4ub(color1.r,color1.g,color1.b,alpha);
		glVertex3f(A.x,A.y,A.z);
		glColor4ub(color2.r,color2.g,color2.b,alpha);
		glVertex3f(B.x,B.y,B.z);
		glColor4ub(color3.r,color3.g,color3.b,alpha);
		glVertex3f(C.x,C.y,C.z);
	glEnd();
}
void drawModel3D(vec3f pos, vec3f rot, vec3f scale){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_position[] = {1,1,1,0};//{ cos((float)ticks/100), cos((float)ticks/100), sin((float)ticks/100), 0.0 };
	GLfloat light_ambient[] = {125/255.0f,206/255.0f,250/255.0f,1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	
	
	vec3ub color = currentRenderOptions.color1;
	unsigned char alpha = currentRenderOptions.alpha;
	
	
	
	glColor4ub(color.r,color.g,color.b,alpha);
	glPushMatrix();
	glFrontFace(GL_CCW);
	
	glTranslatef(pos.x,pos.y,pos.z);
	//
	glScalef(scale.x,scale.y,scale.z);	
	//
	glRotatef(rot.x,1,0,0);
	glRotatef(rot.y,0,1,0);
	glRotatef(rot.z,0,0,1);
	//
	auto &M = currentRenderOptions.m;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, M.t.handle.t);
	int th = M.t.h();
	int tw = M.t.w();
	glBegin(GL_TRIANGLES);
		for(auto I = M.tris.begin(); I != M.tris.end(); I++){
			auto &A = I->v[0];
			auto &B = I->v[1];
			auto &C = I->v[2];
			//glColor3ub(255,0,0);
			glTexCoord2f(A.uv.x,th-1-A.uv.y);
			glNormal3f(A.normal.x,A.normal.y,A.normal.z);
			glVertex3f(A.pos.x,A.pos.y,A.pos.z);
			//glColor3ub(0,255,0);
			glTexCoord2f(B.uv.x,th-1-B.uv.y);
			glNormal3f(B.normal.x,B.normal.y,B.normal.z);
			glVertex3f(B.pos.x,B.pos.y,B.pos.z);
			//glColor3ub(0,0,255);
			glTexCoord2f(C.uv.x,th-1-C.uv.y);
			glNormal3f(C.normal.x,C.normal.y,C.normal.z);
			glVertex3f(C.pos.x,C.pos.y,C.pos.z);
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glPopMatrix();
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
}

void printText2D(const char *text){
	char C = *text;
	int x=currentRenderOptions.textPos.x,y=currentRenderOptions.textPos.y,I=0;
	texture ot = currentRenderOptions.tex;
	while(C != 0){
		if(currentRenderOptions.f.charmap.count(C)){
			glyph G = currentRenderOptions.f.charmap[C];
			texture t = G.t;
			
			//if(C == 'i'){
				//printf("print: t.UV: %s\n",t.UV.toString());
			
			setTexture(t);
			//setColor({0,255,0});
			//drawRectOutline(t.rect().moveBy({150,50}));
			//setColor({0,0,255});
			//drawRectOutline(t.rect().moveTo({x,y}));
			//setColor({255,255,255});
			float scale = currentRenderOptions.textScale;
			drawTexturedRectUV((t.rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y}),t.UV);
			x+=G.advance*scale;
			//}
			//printf("%c",C);
		}else{}//printf("_");}
		I++;
		C = text[I];
	}
	currentRenderOptions.textPos = {x,y};
	//printf("\n");
	setTexture(ot);
}

#include "simplemath.h"
rect preprintText2D(const char *text){
	int maxx=0,maxy=0,minx=0,miny=0;
	char C = *text;
	int x=0,y=0,I=0;
	texture ot = currentRenderOptions.tex;
	while(C != 0){
		if(currentRenderOptions.f.charmap.count(C)){
			glyph G = currentRenderOptions.f.charmap[C];
			texture t = G.t;
			
			//setTexture(t);
			float scale = currentRenderOptions.textScale;
			rect R = (t.rect().moveTo({G.bearingX,-G.bearingY})*scale).moveBy({x,y});
			
			maxx = max(maxx,R.end.x);
			maxy = max(maxy,R.end.y);
			minx = min(minx,R.start.x);
			miny = min(miny,R.start.y);
			
			x+=G.advance*scale;
		}
		I++;
		C = text[I];
	}
	//setTexture(ot);
	//firstn(10,printf("preprint: %d %d %d %d\n",minx,miny,maxx,maxy));
	//onceon(10,printf("preprint output suppressed\n"));
	return rect({minx,miny},{maxx,maxy});
}

#include "stdarg.h"
void printw(const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[80];
	vsnprintf(buff,79,format,ap);
	printText2D(buff);
}
rect preprintw(const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[80];
	vsnprintf(buff,79,format,ap);
	return preprintText2D(buff);
}







