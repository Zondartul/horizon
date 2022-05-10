#ifndef CONSOLE_GUARD
#define CONSOLE_GUARD
#include <functional>
#include <string>
#include "event.h"
#include "renderLayer.h"
using std::function;
using std::string;
typedef int(*funcptr)(int,char**);
class consoleKind;
class terminalKind{
	public:
	virtual void print(string text) = 0;
	function<void(string)> onEnter;
	virtual ~terminalKind() = default;
};
class dropDownTerminal:public eventListener, public terminalKind{
	public:
	renderLayer *layer;
	string inputText;
	consoleKind *console;
	vector<string> terminalStrings;
	dropDownTerminal();
	virtual ~dropDownTerminal();
	bool terminalOn;
	void print(string text);
	void onEvent(eventKind event);
};
struct consoleCommand{
	string name;
	string help;
	funcptr f;
};
class consoleKind{
	public:
	vector<consoleCommand> commands;
	terminalKind *term;
	consoleKind();
	virtual ~consoleKind();
	void addCommand(consoleCommand cmd);
	consoleCommand *getCommand(string cmd);
	void run(string text);
	void print(string text);
};
void initConsole();
void cprint(const char *format, ...);
struct gs_consoleKind {
	consoleKind* g_console;
};
#endif
