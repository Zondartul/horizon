GUIframe *Console;

int ConsoleNumLines;
struct concommand
{
	string cmd;
	string desc;
	funcptr func;
};

vector<concommand> cmdlist;



void ConsoleParse(string text);
void ParseCommand(string inputString);
void MenuConsoleConEntryCallback(void *arg);
void makeInvisible(void *arg);
void OpenMenuConsole();
void cmd_help(void *arg);
void cmd_hi(void *arg);
void cmd_exit(void *arg);
void cmd_resetcam(void *arg);
void cmd_cls(void *arg);
void cmd_color(void *arg);
void cmd_textureLoad(void *arg);
void cmd_textureStore(void *arg);
void cmd_texture(void *arg);
void cmd_quat(void *arg);
void cmd_test(void *arg);
void initConCommands();