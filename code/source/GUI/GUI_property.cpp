#include "GUI_internal.h"
#include "tinyxml2.h"
using namespace tinyxml2;
GUIpropertyTable GUIpropertyTable::operator+(const GUIpropertyTable &other){
	GUIpropertyTable table = *this;
	table.table.insert(other.table.begin(),other.table.end());
	return table;
}
string GUIpropertyTable::toString() const{
	string S = "(\n";
	for(auto I = table.begin(); I != table.end(); I++){
		auto k = I->first;
		auto v = I->second;
		S = S+"("+k+": "+v+")\n";
	}
	S += ")\n";
	return S;
}
GUIpropertyTable::GUIpropertyTable(XMLElement *el){
	if(!el){error("nullptr");}
	const XMLAttribute *atr = el->FirstAttribute();
	while(atr){
		string k = atr->Name();
		string v = atr->Value();
		table[k] = v;
		atr = atr->Next();
	}
}
void GUIpropertyTable::toXML(XMLElement *el) const{
	if(!el){error("nullptr");}
	for(auto I = table.begin(); I != table.end(); I++){
		string k = I->first;
		string v = I->second;
		el->SetAttribute(k.c_str(),v.c_str());
	}
}
GUIbase *GUIcompoundProperty::instantiate() const{
	GUIbase *W = 0;
	W = GUI_constructor_dispatch(name);
	if(W){
		W->setCompoundProperty(*this);
	}
	return W;
}
string GUIcompoundProperty::toString() const{
	string S = "(\n";
	S = S+"name: "+name+"\n";
	S += table.toString();
	for(auto I = children.begin(); I != children.end(); I++){
		const GUIcompoundProperty &P = *I;
		S += P.toString();
	}
	S += ")\n";
	return S;
}
GUIcompoundProperty::GUIcompoundProperty(XMLElement *el){
	if(!el){error("nullptr");}
	name = el->Name();
	table = GUIpropertyTable(el);
	XMLElement *el2 = el->FirstChildElement();
	while(el2){
		children.push_back(GUIcompoundProperty(el2));
		el2 = el2->NextSiblingElement();
	}
}
XMLElement *GUIcompoundProperty::toXML(XMLDocument *doc) const{
	if(!doc){error("nullptr");}
	XMLElement *el = doc->NewElement(name.c_str());
	table.toXML(el);
	for(auto I = children.begin(); I != children.end(); I++){
		const GUIcompoundProperty &P = *I;
		XMLElement *el2 = P.toXML(doc);
		el->InsertEndChild(el2);
	}
	return el;
}
GUIcompoundProperty::~GUIcompoundProperty(){
}
