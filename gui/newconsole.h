#ifndef NEWCONSOLE_GUARD
#define NEWCONSOLE_GUARD
#include "../gui/Gui2.h"

void consolecallback(void *arg);
void parse(string str);
void userparse(string str);

extern GUI2frame *newconsole;
extern GUI2textEntry *textentry;
extern GUI2label *text;
extern map<string, double> convars;
extern map<string, funcptr> confuncs;

void OpenNewConsole(GUI2base *GUI);

void consolecallback(void *arg);

void cunprintfromtop();
int countlines(string str);
void cprint(string str);
void cunprint();
void cclear();
extern bool echoOn;
void userparse(string str);
void parse(string str);
#endif