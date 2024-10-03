#include "game/game.h"
#include "gadgets/gui_filedialog.h"
#include "program/file.h"
#include "util/timer.h"
#include "resource/resource.h"

GUIfileDialog::GUIfileDialog(){
	setSize(vec2(300,224));
	setTitle("Open file");
	btn_up = new GUIbutton();
	btn_up->setImage(getTexture("gui/icontriangleup").val());
	btn_up->setText("");
	btn_up->setSize(vec2(24,24));
	addChild(btn_up);
	address_field = new GUItextEntry();
	address_field->moveTo(vec2(24,0));
	address_field->setSize(vec2(300-24,24));
	address_field->setFunction([=](){setDirectory(address_field->text);});
	addChild(address_field);
	btn_up->setFunction([=](){
		string dir = this->address_field->text;
		dir += getOSdirSeparator()+".."+getOSdirSeparator();
		dir = toCanonicalPath(dir);
		setDirectory(dir);
		});
	scroll = new GUIscrollbar();
	scroll->moveTo(vec2(0,24));
	scroll->setSize(vec2(300-0,200-24-24));
	scroll->setHorizontal(false);
	scroll->setVertical(true);
	addChild(scroll);
	select_field = new GUItextEntry();
	select_field->moveTo(vec2(0,200-24));
	select_field->setSize(vec2(300-50,24));
	addChild(select_field);
	btn_select = new GUIbutton();
	btn_select->setText("Open");
	btn_select->moveTo(vec2(300-50,200-24));
	btn_select->setSize(vec2(50,24));
	btn_select->setFunction([=](){
		string S = select_field->text;
		string dir = address_field->text;
		string fullname = dir + getOSdirSeparator() + S;
		switch(mode){
			case GUIf::Save:
				if(S == ""){return;}
				if(fileExists(fullname)){
					if(btn_select->text != "Overwrite?"){
						btn_select->setText("Overwrite?");
						btn_select->bgColor = vec3(128,128,0);
						btn_select->borderColor = vec3(255,0,0);
						btn_select->hoverColor = vec3(255,255,0);
						btn_select->setSize(vec2(90,24));
						btn_select->moveTo(vec2(300-90,200-24));
						select_field->setSize(vec2(300-90,24));
						return;
					}else{
						func(fullname);
						this->close();
						return;
					}
				}else{
					func(fullname);
					this->close();
					return;
				}
			break;
			case GUIf::Open:
				if(S == ""){return;}
				func(fullname);
				this->close();
				return;
			break;
			case GUIf::Folder:
				if((S != "") && (isFolder(dir+ getOSdirSeparator()+S))){
					fullname = dir+ getOSdirSeparator()+S;
				}else{fullname = dir;}
				func(fullname);
				this->close();
			break;
		}
	});
	addChild(btn_select);
}
GUIfileDialog &GUIfileDialog::setDirectory(string dir){
	address_field->setText(dir);
	scroll->setScrollRatio(vec2(0,0));
	for(unsigned int I = 0; I < elements.size(); I++){
		GUIbase *b = elements[I];
		delete b;
	}
	elements.clear();
	vector<string> ls = getDirectoryList(dir);
	int y = 0;
	for(auto S : ls){
		string fullname = dir+getOSdirSeparator()+S;
		texture *t = getFileIcon(fullname);
		GUIimage *img = new GUIimage();
		img->setImage(t);
		img->moveTo(vec2(0,y));
		img->setSize(vec2(16,16));
		scroll->addChild(img);
		elements.push_back(img);
		GUIbutton *btn = new GUIbutton();
		btn->setText(S);
		btn->moveTo(vec2(18,y));
		y += 18;
		btn->sizeToContents();
		btn->area.size.y = 16;
		btn->noframe = true;
		btn->setFunction([=](){
			select_field->setText(S);
			float t1 = btn->lastPressTime;
			float t2 = getRealTime();
			float dt = t2-t1;
			printf("dt = %f\n",dt);
			if(dt<0.3f){
				string S = select_field->text;
				string dir = address_field->text;
				if(S == ""){return;}
				string fullname = dir + getOSdirSeparator() + S;
				if(isFolder(fullname)){
					setDirectory(fullname);
					return;
				}else{
					if((mode == GUIf::Open) || (mode == GUIf::Save)){
						btn_select->F();
					}
				}
			}
		});
		scroll->addChild(btn);
		elements.push_back(btn);
	}
	vec2 insize = scroll->innerSize;
	insize.y = (float)y;
	scroll->setInnerSize(insize);
	scroll->setScrollRatio(vec2(0,0));
	return *this;
}
GUIfileDialog &GUIfileDialog::setMode(GUIe_fileop newmode){
	mode = newmode;
	string Sb;
	string St;
	switch(mode){
		case GUIf::Open: Sb = "Open"; St = "Open file"; break;
		case GUIf::Save: Sb = "Save"; St = "Save file"; break;
		case GUIf::Folder: Sb = "Select"; St = "Select folder"; break;
	}
	btn_select->setText(Sb);
	setTitle(St);
	return *this;
}
GUIfileDialog &GUIfileDialog::setFunction(function<void(string)> f){
	func = f;
	return *this;
}

texture* getFileIcon(string filename) {
	string ext = getFileExtension(filename);
	texture* t = 0;
	if (isFolder(filename)) {
		t = getTexture("gui/iconfolder").val();
	}
	else
		if (
			(ext == "txt") ||
			(ext == "c") ||
			(ext == "cpp") ||
			(ext == "h") ||
			(ext == "hpp") ||
			(ext == "cfg")
			) {
			t = getTexture("gui/iconfiletxt").val();
		}
		else
			if (
				(ext == "bmp") ||
				(ext == "png") ||
				(ext == "jpg") ||
				(ext == "jpeg") ||
				(ext == "img")
				) {
				t = getTexture("gui/iconpicture").val();
			}
			else
				if (
					(ext == "zip") ||
					(ext == "rar") ||
					(ext == "gz") ||
					(ext == "7z")
					) {
					t = getTexture("gui/iconzipfile").val();
				}
				else
					if (
						(ext == "exe")
						) {
						t = getTexture("gui/iconfileexe").val();
					}
					else
						if (
							(ext == "bat")
							) {
							t = getTexture("gui/iconfilecmd").val();
						}
						else
							if (
								(ext == "dll")
								) {
								t = getTexture("gui/icongear2").val();
							}
							else
								if (
									(ext == "a")
									) {
									t = getTexture("gui/iconfilegear").val();
								}
								else {
									t = getTexture("gui/iconfile").val();
								}
	return t;
}