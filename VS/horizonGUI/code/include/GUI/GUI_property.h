#ifndef GUI_PROPERTY_GUARD
#define GUI_PROPERTY_GUARD
#include "util/globals_gui.h"
#include <map>
#include <vector>
#include <string>
using std::map;
using std::vector;
using std::string;

class GUIbase;

namespace tinyxml2{
	class DLLAPI_GUI XMLDocument;
	class DLLAPI_GUI XMLElement;
	class DLLAPI_GUI XMLAttribute;
	class DLLAPI_GUI XMLComment;
	class DLLAPI_GUI XMLText;
	class DLLAPI_GUI XMLDeclaration;
	class DLLAPI_GUI XMLUnknown;
	class DLLAPI_GUI XMLPrinter;
	class DLLAPI_GUI XMLNode;
}

using namespace tinyxml2;

struct DLLAPI_GUI GUIpropertyTable{
	map<string,string> table;
	GUIpropertyTable operator+(const GUIpropertyTable &other);
	string toString() const;
	GUIpropertyTable() = default;
	GUIpropertyTable(XMLElement *el);
	void toXML(XMLElement *el) const;
};

struct DLLAPI_GUI GUIcompoundProperty{
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