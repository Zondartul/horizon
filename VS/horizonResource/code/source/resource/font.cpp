#include "resource/resource.h"
#include "resource/fonts.h"
#include <sstream>
using std::stringstream;

string toString(font* f) {
	if (!f) { return fstring("font:[null]"); }
	return fstring("font:[%s]", f->name.c_str());
}

void skipText(std::istream& stream, string text) {
	/// maybe we can directly set the read-pointer?
	/// but then we'd have to check for errors
	char c;
	for (auto c2 : text) {
		stream >> c;
	}
}

template<> font* fromString<font*>(const string S) {
	if (S == "font:[null]") { return 0; }
	//char buff[80];
	//std::ignore = sscanf(S.c_str(),"font:[%[^]]]",buff);
	string name;
	char c;
	stringstream ss(S);
	/// this should read "f o n t : [ text number ]" but reads "f o n t : [ text ]"
	//ss >> c >> c >> c >> c >> c >> c >> name >> c;
	skipText(ss, "font:[");
	int size;
	ss >> name >> size;
	stringstream ss2;
	ss2 << name << " " << size;
	name = ss2.str();

	printf("fromString<font*>: S = [%s]\n", S.c_str());
	printf("fromString<font*>: buff = [%s]\n", name.c_str());//buff);
	//buff[79] = 0;
	//string name(buff);
	return getFont(name);
}