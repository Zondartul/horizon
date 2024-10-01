#include "resource/resource.h"
#include "resource/texture.h"
#include "resource/bitmap.h"
#include <sstream>
#include <tuple> // for std::ignore
using std::stringstream;

texture::texture(){
	UV = {{0,0},{0,0},{0,0}};
}
int texture::w(){if(!bmp){return 0;} return UV.size.x*bmp->width;}
int texture::h(){if(!bmp){return 0;} return UV.size.y*bmp->height;}
vec2 texture::size(){return {w(),h()};} 
rect texture::getRect(){return rect(size());}


string DLLAPI_RESOURCE toString(texture* t) {
	if (!t) { return fstring("tex:[null]"); }
	return fstring("tex:[%s]", t->name.c_str());
}
template<> Result<texture*,zError> DLLAPI_RESOURCE fromString<texture*>(const string S) {
	if (S == "tex:[null]") { return 0; }
	char buff[80];
	buff[0] = 0;
	int count = sscanf(S.c_str(),"tex:[%[^]]]",buff);
	string name;
	if(count){
		name = string(buff);
		printf("fromString<texture*>: S = [%s]\n", S.c_str());
		printf("fromString<texture*>: buff = [%s]\n", name.c_str());//buff);
	}else{
		name = S;
	}
	return getTexture(name);
}