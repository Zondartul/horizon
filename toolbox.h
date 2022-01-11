//The little window with random tools!

void genBox(void* arg);

void OpenMenuToolbox()
{
	GUIframe* toolFrame = new GUIframe;
	toolFrame->setPos(64,64);
	toolFrame->setSize(64+8,256);
	toolFrame->title = "Toolbox";
	toolFrame->setParent(GUI);
	
	GUIbutton* btnBox = new GUIbutton;							GUIbutton* btnCyl = new GUIbutton;
	btnBox->setPos(4,4);										btnCyl->setPos(4+32,4);
	btnBox->setSize(32,32);										btnCyl->setSize(32,32);
	btnBox->setImage("C:/Stride/textures/cube.bmp");			btnCyl->setImage("C:/Stride/textures/cylinder.bmp");
	btnBox->func = &genBox;										btnCyl->func = &genBox;
	btnBox->arg = NULL;											btnCyl->arg = NULL;
	btnBox->setParent((GUIbase*) toolFrame);					btnCyl->setParent((GUIbase*) toolFrame);
	
	GUIbutton* btnSphere = new GUIbutton;						GUIbutton* btnCone = new GUIbutton;
	btnSphere->setPos(4,4+32);									btnCone->setPos(4+32,4+32);
	btnSphere->setSize(32,32);									btnCone->setSize(32,32);
	btnSphere->setImage("C:/Stride/textures/sphere.bmp");		btnCone->setImage("C:/Stride/textures/cone.bmp");
	btnSphere->func = &genBox;									btnCone->func = &genBox;
	btnSphere->arg = NULL;										btnCone->arg = NULL;
	btnSphere->setParent((GUIbase*) toolFrame);					btnCone->setParent((GUIbase*) toolFrame);
	/*
	GUIImage* image1 = new GUIImage;
	image1->setPos(32,32);
	image1->setSize(32,32);
	image1->setImage("C:/Stride/textures/grass3.bmp");//"C:/Stride/grass3.raw");
	image1->setParent((GUIbase*)designerFrame);
	*/
	ParseCommand("textureload C:/Stride/textures/crate32.bmp");
	ParseCommand("textureload C:/Stride/textures/grass3.bmp");
}

void genCube(void *arg)
{
	printf("<1>");
	double l = 	((GUIspinner*)(((void**)arg)[0]))->vals[1];
	double w = 	((GUIspinner*)(((void**)arg)[1]))->vals[1];
	double h = 	((GUIspinner*)(((void**)arg)[2]))->vals[1];
	vec V = {l,w,h};
	model* M = new model;
	printf("<2>");
	//((GUIspinner*)(((void**)arg)[0]))->vals[1] = V.length();
	
	M->mesh = new triangle[12];
	printf("<3>");
	M->texmap = new textriangle[12];
	M->numtextures = 2;
	printf("<4>");
	M->textures = new texture[2];
	M->textures[0] = textureGet("C:/Stride/textures/crate32.bmp");
	M->textures[1] = textureGet("C:/Stride/textures/grass3.bmp");
	printf("<5>");
	
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
	
	
	vec A = {0,	0,	0}; //A = A*10;
	vec B = {0,	V.y,0}; //B = B*10;
	vec C = {V.x,V.y,0}; //C = C*10;
	vec D = {V.x,0,	0}; //D = D*10;
	vec E = {0,	0,	V.z}; //E = E*10;
	vec F = {0,	V.y,V.z}; //F = F*10;
	vec G = {V.x,V.y,V.z};// G = G*10;
	vec H = {V.x,0,	V.z}; //H = H*10;
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
	M->mesh[0] = (triangle){A,E,H}; M->texmap[0] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[1] = (triangle){H,D,A}; M->texmap[1] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//right
	M->mesh[2] = (triangle){D,H,G}; M->texmap[2] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[3] = (triangle){G,C,D}; M->texmap[3] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//back
	M->mesh[4] = (triangle){C,G,F}; M->texmap[4] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[5] = (triangle){F,B,C}; M->texmap[5] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//left
	M->mesh[6] = (triangle){B,F,E}; M->texmap[6] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[7] = (triangle){E,A,B}; M->texmap[7] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	//up
	
	M->mesh[8] = (triangle){E,F,G}; M->texmap[8] = (textriangle){{tC,tA,tB},1,R_TEXTURE};
	M->mesh[9] = (triangle){G,H,E}; M->texmap[9] = (textriangle){{tB,tD,tC},1,R_TEXTURE};
	//down
	M->mesh[10] = (triangle){B,A,D}; M->texmap[10] = (textriangle){{tC,tA,tB},0,R_TEXTURE};
	M->mesh[11] = (triangle){D,C,B}; M->texmap[11] = (textriangle){{tB,tD,tC},0,R_TEXTURE};
	
	//
	M->numtris = 12;//12;
	
	printf("<6>");
	myModel = M;
	printf("<7>");
	//OpenMenuModel();
}

void genBox(void* arg)
{
	GUIframe* GenMenu = new GUIframe;
	GenMenu->setPos(128,128);
	GenMenu->setSize(256,128);
	GenMenu->title = "Generator";
	GenMenu->setParent(GUI);
	
	GUIspinner* spinx = new GUIspinner;		GUIlabel* spinxtext = new GUIlabel;
	spinx->setPos(4,4);						spinxtext->setPos(96,4);
	spinx->setVals(-10,0,10,0.5,2);			spinxtext->text = "length";
	spinx->setParent((GUIbase*) GenMenu);	spinxtext->setParent((GUIbase*) GenMenu);

	GUIspinner* spiny = new GUIspinner;		GUIlabel* spinytext = new GUIlabel;
	spiny->setPos(4,4+18);					spinytext->setPos(96,4+16);
	spiny->setVals(-10,0,10,0.5,2);			spinytext->text = "width";
	spiny->setParent((GUIbase*) GenMenu);	spinytext->setParent((GUIbase*) GenMenu);
	
	GUIspinner* spinz = new GUIspinner;		GUIlabel* spinztext = new GUIlabel;
	spinz->setPos(4,4+32+4);					spinztext->setPos(96,4+32);
	spinz->setVals(-10,0,10,0.5,2);			spinztext->text = "height";
	spinz->setParent((GUIbase*) GenMenu);	spinztext->setParent((GUIbase*) GenMenu);
	
	void **newarg = new void*[3];
	newarg[0] = (void*)spinx;
	newarg[1] = (void*)spiny;
	newarg[2] = (void*)spinz;
	GUIbutton* btnGen = new GUIbutton;
	btnGen->setPos(4,4+64);
	btnGen->setSize(96,16);
	btnGen->text = "Generate";
	btnGen->func = &genCube;
	btnGen->arg = (void*)newarg;
	btnGen->setParent((GUIbase*) GenMenu);
	
}