#ifndef VALSCREEN_GUARD
#define VALSCREEN_GUARD
#include "Gui2.h"

extern GUIlabel *valscreentext;

extern map<string,string> valscreenvals;

void valscreenrender(void *arg);

void OpenValScreen(GUIbase *GUI);

void printvals(string str1, string str2);
#endif