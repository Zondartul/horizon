#ifndef NEWCONSOLE_GUARD
#define NEWCONSOLE_GUARD
#include "../gui/Gui2.h"

void consolecallback(void *arg);
void parse(string str);
void userparse(string str);

GUIframe *newconsole;
GUItextEntry *textentry;
GUIlabel *text;
map<string, double> convars;
map<string, funcptr> confuncs;

void OpenNewConsole(GUIbase *GUI);

void consolecallback(void *arg);

void cunprintfromtop();
int countlines(string str);
void cprint(string str);
void cunprint();
void cclear();
bool echoOn;
void userparse(string str);
void parse(string str);
#endif