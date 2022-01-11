#ifndef WINDOW_MODELLER_GUARD
#define WINDOW_MODELLER_GUARD
#include "../util/messenger.h"
void OpenWindowModeller();
void startModellerSession();
class ModellerInput: public messageReceiver{
	void receiveMessage(message *msg);
};
#endif