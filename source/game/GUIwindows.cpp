#include "game.h"


void openTestWindow1(){
	texture *img1 = getTexture("art/printer"); setLayer(loadLayer); uploadTexture(img1);

	GUIbase *F = new GUIframe();
		F->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2){5,0});})\
						->setText("boop")->sizeToContents()->moveTo({20,200}))\
			->addChild((new GUIbutton())->setFunction([=](){delete F;})->setText("X")->setSize({20,20})->moveTo({160,0}))
			);

	GUIbase *W = (new GUIwindow())\
		->setSize({150,100})\
		->moveTo({350,20})\
		->addChild((new GUIlabel())->setText("Yer a wizzard Harry!")->sizeToContents());


	GUI->addChild(F);
	GUI->addChild(W);
}

void openTestWindow2(){
	texture *img1 = getTexture("art/printer"); uploadTexture(img1);
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({128,24})->moveTo({20,220});

	GUI->addChild((new GUIwindow())\
		->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([](){GUI->moveTo(GUI->area.start+(vec2){5,0});})\
							->setText("boop")->sizeToContents()->moveTo({20,200}))\
			->addChild(TE)\
			)\
		);
}

void openTestWindow3(){
	texture *tex1 = getTexture("art/printer"); uploadTexture(tex1);
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({96,24})->moveTo({20,220});

	GUIgrid *grid = new GUIgrid();
	GUIbase *lbl1 = (new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20});
	GUIbase *img1 = (new GUIimage())->setImage(tex1)->setSize({128,128})->moveTo({20,64});
	GUIbase *btn1 = (new GUIbutton())->setFunction(
							[](){
								GUI->moveTo(GUI->area.start+(vec2){5,0});
							}
						)->setText("boop")->sizeToContents()->moveTo({20,200});

	printf("------------ grid setup sterted\n");
	grid->addChild(lbl1);
	grid->addChild(img1);
	grid->addChild(btn1);
	grid->addChild(TE);

	grid->grid(lbl1,0,0,2);
	grid->grid(img1,1,0,2);
	grid->grid(btn1,2,1);
	grid->grid(TE,  2,0);
	printf("------------ grid setup done\n");

	GUIbase *window = (new GUIwindow())\
		->setSize({100,100})\
		->moveTo({100,20})\
		->addChild(grid);

	GUI->addChild(window);
	GUI->invalidateTree();
	printf("---- sizing window\n");
	window->sizeToContents();
	printf("---- window sized\n");
	GUI->invalidateTree();
		//->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild(grid)\
		)\
	);
	printf("------------ GUI setup done\n");
}

void openTestWindow4(){
	renderLayer *test4layerSetup = new renderLayer("openTestWindow4.setup");
	renderLayer *test4layer = new renderLayer("test4layer");
	addLayer(layer3D,test4layerSetup);
	addLayer(test4layerSetup,test4layer);
	setLayer(test4layerSetup);
	setTexturing(false);
	setColoring(true);
	setTransparency(false);
	setLighting(false);
	setRenderMode(2);
	setColor(vec3(255,255,0));
	setLayer(test4layer);

	GUIwindow *win = new GUIwindow();
	win->moveTo({250,40});

	GUItextEntry *te_x = new GUItextEntry();
	te_x->setSize({50,20});
	te_x->setText("1");
	GUItextEntry *te_y = new GUItextEntry();
	te_y->setSize({50,20});
	te_y->setText("1");
	GUItextEntry *te_z = new GUItextEntry();
	te_z->setSize({50,20});
	te_z->setText("1");

	GUIbutton *btn_box = new GUIbutton();
	btn_box->setText("box");
	btn_box->setSize({150,30});
	GUIbutton *btn_cyl = new GUIbutton();
	btn_cyl->setText("cyllinder");
	btn_cyl->setSize({150,30});
	GUIbutton *btn_sph = new GUIbutton();
	btn_sph->setText("sphere");
	btn_sph->setSize({150,30});
	GUIbutton *btn_con = new GUIbutton();
	btn_con->setText("cone");
	btn_con->setSize({150,30});
	GUIbutton *btn_clear = new GUIbutton();
	btn_clear->setText("CLEAR\n");
	btn_clear->setSize({150,30});

	GUIgrid *grid = new GUIgrid();
	grid->addChild(te_x);
	grid->addChild(te_y);
	grid->addChild(te_z);
	grid->addChild(btn_box);
	grid->addChild(btn_cyl);
	grid->addChild(btn_sph);
	grid->addChild(btn_con);
	grid->addChild(btn_clear);
	grid->grid(te_x,0,0);
	grid->grid(te_y,0,1);
	grid->grid(te_z,0,2);
	grid->grid(btn_box,1,0,3);
	grid->grid(btn_cyl,2,0,3);
	grid->grid(btn_sph,3,0,3);
	grid->grid(btn_con,4,0,3);
	grid->grid(btn_clear,5,0,3);

	win->addChild(grid);
	win->sizeToContents();
	GUI->addChild(win);

	btn_box->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make box %s\n",toString(size).c_str());
		rmodel *rm = generateBox(size)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodelStd(rm);
		printf("wut\n");
	};
	btn_cyl->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cyllinder %s\n",toString(size).c_str());
		rmodel *rm = generateCyllinder(size.x,size.z)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodelStd(rm);
		printf("wut\n");
	};
	btn_sph->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make sphere %s\n",toString(size).c_str());
		rmodel *rm = generateSphere(size.x)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodelStd(rm);
		printf("wut\n");
	};
	btn_con->F = [=](){
		float numx = te_x->getNumber();
		float numy = te_y->getNumber();
		float numz = te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cone %s\n",toString(size).c_str());
		rmodel *rm = generateCone(size.x,size.z)->getRmodel(1);
		setLayer(loadLayer);
		uploadRmodel(rm);
		setLayer(test4layer);
		drawRmodelStd(rm);
		printf("wut\n");
	};
	btn_clear->F = [=](){
		clearLayer(test4layer);
	};
}
#include "ecs.h"

//-------------------- render options -----------------------------

#define CHECKBOX_FUNC(button, func) \
[=](){\
		string text = button->text;\
		if(text != "v"){			\
			button->setText("v");	\
			func(1);				\
		}else{						\
			button->setText("");	\
			func(0);				\
		}							\
	}


#define SET_RENDER_OPT(x,b) ecs.render->options.x = b; ecs.render->applyRenderOptions();

#define FUNC_FOR_CHECKBOX(x) std::function<void(int)> funcToggle_ ## x = \
	[=](int b){printf("toggle" #x " = %d\n", b); SET_RENDER_OPT(x, b);};

#define MAKE_CHECKBOX(x,gridobj,row) FUNC_FOR_CHECKBOX(x); auto CB_ ## x = new GUIbutton(); \
	CB_ ## x->setText("");\
	CB_ ## x->setFunction(CHECKBOX_FUNC(CB_ ## x, funcToggle_ ## x));\
	CB_ ## x->setSize({25,25});\
	if(ecs.render->options.x){\
		CB_ ## x->setText("v");\
	}else{\
		CB_ ## x->setText("");\
	}\
	\
	auto L_ ## x = new GUIlabel();\
	L_ ## x->setText(#x);\
	L_ ## x->sizeToContents();\
	\
	gridobj->addChild(CB_ ## x);\
	gridobj->addChild(L_ ## x);\
	gridobj->grid(CB_ ## x,row,0);\
	gridobj->grid(L_ ## x,row,1);

//------------ physics options -----------------------------------------------


#define SET_SYS_OPT(x,system,b) system->options.x = b;

#define FUNC_FOR_CHECKBOX2(x, system) std::function<void(int)> funcToggle_ ## x = \
	[=](int b){printf("toggle" #x " = %d\n", b); SET_SYS_OPT(x, system, b);};

#define MAKE_CHECKBOX2(x,system,gridobj,row) FUNC_FOR_CHECKBOX2(x, system); auto CB_ ## x = new GUIbutton(); \
	CB_ ## x->setText("");\
	CB_ ## x->setFunction(CHECKBOX_FUNC(CB_ ## x, funcToggle_ ## x));\
	CB_ ## x->setSize({25,25});\
	if(system->options.x){\
		CB_ ## x->setText("v");\
	}else{\
		CB_ ## x->setText("");\
	}\
	\
	auto L_ ## x = new GUIlabel();\
	L_ ## x->setText(#x);\
	L_ ## x->sizeToContents();\
	\
	gridobj->addChild(CB_ ## x);\
	gridobj->addChild(L_ ## x);\
	gridobj->grid(CB_ ## x,row,0);\
	gridobj->grid(L_ ## x,row,1);

void openTestWindow5(){
	/*
	auto GB = new GUIbutton();
	std::function<void(int)> funcToggleWireframe =
	[=](int b){
		printf("toggleWireframe = %d\n",b);
		//ecs.render->texturing = b;
		ecs.render->options.wireframe = b;
		ecs.render->applyRenderOptions();
	};
	if(ecs.render->options.wireframe){
		GB->setText("v");
	}else{
		GB->setText("");
	}
	GB->setFunction([=](){
		string text = GB->text;
		if(text != "v"){
			GB->setText("v");
			func(1);
		}else{
			GB->setText("");
			func(0);
		}
	});
	GB->setFunction(CHECKBOX_FUNC(GB,funcToggleWireframe));
	GB->setSize({25,25});

	auto L1 = new GUIlabel();
	L1->setText("wireframe");
	L1->sizeToContents();

	std::function<void(int)> funcToggleBoundingBox =
	[=](int b){
		printf("toggleBoundingBox = %d\n",b);
		ecs.render->options.boundingBoxes = b;
		ecs.render->applyRenderOptions();
	};

	auto GB2 = new GUIbutton();
	GB2->setText("");
	GB2->setFunction(CHECKBOX_FUNC(GB2,funcToggleBoundingBox));
	GB2->setSize({25,25});

	auto L2 = new GUIlabel();
	L2->setText("bounding boxes");
	L2->sizeToContents();
	*/
	auto grid1 = new GUIgrid();

	auto L1 = new GUIlabel();
	L1->setText("--- render options ---");
	L1->sizeToContents();
	grid1->addChild(L1);
	grid1->grid(L1,0,1);

	MAKE_CHECKBOX(wireframe,grid1,1);
	MAKE_CHECKBOX(texturing,grid1,2);
	MAKE_CHECKBOX(lighting,grid1,3);
	MAKE_CHECKBOX(coloring,grid1,4);
	MAKE_CHECKBOX(boundingBoxes,grid1,5);
	MAKE_CHECKBOX(uvColoring,grid1,6);
	MAKE_CHECKBOX(normalColoring,grid1,7);

	auto L2 = new GUIlabel();
	L2->setText("--- physics options ---");
	L2->sizeToContents();
	grid1->addChild(L2);
	grid1->grid(L2,8,1);

	MAKE_CHECKBOX2(gravity,ecs.physics,grid1,9);

	auto L3 = new GUIlabel();
	L3->setText("--- collision options ---");
	L3->sizeToContents();
	grid1->addChild(L3);
	grid1->grid(L3,10,1);

	MAKE_CHECKBOX2(separate,ecs.collision,grid1,11);
	MAKE_CHECKBOX2(resolve,ecs.collision,grid1,12);
	/*
	auto grid1 = new GUIgrid();
	grid1->addChild(CB_wireframe);
	grid1->addChild(L_wireframe);
	grid1->addChild(CB_boundingBoxes);
	grid1->addChild(L_boundingBoxes);
	grid1->addChild(CB_lighting);
	grid1->addChild(L_lighting);
	grid1->grid(GB,0,0);
	grid1->grid(L1,0,1);
	grid1->grid(GB2,1,0);
	grid1->grid(L2,1,1);
	grid1->grid(CB_lighting,2,0);
	grid1->grid(L_lighting,2,1);
	*/

	GUI->addChild((new GUIwindow())\
	->setSize({200,300})\
	->moveTo({100,20})\
	->addChild(grid1)\
	->sizeToContents());
}
#include "util/hook.h"
void openTestWindow6(){
	GUIlabel *label1 = new GUIlabel();

	GUIscrollbar *bar = new GUIscrollbar();
	bar\
			->sizeToParent(true)\
			->setInnerSize({730,4000})\
			->setHorizontal(false)\
			->addChild(label1);


	GUIwindow *win = new GUIwindow();
	win->setSize({750,500})\
		->moveTo({100,20})\
		->addChild(bar);

	GUI->addChild(win);


	hookAdd(globalChannel, EVENT_FRAME, "testWindow6.update",
	[=](eventKind event){
		string S = "Memory allocation report:\n";
		if(allocation_map.empty()){
			S += "operators <new> and <delete> are not replaced in this build\n";
		}else{
			int supertotal = 0;
			for(auto F = allocation_map.begin(); F != allocation_map.end(); F++){
				string alloc_file = F->first;
				S += alloc_file + ":\t\t";//":\n";
				auto &r_file = F->second;
				int filetotal = 0;
				for(auto L = r_file.begin(); L != r_file.end(); L++){
					int alloc_line = L->first;
					auto &r_line = L->second;
					int count = r_line.size();
					int size = 0;
					if(count){size = r_line[0].size;}
					int total = size*count;
					S += fstring(" @%d: %d x %d b = %d b\n",alloc_line,count,size,total);
					supertotal += total;
					filetotal += total;
				}
				S += fstring("file: %.3f kb\n",(float)filetotal/1024.f);
			}
			S += fstring("user total: %.3f kb\n",(float)supertotal/1024.f);
			S += fstring("sys  total: %.3f kb\n",(float)total_size/1024.f);
		}
		label1->setText(S);
		label1->sizeToContents();
		win->invalidate();
        //win->sizeToContents();
	});
};

/*

auto scroll1 = new GUIscrollbar([]{
	sizeToParent = true;
	innerSize = {300,600};
	horizontal = false;
});

auto frame1 = new GUIframe([]{
		size = {200,300};
		pos = {100,20};
		addChild(scroll1);
	});

auto label1 = new GUIlabel(text = "Hello World"; sizeToContents = true; pos = {20,20});

GUI->add({
	{frame1,{
		{scroll1, {
			label1,
			image1,
			button1
		}}
		{vertical,{
			{horizontal,{label1,image1,button1}},
			{horizontal,{label1,image1,button1}}
		}}
	}}
});

GUI->addChild(frame1);


auto label = new GUIlabel({"text","Hello World"},{"sizeToContents","true"},{"pos","[20,20]"});

*/

