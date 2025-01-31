#ifndef DEBUG_GUARD
#define DEBUG_GUARD
#include <cstdio>
#include <cstdint>
#include <vector>
#include <map>
#include "config.h"

#define checkNaN(x) if(x != x){printf(#x " is nan at %d\n",__LINE__);}
#define print1(x) printf(#x " = %s\n", toString(x))

#ifndef NO_SDL
#define profile(x,y) {\
	uint64_t t1 = SDL_GetPerformanceCounter();\
	uint64_t freq = SDL_GetPerformanceFrequency();\
	{x;};\
	uint64_t t2 = SDL_GetPerformanceCounter();\
	y = (float)t2-t1;\
	y = y / freq;\
	}
#else
#define profile(x,y)
#endif


void debuginit();
void debugprint(const char *file, int line, const char *mode, const char *format, ...);
//extern int g_debug_mem_allocated;
//extern int g_debug_mem_watermark;
//extern bool g_printf_enabled;
#ifdef DEBUG_PRINT


	#define printf(format,...)	if(g_printf_enabled){debugprint(__FILE__,__LINE__,"printf",format , ##__VA_ARGS__);}
	#define info(format,...) 	if(g_printf_enabled){debugprint(__FILE__,__LINE__,"info",format , ##__VA_ARGS__);}
	#define warning(format,...) if(g_printf_enabled){debugprint(__FILE__,__LINE__,"warning",format, ##__VA_ARGS__);}
	#define error(format,...)	debugprint(__FILE__,__LINE__,"error",format , ##__VA_ARGS__);crash();
	#define frame(format,...) 	if(g_printf_enabled){debugprint(__FILE__,__LINE__,"frame",format , ##__VA_ARGS__);}


#else

	//#define info(...) printf(__VA_ARGS__)
	//#define warning(...) printf(__VA_ARGS__)
	//#define error(...) printf(__VA_ARGS__);crash();
	//#define frame(...) printf(__VA_ARGS__)
void info(const char *fmt, ...);
void warning(const char *fmt, ...);
void error(const char *fmt, ...);
void frame(const char *fmt, ...);

#endif
int set_alloc_pos(const char *file, int line);
#ifdef DEBUG_MALLOC
	#include "stdlib.h"
	#define malloc(x) debugmalloc(__FILE__,__LINE__,x)
	#define calloc(x,y) debugcalloc(__FILE__,__LINE__,x,y)
	#define realloc(x,y) debugrealloc(__FILE__,__LINE__,x,y)
	#define free(x) debugfree(__FILE__,__LINE__,x)
	void *debugmalloc(const char *file, int line, size_t size);
	void *debugcalloc(const char *file, int line, size_t num, size_t size);
	void *debugrealloc(const char *file, int line, void *ptr, size_t size);
	void debugfree(const char *file, int line, void *ptr);
#endif

struct mapped_alloc{
	int size;
	void *p;
	int frame;
    bool freed;
};
struct mapped_alloc_key{
	const char *alloc_file;
	int alloc_line;
	int num;
};
//typedef vector<mapped_alloc> struct_alloc_line;
//typedef map<int,struct_alloc_line> struct_alloc_file;
//extern map<const char*,struct_alloc_file> g_allocation_map;
//extern map<void*, mapped_alloc_key> g_deallocation_map;

//extern int g_total_size;

#ifdef DEBUG_NEW
	#include <new>
	//extern const char *g_debug_file;
	//extern int g_debug_line;

	//extern const char *alloc_file;
	//extern int alloc_line;
	//void *operator new(size_t size);
	//void operator delete(void *ptr);
	//#define new (g_debug_file=__FILE__,g_debug_line=__LINE__) && 0 ? NULL : new
	//#define delete (g_debug_file=__FILE__,g_debug_line=__LINE__) && 0 ? NULL : delete
	//void debugnew(const char *file, int line, size_t size);
	//void debugdelete(const char *file, int line, void *ptr);
	//#define new set_alloc_pos(__FILE__,__LINE__), new
	#define new set_alloc_pos(__FILE__,__LINE__) && 0 ? NULL : new
    //only works when DEBUG_NEW is on.
    //Returns true if the memory block was freed recently.
    bool wasRecentlyDeleted(void *p);
#else
    //#define wasRecentlyDeleted(x) (false)
#endif



#ifdef DEBUG_ANALYZER

struct debugProfile{
	uint64_t time;
	uint32_t seconds;
	uint32_t milliseconds;
	uint32_t microseconds;
	uint32_t stackused;
	uint32_t heapused;
	uint32_t heapleaked;
};

void profileStart();
debugProfile profileEnd();
#endif
void crash();

#define SSnumvals 1024
#define SSbaseval 1266184225
class stackSentinel{
	public:
	volatile int vals[SSnumvals];
	stackSentinel();
	~stackSentinel();
};

typedef std::vector<mapped_alloc> struct_alloc_line;
typedef std::map<int, struct_alloc_line> struct_alloc_file;
typedef std::vector<void*> delayedDeleteList;


//struct debugProfile {
//	uint64_t time;
//	uint32_t seconds;
//	uint32_t milliseconds;
//	uint32_t microseconds;
//	uint32_t stackused;
//	uint32_t heapused;
//	uint32_t heapleaked;
//}; 

struct debugAllocation {
	void* reported;
	void* actual;
	size_t oldsizereported;
	size_t sizereported;
	size_t sizeactual;
	const char* file;
	int line;
	const char* type;
	uint32_t seconds;
	uint32_t milliseconds;
	int frame;
	int op_index;
	bool freed;
};

struct gs_debugKind {
	delayedDeleteList g_deleteBuffer[2] = { delayedDeleteList{},delayedDeleteList{} };
	int g_activeDeleteBuffer = 0;
	bool g_delayedDelete = false; //causes some lag because memory new-delete-new memory reuse is blocked
	std::map<const char*, struct_alloc_file> g_allocation_map;
	std::map<void*, mapped_alloc_key> g_deallocation_map;

#ifdef DEBUG_NEW
#define debugAllocListSize 10000
	int g_debugAllocListI = 0;
	debugAllocation g_debugAllocList[debugAllocListSize];
#else
	std::vector<debugAllocation> g_debugAllocList;
#endif

	const char* g_alloc_file = 0;
	int g_alloc_line = 0;
	int g_total_size = 0;
	int g_ticks = 0;
	FILE* g_debugFile;

	int g_debug_mem_allocated = 0;
	int g_debug_mem_watermark = 0;
	int g_debug_mem_op_index = 0;
	bool g_redirect_new = 1;
	bool g_redirect_delete = 1;
	const char* g_debug_file = "unknown";
	int g_debug_line = 0;
	debugProfile g_currentDebugProfile;

	void* g_mytrace[100];
	int g_mytraceI;

};

#endif
