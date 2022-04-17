#ifndef GUI_PROPERTY_GUARD
#define GUI_PROPERTY_GUARD

class GUIbase;
namespace tinyxml2{
class XMLElement;
class XMLDocument;
}
using namespace tinyxml2;
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