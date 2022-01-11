//The little window with random tools!
#include "toolbox.h"
#include "../gui/Gui2.h"
#include "../main/control.h"
#include "../gui/console.h"
#include "../resource/textureloader.h"
#include "../resource/models.h"
#include "../game/physics.h"
void genBox(void* arg);
void genCyl(void* arg);
void genCon(void* arg);
void genSph(void* arg);
void windowOpts(void* arg);
void windowPhysbody(void* arg);


void OpenMenuToolbox()
{
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
	
	GUI2button* btnSphere = new GUI2button;						GUI2button* btnCone = new GUI2button;
	btnSphere->setPos(4,4+32);									btnCone->setPos(4+32,4+32);
	btnSphere->setSize(32,32);									btnCone->setSize(32,32);
	btnSphere->setImage("C:/Stride/textures/sphere.bmp");		btnCone->setImage("C:/Stride/textures/cone.bmp");
	btnSphere->func = &genSph;									btnCone->func = &genCon;
	btnSphere->arg = NULL;										btnCone->arg = NULL;
	btnSphere->setParent((GUI2base*) toolFrame);				btnCone->setParent((GUI2base*) toolFrame);
	
	GUI2button* btnGoggles = new GUI2button;					GUI2button* btnArrows = new GUI2button;
	btnGoggles->setPos(4,4+96);									btnArrows->setPos(4+32,4+96);
	btnGoggles->setSize(32,32);									btnArrows->setSize(32,32);
	btnGoggles->setImage("C:/Stride/textures/goggles.bmp");		btnArrows->setImage("C:/Stride/textures/fourarrows.bmp");
	btnGoggles->func = &windowOpts;								btnArrows->func = &windowPhysbody;
	btnGoggles->arg = NULL;										btnArrows->arg = NULL;
	btnGoggles->setParent((GUI2base*) toolFrame);				btnArrows->setParent((GUI2base*) toolFrame);
	/*
	GUIImage* image1 = new GUIImage;
	image1->setPos(32,32);
	image1->setSize(32,32);
	image1->setImage("C:/Stride/textures/grass3.bmp");//"C:/Stride/grass3.raw");
	image1->setParent((GUIbase*)designerFrame);
	*/
	ParseCommand("textureload C:/Stride/textures/crate32.bmp");
	ParseCommand("textureload C:/Stride/textures/crate3test.bmp");
	ParseCommand("textureload C:/Stride/textures/grass3.bmp");
	ParseCommand("textureload C:/Stride/textures/barrel-top.bmp");
	ParseCommand("textureload C:/Stride/textures/barrel-side.bmp");
	ParseCommand("textureload C:/Stride/textures/sinus-wide.bmp");
	texture_load_pixelated = true;
	ParseCommand("textureload C:/Stride/textures/windrose.bmp");
	texture_load_pixelated = false;
}

model *genCube(double l, double w, double h)
{
	//printf("<1>");
	/*
	
	*/
	vec V = {l,w,h};
	model* M = new model;
	//printf("<2>");
	//((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new vtriangle[12];
	//printf("<3>");
	M->texmap = new textriangle[12];
	M->numtextures = 2;
	//printf("<4>");
	M->textures = new texture[2];
	M->textures[0] = textureGet("C:/Stride/textures/crate32.bmp");
	M->textures[1] = textureGet("C:/Stride/textures/grass3.bmp");
	//printf("<5>");
	
	/*
	  E-----H
	 /|    /|
	F-+---G |
	| |   | |
	| A---+-D
	|/    |/
	B-----C
	
	AB = +y
	AD = +x
	AE = +z
	*/ 
	
	
	vec A = {-l/2,-w/2,-h/2}; //A = A*10;
	vec B = {-l/2,w/2,-h/2}; //B = B*10;
	vec C = {l/2,w/2,-h/2}; //C = C*10;
	vec D = {l/2,-w/2,-h/2}; //D = D*10;
	vec E = {-l/2,-w/2,h/2}; //E = E*10;
	vec F = {-l/2,w/2,h/2}; //F = F*10;
	vec G = {l/2,w/2,h/2};// G = G*10;
	vec H = {l/2,-w/2,h/2}; //H = H*10;
	//texture:
	//A - B
	//|   |
	//C - D
	vec2f tA = {0,1};
	vec2f tB = {1,1};
	vec2f tC = {0,0};
	vec2f tD = {1,0};
	//CW culling.
	//
	//front
	M->mesh[0] = (vtriangle){A,E,H}; M->texmap[0] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[1] = (vtriangle){H,D,A}; M->texmap[1] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//right
	M->mesh[2] = (vtriangle){D,H,G}; M->texmap[2] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[3] = (vtriangle){G,C,D}; M->texmap[3] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//back
	M->mesh[4] = (vtriangle){C,G,F}; M->texmap[4] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[5] = (vtriangle){F,B,C}; M->texmap[5] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//left
	M->mesh[6] = (vtriangle){B,F,E}; M->texmap[6] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[7] = (vtriangle){E,A,B}; M->texmap[7] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//up
	
	M->mesh[8] = (vtriangle){E,F,G}; M->texmap[8] = (textriangle){{tC,tA,tB},1,R_TEXTURE};
	M->mesh[9] = (vtriangle){G,H,E}; M->texmap[9] = (textriangle){{tB,tD,tC},1,R_TEXTURE};
	//down
	M->mesh[10] = (vtriangle){B,A,D}; M->texmap[10] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[11] = (vtriangle){D,C,B}; M->texmap[11] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	
	//
	M->numtris = 12;//12;
	
	//printf("<6>");
	//myModel = M;
	return M;
	//AllPhysBodies.push_back(physBody(M));
	//printf("<7>");
	//OpenMenuModel();
}
model *genPlane(double l, double w)
{
	model* M = new model;
	M->mesh = new vtriangle[2];
	M->texmap = new textriangle[2];
	M->numtextures = 1;
	M->textures = new texture;
	M->textures[0] = textureGet("C:/Stride/textures/crate32.bmp");
	vec A = {-l/2,-w/2,	0}; //A = A*10;
	vec B = {-l/2,w/2,0}; //B = B*10;
	vec C = {l/2,w/2,0}; //C = C*10;
	vec D = {l/2,-w/2,0}; //D = D*10;
	vec2f tA = {1,0};
	vec2f tB = {0,0};
	vec2f tC = {0,1};
	vec2f tD = {1,1};
	M->mesh[0] = (vtriangle){A,B,C}; M->texmap[0] = (textriangle){{tA,tB,tC},0,R_TEXTURE};
	M->mesh[1] = (vtriangle){C,D,A}; M->texmap[1] = (textriangle){{tC,tD,tA},0,R_TEXTURE};
	M->numtris = 2;
	return M;
}
model *genCylinder(double height, double radius, int numsides)
{
	printf("<1>");
	/*
	double height = 	((GUIspinner*)(((void**)arg)[0]))->vals[1];
	double radius = 	((GUIspinner*)(((void**)arg)[1]))->vals[1];
	int numsides = 	(int)(((GUIspinner*)(((void**)arg)[2]))->vals[1]);
	*/
	//vec V = {h,r,n};
	model* M = new model;
	printf("<2>");
	//((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new vtriangle[4*numsides];
	printf("<3>");
	M->texmap = new textriangle[4*numsides];
	M->numtextures = 2;
	printf("<4>");
	M->textures = new texture[2];
	M->textures[0] = textureGet("C:/Stride/textures/barrel-top.bmp");
	M->textures[1] = textureGet("C:/Stride/textures/barrel-side.bmp");
	printf("<5>");
	
	/* cylinder wedge
	  T
	 /|\   
	B-+-D next B = prev D
	| | | 
	| O |
	|/ \|
	A---C
	
	AB = +y
	AD = +x
	AE = +z
	*/ 
	//texture:
	//A - B
	//| O |
	//C-L-D
	
	vec2f tA = {0,1};
	vec2f tB = {1,1};
	vec2f tC = {0,0};
	vec2f tD = {1,0};
	vec2f tO = {0.5f,0.5f};
	vec2f tL = {0.5f,0};
	vec2f tLt1 = tL; //top-circling vector
	vec2f tLt2 = tL;
	vec2f tLs1 = tC; //side-circling vector (bottom, first) |    1      1
	vec2f tLs2 = tC; // (bottom, second)                    | -1   1 ->
	vec2f tUs1 = tA; // (upper, first)                      |   -1      0   1
	vec2f tUs2 = tA;
	
	vec O = {0,0,0};
	vec T = {0,0,height};
	
	vec A = {0,-radius,0};
	vec B = {0,-radius,height};
	
	quat r = quat::fromAngleAxis(360.0f/numsides,0,0,1);
	vec C = r.rotateVector({0,-radius,0});
	vec D = r.rotateVector({0,-radius,height});
	
	tLt2.x = (C.x/radius+1)/2; tLt2.y = (C.y/radius+1)/2;
	tLs2.x = tUs2.x= 1.0f/numsides;
	M->mesh[0] = (vtriangle){T,D,B}; M->texmap[0] = (textriangle){{tO,tLt2,tLt1},0,R_TEXTURE};
	M->mesh[1] = (vtriangle){D,A,B}; M->texmap[1] = (textriangle){{tUs2,tLs1,tUs1},1,R_TEXTURE};
	M->mesh[2] = (vtriangle){A,D,C}; M->texmap[2] = (textriangle){{tLs1,tUs2,tLs2},1,R_TEXTURE};
	M->mesh[3] = (vtriangle){O,A,C}; M->texmap[3] = (textriangle){{tO,tLt1,tLt2},0,R_TEXTURE};
	
	for(int i = 2;i<=numsides;i++)
	{
		r = quat::fromAngleAxis(i*360.0f/numsides,0,0,1);
		A = C;
		B = D;
		C = r.rotateVector({0,-radius,0});
		D = r.rotateVector({0,-radius,height});
	
		tLt1 = tLt2;tLs1 = tLs2;tUs1 = tUs2;
		tLt2.x = (C.x/radius+1)/2;tLt2.y = (C.y/radius+1)/2; 
		tLs2.x = tUs2.x= ((double)i)/numsides;
		
		M->mesh[4*i-4] = (vtriangle){T,D,B}; M->texmap[4*i-4] = (textriangle){{tO,tLt2,tLt1},0,R_TEXTURE};
		M->mesh[4*i-3] = (vtriangle){D,A,B}; M->texmap[4*i-3] = (textriangle){{tUs2,tLs1,tUs1},1,R_TEXTURE};
		M->mesh[4*i-2] = (vtriangle){A,D,C}; M->texmap[4*i-2] = (textriangle){{tLs1,tUs2,tLs2},1,R_TEXTURE};
		M->mesh[4*i-1] = (vtriangle){O,A,C}; M->texmap[4*i-1] = (textriangle){{tO,tLt1,tLt2},0,R_TEXTURE};
	}
	
	
	
	
	//CW culling.
	//
	/*
	//front
	M->mesh[0] = (triangle){A,E,H}; M->texmap[0] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[1] = (triangle){H,D,A}; M->texmap[1] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	*/
	M->numtris = 4*numsides;//12;
	
	printf("<6>");
	//myModel = M;
	return M;
	//AllPhysBodies.push_back(physBody(M));
	printf("<7>");
	//OpenMenuModel();
}

model *genCone(double height, double radius, int numsides)
{
	printf("<1>");
	/*
	double height = 	((GUIspinner*)(((void**)arg)[0]))->vals[1];
	double radius = 	((GUIspinner*)(((void**)arg)[1]))->vals[1];
	int numsides = 	(int)(((GUIspinner*)(((void**)arg)[2]))->vals[1]);
	*/
	//vec V = {h,r,n};
	model* M = new model;
	printf("<2>");
	//((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new vtriangle[2*numsides];
	printf("<3>");
	M->texmap = new textriangle[2*numsides];
	M->numtextures = 1;
	printf("<4>");
	M->textures = new texture;
	M->textures[0] = textureGet("C:/Stride/textures/barrel-top.bmp");
	//M->textures[1] = textureGet("C:/Stride/textures/grass3.bmp");
	printf("<5>");
	
	/* cylinder wedge
	  T
	 /|\   
	B-+-D next B = prev D
	| | | 
	| O |
	|/ \|
	A---C
	
	AB = +y
	AD = +x
	AE = +z
	*/ 
	//texture:
	//A - B
	//| O |
	//C-L-D
	
	vec2f tA = {0,1};
	vec2f tB = {1,1};
	vec2f tC = {0,0};
	vec2f tD = {1,0};
	vec2f tO = {0.5f,0.5f};
	vec2f tL = {0.5f,0};
	vec2f tLt1 = tL; //top-circling vector
	vec2f tLt2 = tL;
	vec2f tLs1 = tC; //side-circling vector (bottom, first) |    1      1
	vec2f tLs2 = tC; // (bottom, second)                    | -1   1 ->
	vec2f tUs1 = tA; // (upper, first)                      |   -1      0   1
	vec2f tUs2 = tA;
	
	vec O = {0,0,0};
	vec T = {0,0,height};
	
	vec A = {0,-radius,0};
	quat r = quat::fromAngleAxis(360.0f/numsides,0,0,1);
	vec C = r.rotateVector({0,-radius,0});
	
	tLt2.x = (C.x/radius+1)/2; tLt2.y = (C.y/radius+1)/2;
	M->mesh[0] = (vtriangle){T,C,A}; M->texmap[0] = (textriangle){{tO,tLt2,tLt1},0,R_TEXTURE};
	M->mesh[1] = (vtriangle){O,A,C}; M->texmap[1] = (textriangle){{tO,tLt1,tLt2},0,R_TEXTURE};
	
	for(int i = 2;i<=numsides;i++)
	{
		r = quat::fromAngleAxis(i*360.0f/numsides,0,0,1);
		A = C;
		C = r.rotateVector({0,-radius,0});
		
		tLt1 = tLt2;
		tLt2.x = (C.x/radius+1)/2;tLt2.y = (C.y/radius+1)/2; 
		
		M->mesh[2*i-2] = (vtriangle){T,C,A}; M->texmap[2*i-2] = (textriangle){{tO,tLt2,tLt1},0,R_TEXTURE};
		M->mesh[2*i-1] = (vtriangle){O,A,C}; M->texmap[2*i-1] = (textriangle){{tO,tLt1,tLt2},0,R_TEXTURE};
	}
	
	
	
	
	//CW culling.
	//
	/*
	//front
	M->mesh[0] = (triangle){A,E,H}; M->texmap[0] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[1] = (triangle){H,D,A}; M->texmap[1] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	*/
	M->numtris = 2*numsides;//12;
	
	printf("<6>");
	//myModel = M;
	return M;
	//AllPhysBodies.push_back(physBody(M));
	printf("<7>");
	//OpenMenuModel();
}

model *genSphere(double radius, int numverti, int numhori)
{
	printf("<1>");
	/*
	double radius = 	((GUIspinner*)(((void**)arg)[0]))->vals[1];
	int numverti = 	(int)(((GUIspinner*)(((void**)arg)[1]))->vals[1]);
	int numhori = 	(int)(((GUIspinner*)(((void**)arg)[2]))->vals[1]);
	*/
	//vec V = {h,r,n};
	model* M = new model;
	printf("<2>");
	//((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new vtriangle[2*numverti*numhori];
	printf("<3>");
	M->texmap = new textriangle[2*numverti*numhori];
	M->numtextures = 1;
	printf("<4>");
	M->textures = new texture;
	M->textures[0] = textureGet("C:/Stride/textures/sinus-wide.bmp");
	//M->textures[1] = textureGet("C:/Stride/textures/grass3.bmp");
	printf("<5>");
	
	/* cylinder wedge
	  T
	 /|\   
	B-+-D next B = prev D
	| | | 
	| O |
	|/ \|
	A---C
	
	AB = +y
	AD = +x
	AE = +z
	*/ 
	//texture:
	//A - B
	//| O |
	//C-L-D
	
	// Cylindrical projection <<--
	// sinusoidal projection
	vec O = {0,0,-radius};
	vec T = {0,0,radius};
	vec B,D,A,C;
	vec2f tU1,tU2,tL1,tL2;
	
	for(int i = 0;i<numhori;i++)
	{
		quat hori = quat::fromAngleAxis(i*360.0f/numhori,0,0,-1);
		quat hori2 = quat::fromAngleAxis((i+1)*360.0f/numhori,0,0,-1);
		for(int t = 0;t<numverti;t++)
		{
			quat verti = quat::fromAngleAxis(t*180.0f/numverti,-1,0,0);
			quat verti2 = quat::fromAngleAxis((t+1)*180.0f/numverti,-1,0,0);
			B = hori.corotateVector(verti.rotateVector(T));
			A = hori.corotateVector(verti2.rotateVector(T));
			D = hori2.corotateVector(verti.rotateVector(T));
			C = hori2.corotateVector(verti2.rotateVector(T));
			/*
			tU1.x = ((double)i)/numhori;tU1.y = 1-((double)t)/numverti;
			tU2.x = ((double)i+1)/numhori;tU2.y = 1-((double)t)/numverti;
			tL1.x = ((double)i)/numhori;tL1.y = 1-((double)t+1)/numverti;
			tL2.x = ((double)i+1)/numhori;tL2.y = 1-((double)t+1)/numverti;
			*/
			tU1.y = 1-((double)t)/numverti; tU1.x = 0.5f+(((double)i)/numhori-0.5f)*cos(M_PI*(tU1.y-0.5f));
			tU2.y = 1-((double)t)/numverti; tU2.x = 0.5f+(((double)i+1)/numhori-0.5f)*cos(M_PI*(tU2.y-0.5f));
			tL1.y = 1-((double)t+1)/numverti; tL1.x = 0.5f+(((double)i)/numhori-0.5f)*cos(M_PI*(tL1.y-0.5f));
			tL2.y = 1-((double)t+1)/numverti; tL2.x = 0.5f+(((double)i+1)/numhori-0.5f)*cos(M_PI*(tL2.y-0.5f));
			
			M->mesh[(i*numverti+t)*2] = (vtriangle){A,B,D}; M->texmap[(i*numverti+t)*2] = (textriangle){{tL1,tU1,tU2},0,R_TEXTURE};
			M->mesh[(i*numverti+t)*2+1]=(vtriangle){D,C,A}; M->texmap[(i*numverti+t)*2+1] = (textriangle){{tU2,tL2,tL1},0,R_TEXTURE};
		}
	}
	
	
	
	
	
	//CW culling.
	//
	/*
	//front
	M->mesh[0] = (triangle){A,E,H}; M->texmap[0] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[1] = (triangle){H,D,A}; M->texmap[1] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	*/
	M->numtris = 2*numverti*numhori;//12;
	
	printf("<6>");
	//myModel = M;
	return M;
	//physBody Bod = physBody(M);
	//Bod.tracegroup = 0;
	//AllPhysBodies.push_back(Bod);
	printf("<7>");
	//OpenMenuModel();
}

void btnGenCube(void* arg)
{
	double l = 	((GUI2spinner*)(((void**)arg)[0]))->vals[1];
	double w = 	((GUI2spinner*)(((void**)arg)[1]))->vals[1];
	double h = 	((GUI2spinner*)(((void**)arg)[2]))->vals[1];
	physBody* bod = new physBody(genCube(l,w,h));
	//bod->mdl->applyRenderFlags(R_TEXTURE|R_LIGHTEN);
	//bod->mdl->blendmode = rand()%2+1;
	AllPhysBodies.push_back(bod);
}
void btnGenCyl(void* arg)
{
	double height = 	((GUI2spinner*)(((void**)arg)[0]))->vals[1];
	double radius = 	((GUI2spinner*)(((void**)arg)[1]))->vals[1];
	int numsides = 	(int)(((GUI2spinner*)(((void**)arg)[2]))->vals[1]);
	AllPhysBodies.push_back(new physBody(genCylinder(height,radius,numsides)));
}
void btnGenCone(void* arg)
{
	double height = 	((GUI2spinner*)(((void**)arg)[0]))->vals[1];
	double radius = 	((GUI2spinner*)(((void**)arg)[1]))->vals[1];
	int numsides = 	(int)(((GUI2spinner*)(((void**)arg)[2]))->vals[1]);
	AllPhysBodies.push_back(new physBody(genCone(height,radius,numsides)));
}
void btnGenSphere(void* arg)
{
	double radius = 	((GUI2spinner*)(((void**)arg)[0]))->vals[1];
	int numverti = 	(int)(((GUI2spinner*)(((void**)arg)[1]))->vals[1]);
	int numhori = 	(int)(((GUI2spinner*)(((void**)arg)[2]))->vals[1]);
	AllPhysBodies.push_back(new physBody(genSphere(radius,numverti,numhori)));
}
void genBox(void* arg)
{
	GUI2frame* GenMenu = new GUI2frame;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Box Generator";
	GenMenu->setParent(GUI);
	GUI2base::fixstrata((GUI2base*)GenMenu);
	
	GUI2spinner* spinx = new GUI2spinner;		GUI2label* spinxtext = new GUI2label;
	spinx->setPos(4,4);						spinxtext->setPos(96,4);
	spinx->setVals(-10,1,10,0.5,2);			spinxtext->text = "length";
	spinx->setParent((GUI2base*) GenMenu);	spinxtext->setParent((GUI2base*) GenMenu);

	GUI2spinner* spiny = new GUI2spinner;		GUI2label* spinytext = new GUI2label;
	spiny->setPos(4,4+18);					spinytext->setPos(96,4+16);
	spiny->setVals(-10,1,10,0.5,2);			spinytext->text = "width";
	spiny->setParent((GUI2base*) GenMenu);	spinytext->setParent((GUI2base*) GenMenu);
	
	GUI2spinner* spinz = new GUI2spinner;		GUI2label* spinztext = new GUI2label;
	spinz->setPos(4,4+32+4);				spinztext->setPos(96,4+32);
	spinz->setVals(-10,1,10,0.5,2);			spinztext->text = "height";
	spinz->setParent((GUI2base*) GenMenu);	spinztext->setParent((GUI2base*) GenMenu);
	
	void **newarg = new void*[3];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	GUI2button* btnGen = new GUI2button;
	btnGen->setPos(4,4+64);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &btnGenCube;
	btnGen->arg = (void*)newarg;
	btnGen->setParent((GUI2base*) GenMenu);
	
}



void genCyl(void* arg)
{
	GUI2frame* GenMenu = new GUI2frame;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Cylinder Generator";
	GenMenu->setParent(GUI);
	GUI2base::fixstrata((GUI2base*)GenMenu);
	
	GUI2spinner* spinx = new GUI2spinner;		GUI2label* spinxtext = new GUI2label;
	spinx->setPos(4,4);						spinxtext->setPos(96,4);
	spinx->setVals(-10,1,10,0.5,2);			spinxtext->text = "height";
	spinx->setParent((GUI2base*) GenMenu);	spinxtext->setParent((GUI2base*) GenMenu);

	GUI2spinner* spiny = new GUI2spinner;		GUI2label* spinytext = new GUI2label;
	spiny->setPos(4,4+18);					spinytext->setPos(96,4+16);
	spiny->setVals(-10,0.5,10,0.25,2);			spinytext->text = "radius";
	spiny->setParent((GUI2base*) GenMenu);	spinytext->setParent((GUI2base*) GenMenu);
	
	GUI2spinner* spinz = new GUI2spinner;		GUI2label* spinztext = new GUI2label;
	spinz->setPos(4,4+32+4);					spinztext->setPos(96,4+32);
	spinz->setVals(2,8,64,1,2);			spinztext->text = "num sides";
	spinz->setParent((GUI2base*) GenMenu);	spinztext->setParent((GUI2base*) GenMenu);
	
	void **newarg = new void*[3];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	GUI2button* btnGen = new GUI2button;
	btnGen->setPos(4,4+64);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &btnGenCyl;
	btnGen->arg = (void*)newarg;
	btnGen->setParent((GUI2base*) GenMenu);
	
}

void genCon(void* arg)
{
	GUI2frame* GenMenu = new GUI2frame;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Cone Generator";
	GenMenu->setParent(GUI);
	GUI2base::fixstrata((GUI2base*)GenMenu);
	
	GUI2spinner* spinx = new GUI2spinner;		GUI2label* spinxtext = new GUI2label;
	spinx->setPos(4,4);						spinxtext->setPos(96,4);
	spinx->setVals(-10,1,10,0.5,2);			spinxtext->text = "height";
	spinx->setParent((GUI2base*) GenMenu);	spinxtext->setParent((GUI2base*) GenMenu);

	GUI2spinner* spiny = new GUI2spinner;		GUI2label* spinytext = new GUI2label;
	spiny->setPos(4,4+18);					spinytext->setPos(96,4+16);
	spiny->setVals(-10,0.5,10,0.25,2);		spinytext->text = "radius";
	spiny->setParent((GUI2base*) GenMenu);	spinytext->setParent((GUI2base*) GenMenu);
	
	GUI2spinner* spinz = new GUI2spinner;		GUI2label* spinztext = new GUI2label;
	spinz->setPos(4,4+32+4);					spinztext->setPos(96,4+32);
	spinz->setVals(2,8,64,1,2);			spinztext->text = "num sides";
	spinz->setParent((GUI2base*) GenMenu);	spinztext->setParent((GUI2base*) GenMenu);
	
	void **newarg = new void*[3];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	GUI2button* btnGen = new GUI2button;
	btnGen->setPos(4,4+64);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &btnGenCone;
	btnGen->arg = (void*)newarg;
	btnGen->setParent((GUI2base*) GenMenu);
	
}

void genSph(void* arg)
{
	GUI2frame* GenMenu = new GUI2frame;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Sphere Generator";
	GenMenu->setParent(GUI);
	GUI2base::fixstrata((GUI2base*)GenMenu);
	
	GUI2spinner* spinx = new GUI2spinner;		GUI2label* spinxtext = new GUI2label;
	spinx->setPos(4,4);						spinxtext->setPos(96,4);
	spinx->setVals(-10,0.5,10,0.25,2);			spinxtext->text = "height";
	spinx->setParent((GUI2base*) GenMenu);	spinxtext->setParent((GUI2base*) GenMenu);

	GUI2spinner* spiny = new GUI2spinner;		GUI2label* spinytext = new GUI2label;
	spiny->setPos(4,4+18);					spinytext->setPos(96,4+16);
	spiny->setVals(0,8,10,0.5,2);			spinytext->text = "vertical sides";
	spiny->setParent((GUI2base*) GenMenu);	spinytext->setParent((GUI2base*) GenMenu);
	
	GUI2spinner* spinz = new GUI2spinner;		GUI2label* spinztext = new GUI2label;
	spinz->setPos(4,4+32+4);					spinztext->setPos(96,4+32);
	spinz->setVals(0,8,64,1,2);			spinztext->text = "horizontal sides";
	spinz->setParent((GUI2base*) GenMenu);	spinztext->setParent((GUI2base*) GenMenu);
	
	void **newarg = new void*[3];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	GUI2button* btnGen = new GUI2button;
	btnGen->setPos(4,4+64);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &btnGenSphere;
	btnGen->arg = (void*)newarg;
	btnGen->setParent((GUI2base*) GenMenu);
	
}

void toggleWireframe(void* arg)
{
	if(*(bool*)arg){ConsoleParse("yup wireframe");}
	else{ConsoleParse("Nope wireframe");}
	renderWireframe = *(bool*)arg;
}




physBody *prevSelected = 0;
physBody *EntSelected = 0;
bool wPBfuncSet = 0;
void entSelect(void *arg)
{
	if(EntLookAt){EntLookAt->ptrs.get(&EntSelected);}//EntSelected
	EntSelected = EntLookAt; //even if 0, though ptrs.get assigns too
	if(prevSelected){prevSelected->color = {255,255,255};}
	if(EntSelected){EntSelected->color = {64,255,64}; prevSelected = EntSelected;}
	
	trace ray;
	ray.start = SomeVec1;
	ray.dir = CamAngle.rotateVector({0,1,0});
	ray.scan();
	if(ray.hit)
	{
		physBody *tempPoint = new physBody(genSphere(0.01,8,16));
		tempPoint->tracegroup = 0;
		tempPoint->pos = ray.hitpos; 
		AllPhysBodies.push_back(tempPoint);
	}
	printf("beep boop");
}
void entPush(void *arg)
{
	if(EntLookAt){EntLookAt->applyForce(CamAngle.rotateVector({0,1,0}));}
}

void update3Dcursor(void* arg)
{
	physBody *cursor = (physBody *)arg;//     vvv cursor->pos-SomeVec1
	cursor->orient = quat::from4vecs({0,0,-1},CamAngle.rotateVector({0,1,0}),{1,0,0},CamAngle.rotateVector({1,0,0})); //or just = camAngle;
	if(EntSelected){cursor->pos = EntSelected->pos; cursor->scale = 3*EntSelected->BSradius*EntSelected->scale;}
	else{cursor->scale = 0;}
}

void toggle3Dcursor(void* arg)
{
	static physBody *cursor = 0;
	if(!cursor)
	{
		cursor = new physBody(genPlane(1,1));
		cursor->mdl->textures[0] = textureGet("C:/Stride/textures/windrose.bmp");
		//cursor->mdl->applyRenderFlags(R_TEXTURE|R_LIGHTEN);
		//cursor->mdl->blendmode = 1;
		cursor->onThink = &update3Dcursor;
		AllPhysBodies.push_back(cursor);
	}
	else
	{
		delete cursor;
		cursor = 0;
	}
}

void AABBupdate(void* arg)
{
	physBody* S = (physBody*)arg;
	physBody* B = S->parent;
	double x = B->AABBmax.x-B->AABBmin.x;
	double y = B->AABBmax.y-B->AABBmin.y;
	double z = B->AABBmax.z-B->AABBmin.z;
	S->pos = vec(B->AABBmin.x+x/2,B->AABBmin.y+y/2,B->AABBmin.z+z/2);
	//S->setPos({0,0,0});
	S->setOrient(quat::fromAngleAxis(0,0,0,1));
	S->setScale(1);
	delete S->mdl;
	S->mdl = genCube(x,y,z);
	S->mdl->applyRenderFlags(R_DARKEN|R_LIGHTEN);
	S->mdl->blendmode = 2;
	S->mdl->numtextures = 0;
}

void toggleAABB(void* arg)
{
	if(*(bool*)arg)
	{
		int k = AllPhysBodies.size();
		for(int i = 0;i<k;i++)
		{
			physBody *B = AllPhysBodies[i];
			if(B->tracegroup==1)
			{
				physBody *AABB = new physBody();
				AABB->setParent(B);
				AABB->color = {128,128,256};
				AABB->alpha = 128;
				AABB->tracegroup = -2;
				AABB->onThink = &AABBupdate;
				AllPhysBodies.push_back(AABB);
			}
		}
	}
	else
	{
		int i = 0;
		while(i < AllPhysBodies.size())
		{
			if(AllPhysBodies[i]->tracegroup==-2){delete AllPhysBodies[i];}else {i++;}
		}
	}
}

void windowOpts(void* arg)
{
	GUI2frame* Menu = new GUI2frame;
	Menu->setPos(128,128);
	Menu->setSize(128,128);
	Menu->title = "Display options";
	Menu->setParent(GUI);
	GUI2base::fixstrata((GUI2base*)Menu);

	GUI2checkbox* check = new GUI2checkbox;	GUI2label* label1 = new GUI2label;
	check->pos = {4,4};						label1->pos = {4+16,4};
	check->setParent((GUI2base*)Menu);		label1->text = "wireframe";
	check->func = &toggleWireframe;			label1->setParent((GUI2base*)Menu);
	
	GUI2checkbox* check2 = new GUI2checkbox;	GUI2label* label2 = new GUI2label;
	check2->pos = {4,4+16};					label2->pos = {4+16,4+16};
	check2->setParent((GUI2base*)Menu);		label2->text = "3D cursor";
	check2->func = &toggle3Dcursor;			label2->setParent((GUI2base*)Menu);
	
	GUI2checkbox* check3 = new GUI2checkbox;	GUI2label* label3 = new GUI2label;
	check3->pos = {4,4+32};					label3->pos = {4+16,32};
	check3->setParent((GUI2base*)Menu);		label3->text = "show AABB's";
	check3->func = &toggleAABB;				label3->setParent((GUI2base*)Menu);
}

void windowPhysbodyOptions(void *arg)
{
	double x = 	((GUI2spinner*)(((void**)arg)[0]))->vals[1];
	double y = 	(((GUI2spinner*)(((void**)arg)[1]))->vals[1]);
	double z = 	(((GUI2spinner*)(((void**)arg)[2]))->vals[1]);
	double rx =	(((GUI2spinner*)(((void**)arg)[3]))->vals[1]);
	double ry =	(((GUI2spinner*)(((void**)arg)[4]))->vals[1]);
	double rz =	(((GUI2spinner*)(((void**)arg)[5]))->vals[1]);
	double s =  (((GUI2spinner*)(((void**)arg)[6]))->vals[1]);
	printf("New pos: <%f,%f,%f>, new angles: <%f,%f,%f>\n",x,y,z,rx,ry,rz);
	if(EntSelected)
	{
		EntSelected->setPos({x,y,z});
		EntSelected->setOrient(quat::fromAngleAxis(rz,0,0,1)*quat::fromAngleAxis(rx,1,0,0)*quat::fromAngleAxis(ry,0,1,0));
		EntSelected->setScale(s);
		quat q = EntSelected->orient;
		printf("quat: {%f,%f,%f,%f} | {%f,%f,%f,%f}\n",q.w,q.v.x,q.v.y,q.v.z,q.getAngle(),q.getX(),q.getY(),q.getZ());
	}
}
void windowPhysbodyUpdate(void *arg)
{
	
	if(EntSelected)
	{
		((GUI2label*)(((GUI2base*)arg)->findByTag("bodytext")))->text = 
		"body: "+itoa((double)(*(reinterpret_cast<int*>(&EntSelected))))+"\n"+
		"name: "+EntSelected->name+"\n"+
		"id: "+itoa(EntSelected->id)+"\n"+
		"collision count: "+itoa(EntSelected->collisionCount);
		((GUI2spinner*)(((GUI2base*)arg)->findByTag("spinx")))->vals[1]=EntSelected->pos.x;
		((GUI2spinner*)(((GUI2base*)arg)->findByTag("spiny")))->vals[1]=EntSelected->pos.y;
		((GUI2spinner*)(((GUI2base*)arg)->findByTag("spinz")))->vals[1]=EntSelected->pos.z;
	}
	else
	{
		((GUI2label*)(((GUI2base*)arg)->findByTag("bodytext")))->text = "body: none";
	}
}

void windowPhysbody(void* arg)
{
	GUI2frame* Menu = new GUI2frame;			//GUIbutton* btnUpdate = new GUIbutton;
	Menu->setPos(128,128);					//btnUpdate->setPos(4,128);
	Menu->setSize(192,280);					//btnUpdate->setSize(64,32);
	Menu->title = "Body manipulator";		//btnUpdate->text = "Update";
	Menu->setParent(GUI);					//btnUpdate->func = &windowPhysbodyUpdate;
	GUI2base::fixstrata((GUI2base*)Menu);		//btnUpdate->setParent((GUIbase*) Menu);
	GUI2label* bodytext = new GUI2label;
	bodytext->setPos(4,4);
	bodytext->setSize(192-8,64);
	bodytext->text = "body: none";
	bodytext->setParent((GUI2base*) Menu);
	bodytext->tag = "bodytext";
	
	GUI2spinner* spinx = new GUI2spinner;		GUI2label* spinxtext = new GUI2label;
	spinx->setPos(4,48+32);					spinxtext->setPos(96,48+32);
	spinx->setVals(-10,0,10,0.25,2);		spinxtext->text = "pos x";
	spinx->setParent((GUI2base*) Menu);		spinxtext->setParent((GUI2base*) Menu);
	spinx->tag="spinx";
	GUI2spinner* spiny = new GUI2spinner;		GUI2label* spinytext = new GUI2label;
	spiny->setPos(4,48+32+18);				spinytext->setPos(96,48+32+18);
	spiny->setVals(-10,0,10,0.25,2);		spinytext->text = "pos y";
	spiny->setParent((GUI2base*) Menu);		spinytext->setParent((GUI2base*) Menu);
	spiny->tag="spiny";
	GUI2spinner* spinz = new GUI2spinner;		GUI2label* spinztext = new GUI2label;
	spinz->setPos(4,48+32+32+4);			spinztext->setPos(96,48+32+32+4);
	spinz->setVals(-10,0,10,0.25,2);		spinztext->text = "pos z";
	spinz->setParent((GUI2base*) Menu);		spinztext->setParent((GUI2base*) Menu);
	spinz->tag="spinz";
	
	GUI2spinner* spinrx = new GUI2spinner;	GUI2label* spinrxtext = new GUI2label;
	spinrx->setPos(4,48+32+18*4);			spinrxtext->setPos(96,48+32+18*4);
	spinrx->setVals(-180,0,180,15,0);		spinrxtext->text = "rot x";
	spinrx->setParent((GUI2base*) Menu);		spinrxtext->setParent((GUI2base*) Menu);
	spinrx->tag="spinrx";
	
	GUI2spinner* spinry = new GUI2spinner;	GUI2label* spinrytext = new GUI2label;
	spinry->setPos(4,48+32+18*5);			spinrytext->setPos(96,48+32+18*5);
	spinry->setVals(-180,0,180,15,0);		spinrytext->text = "rot y";
	spinry->setParent((GUI2base*) Menu);		spinrytext->setParent((GUI2base*) Menu);
	spinry->tag="spinry";
	
	GUI2spinner* spinrz = new GUI2spinner;	GUI2label* spinrztext = new GUI2label;
	spinrz->setPos(4,48+32+18*6);			spinrztext->setPos(96,48+32+18*6);
	spinrz->setVals(-180,0,180,15,0);		spinrztext->text = "rot z";
	spinrz->setParent((GUI2base*) Menu);		spinrztext->setParent((GUI2base*) Menu);
	spinrz->tag="spinrz";
	
	GUI2spinner* spins = new GUI2spinner;		GUI2label* spinstext = new GUI2label;
	spins->setPos(4,48+32+18*8);			spinstext->setPos(96,48+32+18*8);
	spins->setVals(-5,1,5,0.1,1);			spinstext->text = "scale";
	spins->setParent((GUI2base*) Menu);		spinstext->setParent((GUI2base*) Menu);
	spins->tag="spins";
	
	void **newarg = new void*[7];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	newarg[3] = (void*)spinrx;
	newarg[4] = (void*)spinry;
	newarg[5] = (void*)spinrz;
	newarg[6] = (void*)spins;
	newarg[7] = (void*)bodytext;
	spinx->func = spiny->func = spinz->func = spinrz->func = 
	spinrx->func = spinry->func = spins->func = &windowPhysbodyOptions;
	spinx->arg = spiny->arg = spinz->arg = spinrz->arg = 
	spinrx->arg = spinry->arg = spins->arg = newarg;
	Menu->think = &windowPhysbodyUpdate;
	
	if(!wPBfuncSet)
	{
		wPBfuncSet=true; 
		cmdlist.push_back({"ent_select","select enitity",&entSelect});
		bindKey(1,&entSelect,NULL,1);
		bindKey(2,&entPush,NULL,1);
	}
}