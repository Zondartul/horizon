#include "gui_editor.h"
#include "gui_filedialog.h"
#include "file.h"

gui_editor_kind *gui_editor;

gui_editor_kind::gui_editor_kind(){
	mainWindow = new GUIwindow();
	mainWindow->setTitle("GUI editor");
	mainWindow->setSize(vec2(400,300));
	
	GUIbutton *btn1 = new GUIbutton();
	btn1->setText("File");
	//btn1->sizeToContents(); //too small, should add borders
	btn1->setSize(vec2(40,20));
	btn1->setFunction([=](){
		GUIfileDialog *dialog = new GUIfileDialog();
		dialog->setDirectory(getConfigDirectory());
		GUI->addChild(dialog);
	});
	mainWindow->addChild(btn1);
	
	GUI->addChild(mainWindow);
}

void openGuiEditor(){
	if(!gui_editor){gui_editor = new gui_editor_kind();}
	printf("opened gui editor\n");
}