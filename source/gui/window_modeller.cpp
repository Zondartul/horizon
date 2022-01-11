#include "gui/window_modeller.h"
#include "gui/Gui2.h"
#include "main/control.h"
#include "game/camera.h"
#include <algorithm>

GUI2frame *frame;
vector<GUI2button*> buttons;
model *M;
rmodel *RM;
vec vector_cursor_pos;
uint32_t cur_renderflags = RENDER_DEFAULT | COLOR_DEFAULT | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT;
int cur_alpha = 255;
vector<int> selected;
void updateRender(){
	RM->regenerate();
	RM->setrenderflags(cur_renderflags, 1);
	RM->color.a = cur_alpha;
	for(int I = 0; I < selected.size(); I++){
		rtriangle &rtri = RM->triangles[selected[I]];
		rtri.setrenderflags(RENDER_FACES | RENDER_BACKFACES | COLOR_UNIFORM | LIGHT_NONE | TRANSPARENCY_NONE,1);
		rtri.color = {255,255,0,255};
	}
}

void clickButtonNew(void *arg){
	printf("modeller: button new\n");
	vector_cursor_pos = camera.pos;//camerapos
	//free(M);
	M = new model;
	if(!RM){
		RM = new rmodel(M); 
		scene.push_back(RM);
	}
	RM->M = M;
	//RM->renderflags = RENDER_DEFAULT | COLOR_DEFAULT | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT;
	//RM->setrenderflags(RENDER_DEFAULT | COLOR_DEFAULT | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT, 1);
	updateRender();
}
void createButtonNew(){
	buttons[0]->setImage("../resource/textures/gui/iconnew.png");
	buttons[0]->func = &clickButtonNew;
	buttons[0]->arg = NULL;
}

void clickButtonMove(void *arg){
	for(int I = 0; I < M->vertices.size(); I++){
		vec &v = M->vertices[I];
		v.z = v.z+0.25;
	}
	updateRender();
	printf("modeller: button move\n");
}
void createButtonMove(){
	buttons[1]->setImage("../resource/textures/gui/iconmove.png");
	buttons[1]->func = &clickButtonMove;
	buttons[1]->arg = NULL;
}
void createButtonOpen(){
	buttons[2]->setImage("../resource/textures/gui/iconopen.png");
}
void createButtonScale(){
	buttons[3]->setImage("../resource/textures/gui/iconscale.png");
}
void createButtonSave(){
	buttons[4]->setImage("../resource/textures/gui/iconsave.png");
}
void createButtonRotate(){
	buttons[5]->setImage("../resource/textures/gui/iconrotate.png");
}
void createButtonSaveAs(){
	buttons[6]->setImage("../resource/textures/gui/iconsaveas.png");
}
void createButtonCopy(){
	buttons[7]->setImage("../resource/textures/gui/iconcopy.png");
}
void createButtonOptions(){
	buttons[8]->setImage("../resource/textures/gui/iconoptions.png");
}
void clickButtonDelete(void *arg){
	scene.clear();
	scene.push_back(RM);
}
void createButtonDelete(){
	buttons[9]->setImage("../resource/textures/gui/icondelete.png");
	buttons[9]->func = &clickButtonDelete;
	buttons[9]->arg = NULL;
}
void createButtonUndo(){
	buttons[10]->setImage("../resource/textures/gui/iconundo.png");
}
void createButtonRedo(){
	buttons[11]->setImage("../resource/textures/gui/iconredo.png");
}
void createButtonGrid(){
	buttons[12]->setImage("../resource/textures/gui/icongrid.png");
}
void createButtonExtrude(){
	buttons[13]->setImage("../resource/textures/gui/iconextrude.png");
}
void clickButtonPreview(void *arg){
	printf("model M = %p\n", M);
	printf("rmodel RM = %p\n", RM);
	printf("num vertices: %d\n",M->vertices.size());
	printf("num triangles: %d\n",M->mesh.size());
}
void createButtonPreview(){
	buttons[14]->setImage("../resource/textures/gui/iconpreview.png");
	buttons[14]->func = &clickButtonPreview;
	buttons[14]->arg = NULL;
}
void createButtonSweep(){
	buttons[15]->setImage("../resource/textures/gui/iconsweep.png");
}
void clickButtonTransparent(void *arg){
	//RM->renderflags = RENDER_OVERRIDE | RENDER_FACES | RENDER_BACKFACES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_UNIFORM;
	cur_renderflags = RENDER_FACES | RENDER_BACKFACES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_UNIFORM;
	cur_alpha = 128;
	updateRender();
	printf("rendering as transparent\n");
}
void createButtonTransparent(){
	buttons[16]->setImage("../resource/textures/gui/icontransparent.png");
	buttons[16]->func = &clickButtonTransparent;
	buttons[16]->arg = NULL;
}
void clickButtonMatte(void *arg){
	//RM->renderflags = RENDER_OVERRIDE | RENDER_FACES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_NONE;
	cur_renderflags = RENDER_FACES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_NONE;
	cur_alpha = 255;
	updateRender();
	printf("rendering as matte\n");
}
void createButtonMatte(){
	buttons[17]->setImage("../resource/textures/gui/iconmatte.png");
	buttons[17]->func = &clickButtonMatte;
	buttons[17]->arg = NULL;
}
void clickButtonWireframe(void *arg){
	//RM->renderflags = RENDER_OVERRIDE | RENDER_EDGES | RENDER_VERTICES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_NONE;
	cur_renderflags = RENDER_EDGES | RENDER_VERTICES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_NONE;
	cur_alpha = 255;
	updateRender();
	printf("rendering as wireframe\n");
}
void createButtonWireframe(){
	buttons[18]->setImage("../resource/textures/gui/iconwireframe.png");
	buttons[18]->func = &clickButtonWireframe;
	buttons[18]->arg = NULL;
}
void clickButtonTextured(void *arg){
	//RM->renderflags = RENDER_OVERRIDE | RENDER_FACES | RENDER_TEXTURES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_TEXTURE;
	cur_renderflags = RENDER_FACES | RENDER_TEXTURES | COLOR_DEFAULT | LIGHT_NONE | TRANSPARENCY_TEXTURE;
	cur_alpha = 255;
	updateRender();
	printf("rendering as textured\n");
}
void createButtonTextured(){
	buttons[19]->setImage("../resource/textures/gui/icontextured.png");
	buttons[19]->func = &clickButtonTextured;
	buttons[19]->arg = NULL;
}

model *genCube(double l, double w, double h){
	model *M = new model;
	//	
	//	  E-----H
	//	 /|    /|
	//	F-+---G |
	//	| |   | |
	//	| A---+-D
	//	|/    |/
	//	B-----C
	//	
	//	AB = +y
	//	AD = +x
	//	AE = +z
	//vec A = {-l/2,-w/2,-h/2}; //A = A*10;
	//vec B = {-l/2,w/2,-h/2}; //B = B*10;
	//vec C = {l/2,w/2,-h/2}; //C = C*10;
	//vec D = {l/2,-w/2,-h/2}; //D = D*10;
	//vec E = {-l/2,-w/2,h/2}; //E = E*10;
	//vec F = {-l/2,w/2,h/2}; //F = F*10;
	//vec G = {l/2,w/2,h/2};// G = G*10;
	//vec H = {l/2,-w/2,h/2}; //H = H*10;
	int A = 0;
	int B = 1;
	int C = 2;
	int D = 3;
	int E = 4;
	int F = 5;
	int G = 6;
	int H = 7;
	M->vertices.push_back({-l/2,-w/2,-h/2});
	M->vertices.push_back({-l/2,w/2,-h/2});
	M->vertices.push_back({l/2,w/2,-h/2});
	M->vertices.push_back({l/2,-w/2,-h/2});
	M->vertices.push_back({-l/2,-w/2,h/2});
	M->vertices.push_back({-l/2,w/2,h/2});
	M->vertices.push_back({l/2,w/2,h/2});
	M->vertices.push_back({l/2,-w/2,h/2});
	M->mesh.push_back({A,E,H});
	M->mesh.push_back({H,D,A});
	M->mesh.push_back({D,H,G});
	M->mesh.push_back({G,C,D});
	M->mesh.push_back({C,G,F});
	M->mesh.push_back({F,B,C});
	M->mesh.push_back({B,F,E});
	M->mesh.push_back({E,A,B});
	M->mesh.push_back({E,F,G});
	M->mesh.push_back({G,H,E});
	M->mesh.push_back({B,A,D});
	M->mesh.push_back({D,C,B});
	return M;
}

void clickButtonBox(void *arg){
	model *cube = genCube(1,1,1);
	M->add(cube);
	updateRender();
	printf("new cube at 0,0,0\n");
}
void createButtonBox(){
	buttons[20]->setImage("../resource/textures/gui/iconbox.png");
	buttons[20]->func = &clickButtonBox;
	buttons[20]->arg = NULL;
}
void createButtonCSGAdd(){
	buttons[21]->setImage("../resource/textures/gui/iconcsgadd.png");
}

model *genCyllinder(double height, double diameter, int numsides){
	model *M = new model;
	double radius = diameter/2;
	//	 cylinder wedge
	//	  T
	//	 /|\   
	//	B-+-D next B = prev D
	//	| | | 
	//	| O |
	//	|/ \|
	//	A---C
	//	
	//	AB = +y
	//	AD = +x
	//	AE = +z
	//	 
	vec vO = {0,0,-height/2};		int O = 0;
	vec vT = {0,0,height/2};		int T = 1;
	M->vertices.push_back(vO);
	M->vertices.push_back(vT);
	vec vA = {0,-radius,-height/2};	int A = 2;
	vec vB = {0,-radius,height/2};	int B = 3;
	M->vertices.push_back(vA);
	M->vertices.push_back(vB);
	quat r = quat::fromAngleAxis(360.0f/numsides,0,0,1);
	vec vC = r.rotateVector(vA);	int C = 4;
	vec vD = r.rotateVector(vB);	int D = 5;
	M->vertices.push_back(vC);
	M->vertices.push_back(vD);
	M->mesh.push_back({T,D,B});
	M->mesh.push_back({D,A,B});
	M->mesh.push_back({A,D,C});
	M->mesh.push_back({O,A,C});
	for(int i = 2; i<=numsides;i++){
		A += 2;
		B += 2;
		C += 2;
		D += 2;
		vC = r.rotateVector(vC); //accumulating error here if we're not making
		vD = r.rotateVector(vD); //new quats for every side
		M->vertices.push_back(vC);
		M->vertices.push_back(vD);
		M->mesh.push_back({T,D,B});
		M->mesh.push_back({D,A,B});
		M->mesh.push_back({A,D,C});
		M->mesh.push_back({O,A,C});
	}
	return M;
}

void clickButtonCyllinder(void *arg){
	model *cyllinder = genCyllinder(1,1,12);
	M->add(cyllinder);
	updateRender();
	printf("new cyllinder at 0,0,0\n");
}
void createButtonCyllinder(){
	buttons[22]->setImage("../resource/textures/gui/iconcyllinder.png");
	buttons[22]->func = &clickButtonCyllinder;
	buttons[22]->arg = NULL;
}
void createButtonCSGSubtract(){
	buttons[23]->setImage("../resource/textures/gui/iconcsgsubtract.png");
}

model *genSphere(double diameter, int numverti, int numhori){
	model *M = new model;
	double radius = diameter/2;
	//	cylinder wedge
	//	  T
	//	 /|\   
	//	B-+-D next B = prev D
	//	| | | 
	//	| O |
	//	|/ \|
	//	A---C
	//	
	//	AB = +y
	//	AD = +x
	//	AE = +z 
	vec vO = {0,0,-radius};		int O = 0;
	vec vT = {0,0,radius};		int T = 1;
	M->vertices.push_back(vO);
	M->vertices.push_back(vT);
	vec vA; int A = 2;
	vec vB; int B = 3;
	vec vC; int C = 4;
	vec vD; int D = 5;
	quat v1;
	quat v2;
	quat h1;
	quat h2;
	//make all the vertices
	for(int i = 0; i < numhori; i++){
		h1 = quat::fromAngleAxis(i*(360.0f/numhori),0,0,1);
		//h2 = quat::fromAngleAxis((i+1)*(360.0f/numhori),0,0,1);
		//make a vertical column of vertices
		for(int t = 1; t < numverti; t++){
			v1 = quat::fromAngleAxis(t*(180.0f/numverti),1,0,0);
			//v2 = quat::fromAngleAxis((t+1)*(180.0f/numverti),1,0,0);
			vA = h1.rotateVector(v1.rotateVector(vO));
			//vB = h2.rotateVector(v1.rotateVector(vO));
			//vC = h1.rotateVector(v2.rotateVector(vO));
			//vD = h2.rotateVector(v2.rotateVector(vO));
			M->vertices.push_back(vA); 
			//M->vertices.push_back(vB);
			//M->vertices.push_back(vC);
			//M->vertices.push_back(vD); 
			//M->mesh.push_back({C,B,A});
			//M->mesh.push_back({B,C,D});
			A += 4;
			B += 4;
			C += 4;
			D += 4;
		}
	}
	//sew the vertices of every slice but the last together
	for(int i = 0; i < numhori-1; i++){
		int hor = (numverti-1)*i;
		for(int t = 0; t < numverti-2; t++){
			A = 2+t+hor;
			B = A+1;
			C = 2+t+numverti-1+hor;
			D = C+1;
			//M->mesh.push_back({B,O,D});
			M->mesh.push_back({B,C,A});
			M->mesh.push_back({C,B,D});
		}
	}
	//sew the vertices of the last slice together
	for(int t = 0; t < numverti-2; t++){
		A = 2+t+(numverti-1)*(numhori-1);
		B = A+1;
		C = 2+t;
		D = C+1;
		M->mesh.push_back({B,C,A});
		M->mesh.push_back({C,B,D});
	}
	//sew the bottom cap
	for(int i = 0; i < numhori-1; i++){
		A = 2+i*(numverti-1);
		B = 2+(i+1)*(numverti-1);
		M->mesh.push_back({O,A,B});
	}
	A = B;
	B = 2;
	M->mesh.push_back({O,A,B});
	//sew the top cap
	for(int i = 0; i < numhori-1; i++){
		A = 2+i*(numverti-1)+numverti-2;
		B = 2+(i+1)*(numverti-1)+numverti-2;
		M->mesh.push_back({T,B,A});
	}
	A = B;
	B = 2+numverti-2;
	M->mesh.push_back({T,B,A});
	// quat v = quat::fromAngleAxis(180.0f/numverti,1,0,0);
	// quat h = quat::fromAngleAxis(360.0f/numhori,0,0,1);
	
	// vec vA = v.rotateVector(vO);				int A = 2;
	// vec vB = h.rotateVector(vA);				int B = 3;
	// M->vertices.push_back(vA);
	// M->vertices.push_back(vB);
	// M->mesh.push_back({A,B,O});
	// vec vC = vA; int C = A;
	// vec vD = vB; int D = B;
	// for(int t = 2; t<numverti; t++){
		// vC = v.rotateVector(vC); C = A+2;
		// vD = h.rotateVector(vC); D = B+2;
		// M->vertices.push_back(vC);
		// M->vertices.push_back(vD);
		// M->mesh.push_back({C,B,A});
		// M->mesh.push_back({B,C,D});
		// A = C;
		// B = D;
	// }
	// M->mesh.push_back({D,C,T});
	return M;
}
void clickButtonSphere(void *arg){
	model *sphere = genSphere(1,12,24);
	M->add(sphere);
	updateRender();
	printf("new sphere at 0,0,0\n");
}
void createButtonSphere(){
	buttons[24]->setImage("../resource/textures/gui/iconsphere.png");
	buttons[24]->func = &clickButtonSphere;
	buttons[24]->arg = NULL;
}
void createButtonCSGIntersect(){
	buttons[25]->setImage("../resource/textures/gui/iconcsgintersect.png");
}

model *genCone(double height, double diameter, double numsides){
	model *M = new model;
	double radius = diameter/2;
	//	 cylinder wedge
	//	  T
	//	 /|\   
	//	B-+-D next B = prev D
	//	| | | 
	//	| O |
	//	|/ \|
	//	A---C
	//	
	//	AB = +y
	//	AD = +x
	//	AE = +z
	//	 
	vec vO = {0,0,-height/2};		int O = 0;
	vec vT = {0,0,height/2};		int T = 1;
	M->vertices.push_back(vO);
	M->vertices.push_back(vT);
	vec vA = {0,-radius,-height/2};	int A = 2;
	M->vertices.push_back(vA);
	quat r = quat::fromAngleAxis(360.0f/numsides,0,0,1);
	vec vC = r.rotateVector(vA);	int C = 3;
	M->vertices.push_back(vC);
	M->mesh.push_back({T,C,A});
	M->mesh.push_back({O,A,C});
	for(int i = 2; i<=numsides;i++){
		A++;
		C++;
		vC = r.rotateVector(vC); //accumulating error here if we're not making
								 //new quats for every side
		M->vertices.push_back(vC);
		M->mesh.push_back({T,C,A});
		M->mesh.push_back({O,A,C});
	}
	return M;
}

void clickButtonCone(void *arg){
	model *cone = genCone(1,1,12);
	M->add(cone);
	updateRender();
	printf("new cone at 0,0,0\n");
}
void createButtonCone(){
	buttons[26]->setImage("../resource/textures/gui/iconcone.png");
	buttons[26]->func = &clickButtonCone;
	buttons[26]->arg = NULL;
}
void createButtonCSGInvert(){
	buttons[27]->setImage("../resource/textures/gui/iconcsginvert.png");
}
void clickButtonVertex(void *arg){
	vec V = camera.pos;//vector_cursor_pos;
	M->vertices.push_back(V);
	printf("new vertex at [%f;%f;%f]\n",V.x,V.y,V.z);
}
void createButtonVertex(){
	buttons[28]->setImage("../resource/textures/gui/iconvertex.png");
	buttons[28]->func = &clickButtonVertex;
	buttons[28]->arg = NULL;
}
void createButtonSplit(){
	buttons[29]->setImage("../resource/textures/gui/iconsplit.png");
}
void createButtonEdge(){
	buttons[30]->setImage("../resource/textures/gui/iconedge.png");
}
void createButtonMerge(){
	buttons[31]->setImage("../resource/textures/gui/iconmerge.png");
}
void createButtonFace(){
	buttons[32]->setImage("../resource/textures/gui/iconface.png");
}
void createButtonCollapse(){
	buttons[33]->setImage("../resource/textures/gui/iconcollapse.png");
}

void OpenWindowModeller(){
	frame = new GUI2frame;
	frame->setPos(64,64);
	frame->setSize(64+4,512);
	frame->title = "Modeller";
	frame->setParent(GUI);
	
	for(int I = 0; I<17; I++){
		GUI2button *btn = new GUI2button;
		btn->setPos(0,0+32*I);
		btn->setSize(32,32);
		btn->func = NULL;
		btn->arg = NULL;
		btn->setParent((GUI2base*) frame);
		buttons.push_back(btn);
		
		GUI2button *btn2 = new GUI2button;
		btn2->setPos(32+0,0+32*I);
		btn2->setSize(32,32);
		btn2->func = NULL;
		btn2->arg = NULL;
		btn2->setParent((GUI2base*) frame);
		buttons.push_back(btn2);
	}
	createButtonNew();
	createButtonMove();
	createButtonOpen();
	createButtonScale();
	createButtonSave();
	createButtonRotate();
	createButtonSaveAs();
	createButtonCopy();
	createButtonOptions();
	createButtonDelete();
	createButtonUndo();
	createButtonRedo();
	createButtonGrid();
	createButtonExtrude();
	createButtonPreview();
	createButtonSweep();
	createButtonTransparent();
	createButtonMatte();
	createButtonWireframe();
	createButtonTextured();
	createButtonBox();
	createButtonCSGAdd();
	createButtonCyllinder();
	createButtonCSGSubtract();
	createButtonSphere();
	createButtonCSGIntersect();
	createButtonCone();
	createButtonCSGInvert();
	createButtonVertex();
	createButtonSplit();
	createButtonEdge();
	createButtonMerge();
	createButtonFace();
	createButtonCollapse();
}

bool intersection_closest(pair<int, float> A, pair<int, float> B){
	return (A.second > B.second);
}

void ModellerInput::receiveMessage(message *msg){
	if(msg->type == "lmb_up"){
		printf("modeller:click!\n");
		vec P = camera.pos;
		printf("camera pos: %f, %f, %f\n", P.x, P.y, P.z);
		vec Pf = camera.angle.forward();
		printf("camera fwd: %f, %f, %f\n", Pf.x, Pf.y, Pf.z);
		vec2i M = input.getMousePos();
		printf("mouse pos: %d, %d\n", M.x, M.y);
		vec Mw = camera.screentoworld_simple(M);
		printf("m to world: %f, %f, %f\n", Mw.x, Mw.y, Mw.z);
		
		int found = 0;
		vector<pair<int, float>> intersections;
		for(int I = 0; I < RM->triangles.size(); I++){
			rtriangle &rtri = RM->triangles[I];
			vec result;
			if(ray_triangle_intersection(camera.pos, camera.cursorDir(), rtri.A.pos, rtri.B.pos, rtri.C.pos, result)){
				float dist = (result-camera.pos).length();
				intersections.push_back({I,dist});
				found = 1;
			}
		}
		if(found){
			std::sort(intersections.begin(), intersections.end(), &intersection_closest);
			selected.push_back(intersections.back().first);
		}else{selected.clear();}
		//green - camera to result
		// line *L = new line(camera.pos, result, -1);
		// L->color = {0,255,0,255};
		// L->setrenderflags(RENDER_DEFAULT | RENDER_VERTICES | COLOR_UNIFORM | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT, 1);
		// scene.push_back(L);
		//red - camera to direction
		line *L2 = new line(camera.pos, camera.pos+camera.cursorDir()*3, 400);
		L2->color = {255,0,0,255};
		L2->setrenderflags(RENDER_DEFAULT | RENDER_VERTICES | COLOR_UNIFORM | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT, 1);
		scene.push_back(L2);
		//blue - origin to result
		//line *L3 = new line({0,0,0}, result, -1);
		//L3->color = {0,0,255,255};
		//L3->setrenderflags(RENDER_DEFAULT | RENDER_VERTICES | COLOR_UNIFORM | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT, 1);
		//scene.push_back(L3);
		//blue - camera forward
		line *L4 = new line(camera.pos, camera.pos+camera.angle.forward()*3, 400);
		L4->color = {0,0,255,255};
		L4->setrenderflags(RENDER_DEFAULT | RENDER_VERTICES | COLOR_UNIFORM | LIGHT_DEFAULT | TRANSPARENCY_DEFAULT, 1);
		scene.push_back(L4);
		updateRender();
		printf("%d triangles selected\n", selected.size());
	}
}
ModellerInput MI;
void startModellerSession(){
	OpenWindowModeller();
	MI.subscribeToMessageChannel(&input.channel, "");
	//input.channel.subscribe("", &MI);
	clickButtonNew(NULL);
}

/* this is from toolbox
	GUI2frame* toolFrame = new GUI2frame;
	toolFrame->setPos(64,64);
	toolFrame->setSize(64+8,256);
	toolFrame->title = "Toolbox";
	toolFrame->setParent(GUI);
	
	GUI2button* btnBox = new GUI2button;						GUI2button* btnCyl = new GUI2button;
	btnBox->setPos(4,4);										btnCyl->setPos(4+32,4);
	btnBox->setSize(32,32);										btnCyl->setSize(32,32);
	btnBox->setImage("C:/Stride/textures/cube.bmp");			btnCyl->setImage("C:/Stride/textures/cylinder.bmp");
	btnBox->func = &genBox;										btnCyl->func = &genCyl;
	btnBox->arg = NULL;											btnCyl->arg = NULL;
	btnBox->setParent((GUI2base*) toolFrame);					btnCyl->setParent((GUI2base*) toolFrame);
	*/