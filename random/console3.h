#ifndef CONSOLE3_GUARD
#define CONSOLE3_GUARD
#include "util/globals.h"
#include "gui/Gui2.h"
class consoleChar{
	color4i bgcolor;
	color4i fgcolor;
	char C;
};

//more like a terminal really
class console3:public GUI2base{
	public:
	//keeps track of printing position in the window
	int cursorpos;
	//how many lines have we scrolled down
	int scrolllines;
	//window size in characters
	int width_chars;
	int height_chars;
	//output history height in lines
	int height_history;
	//character size in pixels
	int char_width;
	int char_height;
	//current drawing color
	color4i fgcolor;
	color4i bgcolor;
	//stores the command currently being entered
	string inputBuffer;
	//stores previously entered commands
	vector<string> inputHistory;
	//stores previously printed output
	vector<consoleChar> outputHistory;
	//stores the characters to be drawn in the terminal window
	vector<consoleChar> drawBuffer;
	//print formatted output to cursor, advance cursor by amount printed
	void printf(const char *fmt, ...);
	//print formatted output to specific coordinates
	void printfXY(int x, int y, const char *fmt, ...);
	//print a c string to cursor, advance cursor by amount printed
	void print(const char *str);
	//print a c string to specific coordinates
	void printXY(int x, int y, const char *str);
	//print character to cursor, advance cursor by one
	void printchar(char C);
	//print character to specific coordinates
	void printcharXY(char C);
	
	//custom constructor
	console3();
	//custom render function
	void render();
	//custom message receiver
	void receiveMessage(message *msg);
};

#endif