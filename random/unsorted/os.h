#ifndef OS_GUARD
#define OS_GUARD

// Module: OS
// links with SDL

//
// tasks: file search, listing, opening, writing
// get:
//		screenSize
//		mousePos
// set:
//		mouseRelativeMode

void os_init();
void os_deinit();
string fileSearch(const char *type, const char *dir, const char *name);
vector<string> fileList(const char *type, const char *dir);

vec2i getScreenSize();
void setMouseRelativeMode(bool mode);
vec2i getMousePos();
string locateResource(const char *type, const char *name);

#endif