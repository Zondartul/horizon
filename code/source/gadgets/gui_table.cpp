#include "gadgets/gui_table.h"

GUItable::GUItable(){
	bar = new GUIscrollbar();
	bar->setInnerSize(vec2(400,150));
	addChild(bar);
	grid = new GUIgrid();
	bar->addChild(grid);
}

GUItable::GUItable(GUIpropertyTable tab):GUItable(){
	setTable(tab);
}

GUItable &GUItable::setTable(GUIpropertyTable tab){
	//todo: also clean up before adding entries
	table.entries = tab;
	grid->resize(table.entries.table.size(),2);
	grid->configureColumn(0,32,128,1.0);
	grid->configureColumn(1,32,128,1.0);
	int y = 0;
	int row = 0;
	vec2 lblSize = vec2(128,32);
	vec2 teSize = vec2(128,32);
	for(auto I = tab.table.begin(); I != tab.table.end(); I++){
		
		string name = I->first;
		string val = I->second;
		//grid->configureRow(row,32,32,1.0);
		
		//GUItableEntry e = *I;
		GUIlabel *l = new GUIlabel(name);
		//l->sizeToContents();
		l->setSize(lblSize);
		//l->moveTo(vec2(0,y));
		//bar->addChild(l);
		grid->addChild(l);
		grid->grid(l,row,0);
		
		GUItextEntry *te = new GUItextEntry();
		te->setText(val);
		te->setSize(teSize);
		//te->sizeToContents();
		//te->moveTo(vec2(128,y));
		te->setFunction([=](){
			printf("GUItextEntry: onEnter: calling..\n");
			if(!this){printf("no this\n");}else{printf("has this(%p)\n",this);}
			if(!(this->f_entryChanged)){printf("no f\n");}else{printf("has f\n");}
			if(!te){printf("no te\n");}else{printf("has te(%p)\n",te);}
			string key = name;
			string val = te->text;
			printf("calling this->f(%s,%s)\n",key.c_str(),val.c_str());
			this->f_entryChanged(key,val);
		});
		//bar->addChild(te);
		grid->addChild(te);
		grid->grid(te,row++,1);
		
		table.entry_widgets[name] = te;
		
		y+= 32;
	}
	bar->setInnerSize(vec2(lblSize.x+teSize.x,y));
	//grid->sizeToContents();
	//bar->sizeToContents();
	return *this;
}

void GUItable::invalidate(){
	GUIwindow::invalidate();
	bar->setSize(clientArea.size);
}

GUItable &GUItable::setFunction(function<void(string, string)> f){
	f_entryChanged = f;
	return *this;
}