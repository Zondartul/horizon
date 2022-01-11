#ifndef SELFAWARE_GUARD
#define SELFAWARE_GUARD
#include "vec.h"
#include <string>
using std::string;
vec2i getScreenSize();
void setMouseRelativeMode(bool mode);
vec2i getMousePos();
string locateResource(const char *type, const char *name);

#endif