#ifndef WINDOW_MODELLER_GUARD
#define WINDOW_MODELLER_GUARD
#include "../util/messenger.h"
void OpenWindowModeller();
void startModellerSession();
class ModellerInput: public PSsubscriber{
	void PSreceive(message msg);
};
#endif