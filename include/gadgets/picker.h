#ifndef PICKER_GUARD
#define PICKER_GUARD
struct entity;

void closeEntityOptionsWindow();
void openEntityOptionsWindow(entity *E);
void pickerSelect(entity *E);
void pickerDeselect(entity *E);
void initPicker();

#endif