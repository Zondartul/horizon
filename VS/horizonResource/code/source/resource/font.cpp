#include "resource/resource.h"
#include "resource/fonts.h"
#include <sstream>
using std::stringstream;

string toString(font* f) {
	if (!f) { return fstring("font:[null]"); }
	return fstring("font:[%s]", f->name.c_str());
}
template<> font* fromString<font*>(const string S) {
	if (S == "font:[null]") { return 0; }
	//char buff[80];
	//std::ignore = sscanf(S.c_str(),"font:[%[^]]]",buff);
	string name;
	char c;
	stringstream ss(S);
	ss >> c >> c >> c >> c >> c >> c >> name >> c;
	printf("fromString<font*>: S = [%s]\n", S.c_str());
	printf("fromString<font*>: buff = [%s]\n", name.c_str());//buff);
	//buff[79] = 0;
	//string name(buff);
	return getFont(name);
}