#ifndef PICKER_GUARD
#define PICKER_GUARD
class entity;

void closeEntityOptionsWindow();
void openEntityOptionsWindow(entity *E);
void pickerSelect(entity *E);
void pickerDeselect(entity *E);
void initPicker();

class GUIwindow;
class entity;
class renderLayer;

struct gs_pickerKind {
	GUIwindow* g_entWindow = 0;
	entity* g_pickedEntity = 0;
	renderLayer* g_pickerLayer = 0;
};

#endif