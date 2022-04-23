#include "renderLayer.h"
#include "renderLow.h"
#include "stdio.h"
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include "renderCommand.h"

#include "paint.h"

#ifndef NO_GLEW
#include "GL/glew.h"
#endif

#include "window.h"


vector<renderLayer*> all_layers;
vector<renderLayer*> layers;


renderLayer::renderLayer(string name,bool persistent, bool special):name(name),persistent(persistent),special(special){
	all_layers.push_back(this);
}

void renderLayer::render(){
    renderLow->renderParseQueue(&queue3, this);
}

void renderLayer::clear(){
	int i = 0;


    for(auto I = queue3.begin(); I != queue3.end(); I++){
        renderCommand3 *cmd = *I;
        delete cmd;
    }
    queue3.clear();
}
void renderLayer::reset(){
    queue3.push_back(new renderCommand3(RC3T::LAYER, resetLayer));
}
void renderLayer::print(){
	int J = 0;

    J = 0;
    for(auto I = queue3.begin(); I != queue3.end(); I++,J++){
        printf("%d: %s\n",J,toCString(*I));
    }
}


void renderLayer::push(renderCommand3 *cmd){
    queue3.push_back(cmd);
}

renderLayer *renderLayer::duplicate(){//https://www.youtube.com/watch?v=kJ6flOet6qc
	renderLayer *L2 = new renderLayer();

    for(auto I = queue3.begin(); I != queue3.end(); I++){
        renderCommand3 *cmd = (*I)->clone();
        if(cmd){    //priviliged commands can not be copied
            L2->queue3.push_back(cmd);
        }
    }
	return L2;
}


string toString(renderLayer *l){
	if(!l){return "<null>";}
	return l->name;
}

void addWithIndent(stringstream &ss1, stringstream &ss2, int num){
	char buff[200];
	while(ss2.getline(buff,199)){
		for(int I = 0; I < num; I++){
			ss1 << " ";
		}
		ss1 << buff;
		if(!ss2.eof()){ss1 << "\n";}
	}
}

string renderLayer::report3(){
	stringstream ss;
	ss << "layer \"" << name << "\"";
	if(persistent){ss << "+persistent ";}
	if(special){ss << "+special ";}
	ss << "\n";

	int J = 0;
	for(auto I = queue3.begin(); I != queue3.end(); I++,J++){
		ss << " " << J << ": " << escapeString(toString(*I)) << "\n";
        renderCommand3 &rcmd = **I;
		if(rcmd.type == RC3T::LAYER){
			stringstream ss2;
			if(rcmd.layer){
				ss2 << rcmd.layer->report3();
			}else{
				ss2 << "<null>\n";
			}
			addWithIndent(ss, ss2, 4);
		}

	}
	return ss.str();
}

const renderCommand3 *renderLayer::get(int num){
    if(queue3.size() > num){
        return queue3[num];
    }else{
        return 0;
    }
}





