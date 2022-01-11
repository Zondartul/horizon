#ifndef VALSCREEN_GUARD
#define VALSCREEN_GUARD
#include "Gui2.h"

extern GUI2label *valscreentext;

extern map<string,string> valscreenvals;

void valscreenrender(void *arg);

void OpenValScreen(GUI2base *GUI);

void printvals(string str1, string str2);
#endif