#ifndef GUI_PROPERTY_GUARD
#define GUI_PROPERTY_GUARD
//#include "tinyxml2.h"		//we pretty much have to include this
//using namespace tinyxml2;	

class GUIbase;
namespace tinyxml2{
class XMLElement;
class XMLDocument;
}
using namespace tinyxml2;
//typedef map<string,string> GUIpropertyTable;
struct GUIpropertyTable{
	map<string,string> table;
	GUIpropertyTable operator+(const GUIpropertyTable &other);
	string toString() const;
	GUIpropertyTable() = default;
	//constructs the property table from an XMLElement
	GUIpropertyTable(XMLElement *el);
	//adds all properties as attributes to XMLElement el
	void toXML(XMLElement *el) const;
};

//function to concatenate several tables
//GUIpropertyTable combinePropertyTables(vector<GUIpropertyTable> tables);

struct GUIcompoundProperty{
	string name;
	GUIpropertyTable table;
	vector<GUIcompoundProperty> children;
	GUIbase *instantiate() const; //creates GUIwidgets from this compound
	string toString() const;
	GUIcompoundProperty() = default;
	//constructs the compound property from an XML element
	GUIcompoundProperty(XMLElement *el);
	//creates a new XMLElement and fills it with compound information
	XMLElement *toXML(XMLDocument *doc) const;
	~GUIcompoundProperty();
};

#endif