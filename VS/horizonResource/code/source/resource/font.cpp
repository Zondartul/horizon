#include "resource/resource.h"
#include "resource/fonts.h"
#include "util/res_or_err.hpp"
#include "util/zerror.hpp"
#include <sstream>
using std::stringstream;

string DLLAPI_RESOURCE toString(font* f) {
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

template<> Result<font*,zError> DLLAPI_RESOURCE fromString<font*>(const string S) {
	if (S == "font:[null]") { return zError("null value"); }
	
	string name;
	char c;
	stringstream ss(S);
	
	skipText(ss, "font:[");
	int size;
	ss >> name >> size;
	stringstream ss2;
	ss2 << name << " " << size;
	name = ss2.str();

	//printf("fromString<font*>: S = [%s]\n", S.c_str());
	//printf("fromString<font*>: buff = [%s]\n", name.c_str());
	
	return getFont(name);
}
