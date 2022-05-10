#ifndef PICKER_GUARD
#define PICKER_GUARD
void closeEntityOptionsWindow();
void initPicker();
class GUIwindow;
class renderLayer;
struct gs_pickerKind {
	GUIwindow* g_entWindow = 0;
	renderLayer* g_pickerLayer = 0;
};
#endif