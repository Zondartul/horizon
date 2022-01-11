#ifndef DEBUG_GUARD
#define DEBUG_GUARD
#include <map>
#include <string>
#include <stack>
using std::map;
using std::string;
using std::stack;
extern map<string,bool> debugdisabled; //list of debug places for which messages are disabled.
extern stack<string> debugcurrent;		//current debug place
extern int print_file;
extern int print_line;
extern int print_current;
#define debug(...) if(! debugdisabled[debugcurrent.top()] ){ \
if(print_file or print_line or print_current){printf("[");} \
if(print_file){printf("%s", __FILE__ );} \
if(print_file and (print_line or print_current)){printf(":");}\
if(print_line){printf("%d", __LINE__ );} \
if((print_file or print_line) and print_current){printf(":");}\
if(print_current){printf("%s", debugcurrent.top().c_str());} \
if(print_file or print_line or print_current){printf("] ");} \
printf( __VA_ARGS__ );}

#endif