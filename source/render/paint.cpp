#include "paint.h"
#include "stdio.h"
#include "globals.h"

#include "renderLayer.h"
#include "rmodel.h"
#include "model.h"
#include <vector>
using std::vector;

renderLayer *currentLayer = 0;
//core functions

vector<renderLayer*> layers;

void setLayer(renderLayer *L){currentLayer = L;}
void addLayer(renderLayer *L){
	if(currentLayer){
		for(auto I = layers.begin(); I!= layers.end(); I++){
			if(*I == currentLayer){
				layers.insert(I+1,L);
				return;
			}
		}
	}else{
		layers.push_back(L);
	}
}
void addLayer(renderLayer *L1, renderLayer *L2){
	for(auto I = layers.begin(); I!= layers.end(); I++){
			if(*I == L1){
				layers.insert(I+1,L2);
				return;
			}
	}
}
renderLayer *addNewLayer(){
	renderLayer *L = new renderLayer();
	addLayer(L);
	return L;
}
void removeLayer(renderLayer *L){
	for(auto I = layers.begin(); I!= layers.end(); I++){
		if(*I == L){
			I = layers.erase(I);
		}
	}
}
bool printAllPending = false;
void renderAllLayers(){
	if(printAllPending){printAllPending = false; printAllLayers();}
	for(auto I = layers.begin(); I!= layers.end(); I++){
		(*I)->render();
	}
}
void printAllLayers(){
	int J = 0;
	for(auto I = layers.begin(); I != layers.end(); I++, J++){
		printf("layer %d:\n",J);
		(*I)->print();
	}
}
void printAllLayersNextRender(){printAllPending = true;}

void setColoring(bool b){currentLayer->queue.push_back(new rcmd_coloring(b));}				//enables per-vertex coloring (no color = white)
void setTransparency(bool b){currentLayer->queue.push_back(new rcmd_transparency(b));}		//enables transparency (no transparency = alpha 1)
void setTexturing(bool b){currentLayer->queue.push_back(new rcmd_texturing(b));}			//enables texturing (no texture = colors only)
void setDebug(bool b){currentLayer->queue.push_back(new rcmd_debug(b));}					//enables debug rendering mode
void setScissoring(bool b){currentLayer->queue.push_back(new rcmd_scissoring(b));}			//enable scissor test

void setColor(vec3f col){currentLayer->queue.push_back(new rcmd_color(col));}				//sets global color (default = 1,1,1)
void setAlpha(float a){currentLayer->queue.push_back(new rcmd_alpha(a));}					//sets global alpha (default = 1), needs transparency
void setTexture(texture *t){currentLayer->queue.push_back(new rcmd_texture_select(t));}		//selects current texture, needs upload
void setFont(font *f){currentLayer->queue.push_back(new rcmd_font_select(f));}				//selects current font, needs upload
void setRenderMode(int mode){currentLayer->queue.push_back(new rcmd_mode_select(mode));}	//0 - points, 1 - lines, 3 - triangles, 4 - triangles (wireframe)
void setTextPos(vec2 textPos){currentLayer->queue.push_back(new rcmd_text_pos(textPos));} 	//textPos is advanced automatically after each print
void setScissor(rect S){currentLayer->queue.push_back(new rcmd_scissor(S));}				//limits the drawn-to area
void setPointSize(float size){currentLayer->queue.push_back(new rcmd_pointsize(size));}		//sets size of single points (not lines)
void setLineWidth(float width){currentLayer->queue.push_back(new rcmd_linewidth(width));}	//sets the width of lines

void uploadTexture(texture *t){currentLayer->queue.push_back(new rcmd_texture_upload(t));}	//uploads texture to GPU
void uploadRmodel(rmodel *rm){currentLayer->queue.push_back(new rcmd_rmodel_upload(rm));}	//uploads rmodel to GPU
void deleteRmodel(rmodel *rm){currentLayer->queue.push_back(new rcmd_rmodel_delete(rm));}	//deletes rmodel from GPU

void setProjection(mat4 proj){currentLayer->queue.push_back(new rcmd_projection(proj));}	//changes current projection matrix
void setPosition(vec3 pos){currentLayer->queue.push_back(new rcmd_position(pos));}			//changes current world pos
void setScale(vec3 scale){currentLayer->queue.push_back(new rcmd_scale(scale));}       		//changes current object scale 

void clearScreen(){currentLayer->queue.push_back(new rcmd_clear_screen());}					//clears screen with current color
void drawRmodel(rmodel *rm){currentLayer->queue.push_back(new rcmd_rmodel_render(rm));}//draws rendermodel
void printText(string text){currentLayer->queue.push_back(new rcmd_print_text(text));}	//draws text at current textPos

//simple drawing functions:
void drawPoint(vec3 pos){
	rmodel *rm = new rmodel(); 
	rm->vertices->push_back(pos); 
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(1);
	drawRmodel(rm); 
	deleteRmodel(rm);
}
void drawPoint(vec3 pos,vec3f color){
	setColor(color);
	rmodel *rm = new rmodel(); 
	rm->vertices->push_back(pos); 
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(1);
	drawRmodel(rm); 
	deleteRmodel(rm);
}
void drawLine(vec3 start, vec3 end){
	rmodel *rm = new rmodel(); 
	rm->vertices->push_back(start); 
	rm->vertices->push_back(end); 
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm); 
	deleteRmodel(rm);
}
void drawLine(vec3 start, vec3 end,vec3f color){
	setColor(color);
	rmodel *rm = new rmodel(); 
	rm->vertices->push_back(start); 
	rm->vertices->push_back(end); 
	rm->finalize(); 
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm); 
	deleteRmodel(rm);
}
void drawTriangle(vec3 A, vec3 B, vec3 C){
	rmodel *rm = new rmodel();
	rm->vertices->push_back(A); rm->uvs->push_back(vec2(0,0)); rm->colors->push_back(vec3(1,0,0));
	rm->vertices->push_back(B); rm->uvs->push_back(vec2(0,1)); rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(C); rm->uvs->push_back(vec2(1,1)); rm->colors->push_back(vec3(0,0,1));
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(3);
	drawRmodel(rm);
	deleteRmodel(rm);
}
void drawRect(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.start.y,0.0f);
	vec3 D = vec3(R.end.x,R.end.y,0.0f);
	vec2 UVA = vec2(0,0);//*0.1f;
	vec2 UVB = vec2(0,1);//*0.1f;
	vec2 UVC = vec2(1,0);//*0.1f;
	vec2 UVD = vec2(1,1);//*0.1f;
	rm->vertices->push_back(A);
	rm->vertices->push_back(B);
	rm->vertices->push_back(C);
	rm->vertices->push_back(B);
	rm->vertices->push_back(C);
	rm->vertices->push_back(D);
	
	rm->uvs->push_back(UVA);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVC);
	rm->uvs->push_back(UVB);
	rm->uvs->push_back(UVC);
	rm->uvs->push_back(UVD);
	
	
	rm->colors->push_back(vec3(1,0,0));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,1,1));
	rm->colors->push_back(vec3(1,0,0));
	
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(3);
	drawRmodel(rm);
	deleteRmodel(rm);
}
void drawRectOutline(rect R){
	rmodel *rm = new rmodel();
	vec3 A = vec3(R.start.x,R.start.y,0.0f);
	vec3 B = vec3(R.start.x,R.end.y,0.0f);
	vec3 C = vec3(R.end.x,R.end.y,0.0f);
	vec3 D = vec3(R.end.x,R.start.y,0.0f);
	vec2 UVA = vec2(0,0);//*0.1f;
	vec2 UVB = vec2(0,1);//*0.1f;
	vec2 UVC = vec2(1,0);//*0.1f;
	vec2 UVD = vec2(1,1);//*0.1f;
	
	rm->vertices->push_back(A);	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));
	rm->vertices->push_back(B);	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));
	
	rm->vertices->push_back(B);	rm->uvs->push_back(UVB);	rm->colors->push_back(vec3(0,1,0));
	rm->vertices->push_back(C);	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));
	
	rm->vertices->push_back(C);	rm->uvs->push_back(UVC);	rm->colors->push_back(vec3(0,0,1));
	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));
	
	rm->vertices->push_back(D);	rm->uvs->push_back(UVD);	rm->colors->push_back(vec3(1,1,0));
	rm->vertices->push_back(A);	rm->uvs->push_back(UVA);	rm->colors->push_back(vec3(1,0,0));
	
	
	rm->finalize();
	uploadRmodel(rm);
	setRenderMode(2);
	drawRmodel(rm);
	deleteRmodel(rm);
}
void drawRectOutlineColored(rect R, vec3f col){
	setScissoring(false);
	setColor(col);
	drawRectOutline(R);
}
void drawModel(vec3 pos, vec3 rot, vec3 scale, model *m){
	rmodel *rm;
	if(m->rm){
		rm = m->rm;
		drawRmodel(rm);
	}else{
		rm = new rmodel(m);
		uploadRmodel(rm);
		drawRmodel(rm);
		deleteRmodel(rm);
	}
}

void renderComment(string str){
	currentLayer->queue.push_back(new rcmd_comment(str));
}

void setDepthTest(bool b){currentLayer->queue.push_back(new rcmd_depth_test(b));}
//these are in printw.cpp
//rect preprintText2D(const char *text);
//void printw(const char *format, ...);
//rect preprintw(const char *format, ...);	//returns the area taken up by the printed string


// void setTexture(texture *t){
	// rqueue->push_back(new rcmd_texture_select(t));
// }


// void drawTexturedRectUV(rect area,rectf UV){
	// rmodel *rm = new rmodel();
	// vec3 A = vec3(area.start.x,area.start.y,0.0f)*0.05f;
	// vec3 B = vec3(area.start.x,area.end.y,0.0f)*0.05f;
	// vec3 C = vec3(area.end.x,area.start.y,0.0f)*0.05f;
	// vec3 D = vec3(area.end.x,area.end.y,0.0f)*0.05f;
	// vec2 UVA = vec2(UV.start.x,UV.start.y);//*0.1f;
	// vec2 UVB = vec2(UV.start.x,UV.end.y);//*0.1f;
	// vec2 UVC = vec2(UV.end.x,UV.start.y);//*0.1f;
	// vec2 UVD = vec2(UV.end.x,UV.end.y);//*0.1f;
	// rm->vertices->push_back(A);
	// rm->vertices->push_back(B);
	// rm->vertices->push_back(C);
	// rm->vertices->push_back(B);
	// rm->vertices->push_back(C);
	// rm->vertices->push_back(D);
	
	// rm->uvs->push_back(UVA);
	// rm->uvs->push_back(UVB);
	// rm->uvs->push_back(UVC);
	// rm->uvs->push_back(UVB);
	// rm->uvs->push_back(UVC);
	// rm->uvs->push_back(UVD);
	
	
	// rm->colors->push_back(vec3(1,0,0));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,1,1));
	// rm->colors->push_back(vec3(1,0,0));
	
	// once(printf("A = (%f,%f,%f)\n",A.x,A.y,A.z));
	// once(printf("B = (%f,%f,%f)\n",B.x,B.y,B.z));
	// once(printf("C = (%f,%f,%f)\n",C.x,C.y,C.z));
	// once(printf("D = (%f,%f,%f)\n",D.x,D.y,D.z));
	
	// once(printf("UVA = (%f,%f)\n",UVA.x,UVA.y));
	// once(printf("UVB = (%f,%f)\n",UVB.x,UVB.y));
	// once(printf("UVC = (%f,%f)\n",UVC.x,UVC.y));
	// once(printf("UVD = (%f,%f)\n",UVD.x,UVD.y));
	
	// rm->finalize();
	// rqueue->push_back(new rcmd_rmodel_upload(rm));
	// rqueue->push_back(new rcmd_coloring(true));
	// rqueue->push_back(new rcmd_texturing(true));
	// rqueue->push_back(new rcmd_rmodel_render(rm,3));
	// rqueue->push_back(new rcmd_rmodel_delete(rm));
// }