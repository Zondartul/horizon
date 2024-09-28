#include "game/game.h"
#include "gadgets/gui_texturebrowser.h"
#include "program/file.h"
//#include "Ext/tinyXML2/tinyxml2.h"
#include "GUI/GUI_property.h"
#include "util/global_vars_gui.h"
#include "GUI/GUI.h"
#include "resource/resource.h"
#include <stdexcept>
#include <sstream>
using std::stringstream;
using namespace tinyxml2;

GUIcompoundProperty loadGUI(string filepath){
	GUIcompoundProperty P;
	XMLDocument doc;
	if(!fileReadable(filepath)){
		//error("can't read file [%s]\n",filepath.c_str());
		stringstream ss;
		ss << "can't read file [" << filepath << "]\n";
		throw std::runtime_error(ss.str());
	}
	doc.LoadFile(filepath.c_str());
	XMLElement *el = doc.RootElement();
	GUIcompoundProperty P2(el);
	for(auto I = P2.children.begin(); I != P2.children.end(); I++){
		const GUIcompoundProperty &P3 = *I;
		if(P3.table.table.at("isClient") == "1"){
			P = P3;
			break;
		}
	}
	return P;
}
GUItextureBrowser::GUItextureBrowser(){
	printf("GUItextureBrowser()\n");
	Pwindow = loadGUI("data/texturebrowser.gui.xml");
	this->setCompoundProperty(Pwindow);
	Ptile = loadGUI("data/sel_tile_small.gui.xml");
	printf("GTB constructed\n");
	lbl = dynamic_cast<GUIlabel*>(getByName("TBlbl"));
	if(!lbl){
		//error("no lbl");
		throw std::runtime_error("no lbl");
	}
	btn = dynamic_cast<GUIbutton*>(getByName("TBbtnsel"));
	if(!btn){
		//error("no btn");
		throw std::runtime_error("no btn");
	}
	btn->setFunction([=](){
		addTile("materials/APPULS","APPULS");
	});
	sg = dynamic_cast<GUIselectionGroup*>(getByName("TBselgroup"));
	if(!sg){
		//error("no sg");
		throw std::runtime_error("no sg");
	}
	scroll = dynamic_cast<GUIscrollbar*>(getByName("TBscroll"));
	if(!scroll){
		//error("no scroll");
		throw std::runtime_error("no scroll");
	}
	populate();
}
void openTextureBrowser(){
	auto& GUI = Gg->gs_GUI->g_GUI;
	GUI->addChild(new GUItextureBrowser());
}
void GUItextureBrowser::addTile(string imgpath, string label){
	GUIselectable *sel = dynamic_cast<GUIselectable*>(Ptile.instantiate());
	int Sx = (int)sel->area.size.x;
	int Sy = (int)sel->area.size.y;
	int TPR = 4; 
	sel->moveTo(vec2(Sx*(numTiles%TPR),Sy*(numTiles/TPR)));
	sel->moveable = false;
	GUIimage *img = dynamic_cast<GUIimage*>(sel->getByName("SLimage"));
	if(imgpath != ""){
		auto tex = getTexture(imgpath);
		if(tex.ok()){
			img->setImage(tex.val());
		}else{
			push(*tex.err);
		}
	}
	GUIlabel *lbl = dynamic_cast<GUIlabel*>(sel->getByName("SLtext"));
	if(label != ""){lbl->setText(label);}
	sg->addChild(sel);
	sg->sizeToContents();
	scroll->sizeToContents();
	numTiles++;
}
void GUItextureBrowser::populate(){
	printf("tb::populate()\n");
	string dir1 = "resource/textures";
	string dir2 = "materials";
	vector<string> ls = getDirectoryList(dir1+"/"+dir2);
	printf("dir (%s) has %d entries\n",(dir1+"/"+dir2).c_str(), (int)ls.size());
	for(auto I = ls.begin(); I != ls.end(); I++){
		string filename = *I;
		string nameNoExt = getFileNameNoExt(filename);
		string fullpath = dir1 +"/" + dir2 + "/" + filename;
		if(!isFolder(fullpath)){
			string resname = dir2 + "/" + nameNoExt;
			addTile(resname,filename);
		}
	}
}
