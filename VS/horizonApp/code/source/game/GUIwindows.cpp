#include "game/game.h"
#include "util/hook.h"
#include "util/global_vars_gui.h"
#include "util/global_vars_render.h"
#include "util/global_vars_util.h"
#include "util/global_vars_app.h"
#include <iostream>

using namespace std;

void closeTestWindows() {
	auto& testWindows = Ga->gs_commands->testWindows;
	for (auto* win : testWindows) {
		win->close();
	}
	testWindows.clear();
}

void openTestWindow1(){
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	texture *img1 = getTexture("art/printer").val(); setLayer(loadLayer); uploadTexture(img1);
	GUIbase *F = new GUIframe();
		F->setSize({200,300})\
		->moveTo({100,20})\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({300,600})\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20}))\
			->addChild((new GUIimage())->setImage(img1)->setSize({128,128})->moveTo({20,64}))\
			->addChild((new GUIbutton())->setFunction([GUI](){GUI->moveTo(GUI->area.start+vec2{5,0});})\
						->setText("boop")->sizeToContents()->moveTo({20,200}))\
			->addChild((new GUIbutton())->setFunction([=](){delete F;})->setText("X")->setSize({20,20})->moveTo({160,0}))
			);
	GUIbase *W = (new GUIwindow())\
		->setSize({150,100})\
		->moveTo({350,20})\
		->addChild((new GUIlabel())->setText("Yer a wizzard Harry!")->sizeToContents());
	GUI->addChild(F);
	GUI->addChild(W);
	testWindows.push_back(F);
	testWindows.push_back(W);
}
void openTestWindow2(){
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	texture *img1 = getTexture("art/printer").val(); uploadTexture(img1);
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({128,24})->moveTo({20,220});
	auto* win = (new GUIwindow())\
		->setSize({ 200,300 })\
		->moveTo({ 100,20 })\
		->addChild((new GUIscrollbar())\
			->sizeToParent(true)\
			->setInnerSize({ 300,600 })\
			->setHorizontal(false)\
			->addChild((new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({ 20,20 }))\
			->addChild((new GUIimage())->setImage(img1)->setSize({ 128,128 })->moveTo({ 20,64 }))\
			->addChild((new GUIbutton())->setFunction([GUI]() {GUI->moveTo(GUI->area.start + vec2{ 5,0 }); })\
				->setText("boop")->sizeToContents()->moveTo({ 20,200 }))\
			->addChild(TE)\
		);
	GUI->addChild(win);
	testWindows.push_back(win);
}
void openTestWindow3(){
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	texture *tex1 = getTexture("art/printer").val(); uploadTexture(tex1);
	auto TE = new GUItextEntry();
	TE->setFunction([=](){printf("Text:[%s]\n",TE->text.c_str());})->setSize({96,24})->moveTo({20,220});
	GUIgrid *grid = new GUIgrid();
	GUIbase *lbl1 = (new GUIlabel())->setText("Hello World")->sizeToContents()->moveTo({20,20});
	GUIbase *img1 = (new GUIimage())->setImage(tex1)->setSize({128,128})->moveTo({20,64});
	GUIbase *btn1 = (new GUIbutton())->setFunction(
							[GUI](){
								GUI->moveTo(GUI->area.start+vec2{5,0});
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
	printf("------------ GUI setup done\n");
	testWindows.push_back(window);
}

struct testWindow4data {
	renderLayer* test4layer = 0;
	renderLayer* test4layerSetup = 0;
};

void testWindow4_prepare_wireframe_layer(testWindow4data &data){
	auto& layer3D = Gr->gs_paint->g_layer3D;
	data.test4layerSetup = new renderLayer("openTestWindow4.setup");
	data.test4layer = new renderLayer("test4layer");
	addLayer(layer3D,data.test4layerSetup);
	addLayer(data.test4layerSetup,data.test4layer);
	setLayer(data.test4layerSetup);
	setTexturing(false);
	setColoring(true);
	setTransparency(false);
	setLighting(false);
	setRenderMode(2);
	setColor(vec3(255,255,0));
	setLayer(data.test4layer);
}

void openTestWindow4(){
	auto& layer3D = Gr->gs_paint->g_layer3D;
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;
	testWindow4data data;
	//testWindow4_prepare_wireframe_layer(data);

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
	testWindows.push_back(win);
	btn_box->F = [=](){
		float numx = (float)te_x->getNumber();
		float numy = (float)te_y->getNumber();
		float numz = (float)te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make box %s\n",toString(size).c_str());
		setLayer(loadLayer);
		setLayer(data.test4layer);
		printf("wut\n");
	};
	btn_cyl->F = [=](){
		float numx = (float)te_x->getNumber();
		float numy = (float)te_y->getNumber();
		float numz = (float)te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cyllinder %s\n",toString(size).c_str());
		setLayer(loadLayer);
		setLayer(data.test4layer);
		printf("wut\n");
	};
	btn_sph->F = [=](){
		float numx = (float)te_x->getNumber();
		float numy = (float)te_y->getNumber();
		float numz = (float)te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make sphere %s\n",toString(size).c_str());
		setLayer(loadLayer);
		setLayer(data.test4layer);
		printf("wut\n");
	};
	btn_con->F = [=](){
		float numx = (float)te_x->getNumber();
		float numy = (float)te_y->getNumber();
		float numz = (float)te_z->getNumber();
		vec3 size = vec3(numx,numy,numz);
		printf("make cone %s\n",toString(size).c_str());
		setLayer(loadLayer);
		setLayer(data.test4layer);
		printf("wut\n");
	};
	btn_clear->F = [=](){
		if (data.test4layer) { clearLayer(data.test4layer); }
	};
}
std::function<void(void)> checkbox_func(GUIbutton* button, std::function<void(int)> func) {
	return [=]() {
		if (button->text != "v") {
			button->setText("v");
			func(1);
		}
		else {
			button->setText("");
			func(0);
		}
	};
}
void set_render_opt(bool* opt, bool val) {
}
std::function<void(int)> func_for_checkbox(bool* opt, string name) {
	return [=](int val) {
		cout << "toggle " << name << " = " << val << endl;
		set_render_opt(opt, val);
	};
}
void make_checkbox(bool* val, string name, GUIgrid* gridobj, int row) {
	auto CB = new GUIbutton();
	CB->setText("");
	CB->setFunction(checkbox_func(CB, func_for_checkbox(val, name)));
	CB->setSize({ 25,25 });
	if (*val) {
		CB->setText("v");
	}
	else {
		CB->setText("");
	}
	auto L = new GUIlabel();
	L->setText(name);
	L->sizeToContents();
	gridobj->addChild(CB);
	gridobj->addChild(L);
	gridobj->grid(CB, row, 0);
	gridobj->grid(L, row, 1);
}
std::function<void(int)> func_for_checkbox2(bool* val, string name) {
	return [=](int b) {
		cout << "toggle " << name << " = " << b << endl;
		*val = b;
	};
}
void make_checkbox2(bool *val, string name, GUIgrid *gridobj, int row) {
	auto CB = new GUIbutton();
	CB->setText("");
	CB->setFunction(checkbox_func(CB, func_for_checkbox2(val, name)));
	CB->setSize({25,25});
	if(val){
		CB->setText("v");
	}else{
		CB->setText("");
	}
	auto L = new GUIlabel();
	L->setText(name);
	L->sizeToContents();
	gridobj->addChild(CB);
	gridobj->addChild(L);
	gridobj->grid(CB,row,0);
	gridobj->grid(L,row,1);
}
void openTestWindow5(){
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	auto grid1 = new GUIgrid();
	auto L1 = new GUIlabel();
	L1->setText("--- render options ---");
	L1->sizeToContents();
	grid1->addChild(L1);
	grid1->grid(L1,0,1);
	auto L2 = new GUIlabel();
	L2->setText("--- physics options ---");
	L2->sizeToContents();
	grid1->addChild(L2);
	grid1->grid(L2,8,1);
	auto L3 = new GUIlabel();
	L3->setText("--- collision options ---");
	L3->sizeToContents();
	grid1->addChild(L3);
	grid1->grid(L3,10,1);
	auto* win = (new GUIwindow())\
		->setSize({ 200,300 })\
		->moveTo({ 100,20 })\
		->addChild(grid1)\
		->sizeToContents();
	GUI->addChild(win);
	testWindows.push_back(win);
}

void openTestWindow6(){
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& globalChannel = Gu->sysEvent->globalChannel;
	//auto& allocation_map = Gr->gs_debug->g_allocation_map;
	//auto& total_size = Gr->gs_debug->g_total_size;
	auto& sysEvent = Gu->sysEvent;
	auto& testWindows = Ga->gs_commands->testWindows;

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
	testWindows.push_back(win);

	sysEvent->hookAdd(&globalChannel, EVENT_FRAME, "testWindow6.update",
	[=](eventKind event){
		static int i = 0;
		if(i < 100){i++; return;}
		i = 0;
		string S = "Memory allocation report:\n";
		/*if(allocation_map.empty()){
			S += "operators <new> and <delete> are not replaced in this build\n";
		}else{
			int supertotal = 0;
			for(auto F = allocation_map.begin(); F != allocation_map.end(); F++){
				string alloc_file = F->first;
				S += alloc_file + ":\t\t";
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
		}*/
		label1->setText(S);
		label1->sizeToContents();
		win->invalidate();
	});
};

void openTestWindow7() {
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	GUIlabel* label1 = new GUIlabel();
	GUI->addChild(label1);
	testWindows.push_back(label1);
	label1->setText("Hello World!");
	label1->sizeToContents();
}

void openTestWindow8() {
	auto& GUI = Gg->gs_GUI->g_GUI;
	auto& testWindows = Ga->gs_commands->testWindows;

	GUIwindow* win = new GUIwindow();
	
	vector<string> fonts = {
		"cour 14", "cour 16", "cour 22", 
		"times 14", "times 16", "times 22",
		"calibri 12", "calibri 14", "calibri 16", 
		"calibri 18", "calibri 20", "calibri 22"
	};

	unsigned int y = 0;
	for (auto fname : fonts) {
		GUIlabel* lbl = new GUIlabel();
		lbl->setText(fname);

		auto f = getFont(fname);
		if(f.ok()){
			font* F = f.val();
			assert(F);
			lbl->setTextFont(F);
		}else{
			push(*f.err);
		}
		
		lbl->sizeToContents();
		win->addChild(lbl);
		lbl->moveTo(vec2(0, y));
		//lbl->area.moveBy(vec2(0, y));
		y += 32;
	}
	//win->sizeToContents();
	win->setSize(vec2(128, y+32));
	GUI->addChild(win);
	testWindows.push_back(win);
}
