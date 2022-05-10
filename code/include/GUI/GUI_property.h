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
	GUIpropertyTable(XMLElement *el);
	void toXML(XMLElement *el) const;
};
struct GUIcompoundProperty{
	string name;
	GUIpropertyTable table;
	vector<GUIcompoundProperty> children;
	GUIbase *instantiate() const; 
	string toString() const;
	GUIcompoundProperty() = default;
	GUIcompoundProperty(XMLElement *el);
	XMLElement *toXML(XMLDocument *doc) const;
	~GUIcompoundProperty();
};
#endif