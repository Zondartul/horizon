#include "game/game.h"
#include "gadgets/gui_texturebrowser.h"
#include "gadgets/gui_filedialog.h"
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
	if(!lbl){throw std::runtime_error("no lbl");}

	btn = dynamic_cast<GUIbutton*>(getByName("TBbtnsel"));
	if(!btn){throw std::runtime_error("no btn");}
	btn->setFunction([=](){addTile("materials/APPULS","APPULS");});
	
	sg = dynamic_cast<GUIselectionGroup*>(getByName("TBselgroup"));
	//if(!sg){throw std::runtime_error("no sg");}

	scroll = dynamic_cast<GUIscrollbar*>(getByName("TBscroll"));
	//if(!scroll){throw std::runtime_error("no scroll");}
	
	TBpath = dynamic_cast<GUItextEntry*>(getByName("TBpath"));
	TBpath->setFunction([&](){setDirectory(TBpath->text);});

	TBbtnBrowse = dynamic_cast<GUIbutton*>(getByName("TBbtnBrowse"));
	TBbtnBrowse->setFunction(std::bind(&GUItextureBrowser::onBtnBrowse, this));
	assert(lbl && btn && sg && scroll && TBpath && TBbtnBrowse);

	setDirectory("materials");
	//populate();
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

bool begins_with(string A, string B){
	return A.substr(0,B.length()) == B;
}
string skip(string A, string B){
	return A.substr(B.length());
}

void GUItextureBrowser::setDirectory(std::string path){
	/// remove the base-path from the url
	if(begins_with(path, basepath)){ path = skip(path, basepath);}
	if(begins_with(path, getOSdirSeparator())){path = skip(path, getOSdirSeparator());}

	dir = path;
	TBpath->setText(path);
	populate();
}

void GUItextureBrowser::clear(){
	for(auto ch:sg->getChildren()){ch->close();}
	numTiles = 0;
}

void GUItextureBrowser::populate(){
	printf("tb::populate()\n");
	clear();
	string dir1 = basepath; //"resource/textures";
	string dir2 = dir; //"materials";
	string dir12 = dir1 + "/" + dir2;
	vector<string> ls = getDirectoryList(dir12);//(dir1+"/"+dir2);
	printf("dir (%s) has %d entries\n",dir12,ls.size());//(dir1+"/"+dir2).c_str(), ls.size());
	for(auto I = ls.begin(); I != ls.end(); I++){
		string filename = *I;
		string nameNoExt = getFileNameNoExt(filename);
		string fullpath = dir12 + "/" + filename;//dir1 +"/" + dir2 + "/" + filename;
		if(!isFolder(fullpath)){
			string resname = dir2 + "/" + nameNoExt;
			addTile(resname,filename);
		}
	}
}

/// bind first argument
/// from https://stackoverflow.com/questions/33724042/bind-first-argument-of-function-without-knowing-its-arity
template <typename Base, typename... Head>
[[nodiscard]] auto bind_head(Base&& func, Head&&... args) {
    return [base{std::forward<Base>(func)}, head{std::make_tuple(std::forward<Head>(args)...)}](auto&&... tail) mutable {
        return std::apply(base, std::tuple_cat(head, std::forward_as_tuple(std::forward<decltype(tail)>(tail)...)));
    };
}

void GUItextureBrowser::onBtnBrowse(){
	auto fd = new GUIfileDialog();
	string dir1 = basepath; //"resource/textures";
	string dir2 = dir; //"materials";
	string dir12 = dir1 + "/" + dir2;
	fd->setDirectory(dir12);
	fd->setMode(GUIe_fileop::Folder);
	fd->setFunction(bind_head(&GUItextureBrowser::setDirectory, this));
	addChild(fd);
}