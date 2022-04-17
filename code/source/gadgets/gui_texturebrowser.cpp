#include "game.h"
#include "gui_texturebrowser.h"
#include "file.h"
#include "tinyxml2.h"
using namespace tinyxml2;

GUIcompoundProperty loadGUI(string filepath){
	GUIcompoundProperty P;
	
	XMLDocument doc;
	if(!fileReadable(filepath)){error("can't read file [%s]\n",filepath.c_str());}
	doc.LoadFile(filepath.c_str());
	XMLElement *el = doc.RootElement();
	
	GUIcompoundProperty P2(el);
	
	//this silly construct is used because we still have "workwindow" in the layout file.
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
	if(!lbl){error("no lbl");}
	
	btn = dynamic_cast<GUIbutton*>(getByName("TBbtnsel"));
	if(!btn){error("no btn");}
	btn->setFunction([=](){
		addTile("materials/APPULS","APPULS");
	});
	
	sg = dynamic_cast<GUIselectionGroup*>(getByName("TBselgroup"));
	if(!sg){error("no sg");}
	
	scroll = dynamic_cast<GUIscrollbar*>(getByName("TBscroll"));
	if(!scroll){error("no scroll");}
	
	populate();
}


void openTextureBrowser(){GUI->addChild(new GUItextureBrowser());}

void GUItextureBrowser::addTile(string imgpath, string label){
	GUIselectable *sel = dynamic_cast<GUIselectable*>(Ptile.instantiate());
	int Sx = sel->area.size.x;
	int Sy = sel->area.size.y;
	int TPR = 4; //tiles per row
	sel->moveTo(vec2(Sx*(numTiles%TPR),Sy*(numTiles/TPR)));
	sel->moveable = false;
	
	GUIimage *img = dynamic_cast<GUIimage*>(sel->getByName("SLimage"));
	if(imgpath != ""){img->setImage(getTexture(imgpath));}
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
	printf("dir (%s) has %d entries\n",(dir1+"/"+dir2).c_str(), ls.size());
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













