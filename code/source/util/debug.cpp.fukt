#include <new>
#include <vector>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#ifndef NO_SDL
#include <SDL2/SDL.h>
#endif
#include "debug.h"
#include "global_vars.h"
#include "config.h"
#include "stringUtils.h"
#include "simplemath.h"
#include "timer.h"
using std::vector;
void clearDeleteBuffer();
void debuginit(){
	auto& debugFile = Gb->gs_debug->g_debugFile;
	debugFile = fopen("log.txt","w");
	setbuf(debugFile,0);
	if(!debugFile){printf("ERROR: can't open debug log file!\n");exit(0);}
	else{printf("opened log file %p\n",debugFile);}
    new timer([=](timer *T){clearDeleteBuffer();}, 3, true);
	#ifdef DEBUG_MALLOC
	printf("DEBUG_MALLOC set\n");
	#endif
	#ifdef DEBUG_NEW
	printf("DEBUG_NEW set\n");
	#endif
	printf("-------- debug init done ----------\n");
}
void crash();
void debugprint(const char *file, int line, const char *mode, const char *format, ...){
	auto& debugFile = Gb->gs_debug->g_debugFile;
	auto& ticks = Gb->gs_debug->g_ticks;
	va_list ap;
	va_start(ap, format);
	char buff[240];
	vsnprintf(buff, 239, format, ap);
	if(debugFile){	
		char buff2[320];
		#ifndef NO_SDL
				uint32_t time = SDL_GetTicks();
		#else
				uint32_t time = 0;
		#endif
		int seconds = time/1000;
		int miliseconds = time%1000;
		snprintf(buff2,319,"[%d.%d]:\tf%d:\t%s:%d:\t\t%s",seconds,miliseconds,ticks,file,line,buff);
		sprintf(buff2,"[%d.%d]:\tf%d:\t%s:%d:\t\t%s",seconds,miliseconds,ticks,file,line,buff);
		fprintf(debugFile,"%s",buff2);
	}	
	va_end(ap);
	if(!strcmp(mode,"memory")){return;}
	if(!strcmp(mode,"printf")){printf("%s",buff);}
	if(!strcmp(mode,"warning")){printf("%s",buff);}
	if(!strcmp(mode,"error")){printf("%s",buff);crash();}
}
int set_alloc_pos(const char *file, int line){
	auto& alloc_file = Gb->gs_debug->g_alloc_file;
	auto& alloc_line = Gb->gs_debug->g_alloc_line;
	alloc_file = file;
	alloc_line = line;
	return 0;
}
int getGameTicks();
#ifdef DEBUG_NEW
void *operator new(size_t size){ 
	void *res = 0;
	static size_t num = 0;
	if(g_alloc_file != 0){
		const char *af = g_alloc_file; g_alloc_file = 0;
		int al = g_alloc_line; g_alloc_line = 0;
		auto &r_afile = g_allocation_map[af];
		auto &r_aline = r_afile[al];
		int num = r_aline.size();
		mapped_alloc_key key = {af,al,num};
		mapped_alloc A;
		A.size = size;
		A.p = malloc(size);
		A.frame = getGameTicks();
        A.freed = false;
		g_deallocation_map[A.p] = key;
		r_aline.push_back(A);
		g_total_size = g_total_size + size;
		res = A.p;
	}else{
		res = malloc(size);
	}
	if(!res){printf("ERROR: OUT OF MEMORY\n"); throw std::bad_alloc();}
	return res;
}
void debug_delete(void *ptr){
	if(!ptr){printf("dbgdel: warning: delete null\n"); return;}
	static int recurseCounter = 0;
	if(!recurseCounter){
		recurseCounter++;
		if(g_deallocation_map.count(ptr)){
			mapped_alloc_key key = g_deallocation_map[ptr];
			auto &r_afile = g_allocation_map[key.g_alloc_file];
			auto &r_line = r_afile[key.g_alloc_line];
			for(auto I = r_line.begin(); I != r_line.end();){
				mapped_alloc &A = *I;
				if(A.p == ptr){
					g_total_size -= A.size;
                    if(g_delayedDelete){
                        A.freed = true;
                        g_deleteBuffer[g_activeDeleteBuffer].push_back(ptr);
                        I++;
                    }else{
                        I = r_line.erase(I);
                        g_deallocation_map.erase(ptr);
                    }
				}else{
					I++;
				}
			}
		}
		recurseCounter--;
	}
    if(!g_delayedDelete){free(ptr);}
}
#endif
void clearDeleteBuffer(){
	auto& delayedDelete = Gb->gs_debug->g_delayedDelete;
	auto& activeDeleteBuffer = Gb->gs_debug->g_activeDeleteBuffer;
	auto& deleteBuffer = Gb->gs_debug->g_deleteBuffer;
	auto& deallocation_map = Gb->gs_debug->g_deallocation_map;
	auto& allocation_map = Gb->gs_debug->g_allocation_map;
    if(!delayedDelete){return;}
    int inactiveDeleteBuffer = 0;
    if(activeDeleteBuffer == 0){inactiveDeleteBuffer = 1;}
    delayedDeleteList &dellist = deleteBuffer[inactiveDeleteBuffer];
    int total = 0;
    for(unsigned int I = 0; I < dellist.size(); I++){
        void *ptr = dellist[I];
        mapped_alloc_key key = deallocation_map[ptr];
        struct_alloc_file &f = allocation_map[key.alloc_file];
        struct_alloc_line &l = f[key.alloc_line];
        for(auto J = l.begin(); J != l.end();){
            mapped_alloc &al = *J;
            if(al.p == ptr){
                J = l.erase(J);
                deallocation_map.erase(ptr);
                free(ptr);
                total++;
            }else{
                J++;
            }
        }
    }
    dellist.clear();
    activeDeleteBuffer = inactiveDeleteBuffer;
}
bool wasRecentlyDeleted(void *ptr){
	auto& deallocation_map = Gb->gs_debug->g_deallocation_map;
	auto& allocation_map = Gb->gs_debug->g_allocation_map;
    if(deallocation_map.count(ptr)){
        mapped_alloc_key key = deallocation_map[ptr];
        struct_alloc_file &f = allocation_map[key.alloc_file];
        struct_alloc_line &l = f[key.alloc_line];
        for(auto J = l.begin(); J != l.end(); J++){
            mapped_alloc &al = *J;
            if(al.p == ptr){
                return al.freed;
            }
        }
    }
    return false;
}
#ifdef DEBUG_NEW
void operator delete(void *ptr, size_t size) noexcept{
	debug_delete(ptr);
}
void operator delete(void *ptr) noexcept{
	debug_delete(ptr);
}
#endif
#ifdef DEBUG_MALLOC
void *debugmalloc(const char *file, int line, size_t size){
	void *p = malloc(size);
	debugprint(file,line,"memory","[memop:%d] malloc(%d) = %p\n",debug_mem_op_index,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	g_debug_mem_allocated += size;
	g_debug_mem_watermark = max(g_debug_mem_watermark, g_debug_mem_allocated);
	int time = SDL_GetTicks();
	int seconds = time/1000;
	int milliseconds = time%1000;
	debugAllocation A;
	A.reported = p;
	A.actual = p;
	A.oldsizereported = 0;
	A.sizereported = size;
	A.sizeactual = size;
	A.file = file;
	A.line = line;
	A.type = "malloc";
	A.seconds = seconds;
	A.milliseconds = milliseconds;
	A.frame = ticks;
	A.op_index = debug_mem_op_index++;
	A.freed = false;
	#ifdef DEBUG_NEW
	g_debugAllocList[g_debugAllocListI++]=A;
	#else
	g_debugAllocList.push_back(A);
	#endif
	return p;
}
void *debugcalloc(const char *file, int line, size_t num, size_t size){
	void *p = calloc(num,size);
	debugprint(file,line,"memory","[memop:%d] calloc(%d,%d) = %p\n",debug_mem_op_index,num,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	g_debug_mem_allocated += size;
	g_debug_mem_watermark = max(g_debug_mem_watermark, g_debug_mem_allocated);
	int time = SDL_GetTicks();
	int seconds = time/1000;
	int milliseconds = time%1000;
	debugAllocation A;
	A.reported = p;
	A.actual = p;
	A.oldsizereported = 0;
	A.sizereported = size;
	A.sizeactual = size;
	A.file = file;
	A.line = line;
	A.type = "calloc";
	A.seconds = seconds;
	A.milliseconds = milliseconds;
	A.frame = ticks;
	A.op_index = debug_mem_op_index++;
	A.freed = false;
	#ifdef DEBUG_NEW
	g_debugAllocList[g_debugAllocListI++]=A;
	#else
	g_debugAllocList.push_back(A);
	#endif
	return p;
}
void *debugrealloc(const char *file, int line, void *ptr, size_t size){
	void *p = realloc(ptr,size);
	debugprint(file,line,"memory","[memop:%d] realloc(%p,%d) = %p\n",debug_mem_op_index,ptr,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	g_debug_mem_allocated += size;
	g_debug_mem_watermark = max(g_debug_mem_watermark, g_debug_mem_allocated);
	int time = SDL_GetTicks();
	int seconds = time/1000;
	int milliseconds = time%1000;
	debugAllocation A;
	A.reported = p;
	A.actual = p;
	A.oldsizereported = 0;
	A.sizereported = size;
	A.sizeactual = size;
	A.file = "unknown";
	A.line = 0;
	A.type = "new";
	A.seconds = seconds;
	A.milliseconds = milliseconds;
	A.frame = ticks;
	A.op_index = debug_mem_op_index++;
	A.freed = false;
	#ifdef DEBUG_NEW
	g_debugAllocList[g_debugAllocListI++]=A;
	#else
	g_debugAllocList.push_back(A);
	#endif
	return p;
}
void debugfree(const char *file, int line, void *ptr){
	bool found = false;
	int I;
	#ifdef DEBUG_NEW
	for(I = debugAllocListSize-1;I >= 0;I--){
	#else
	for(I = g_debugAllocList.size();I>= 0;I--){
	#endif
		if(g_debugAllocList[I].reported == ptr){
			if(!g_debugAllocList[I].freed){
				g_debugAllocList[I].freed = true;
				g_debug_mem_allocated -= g_debugAllocList[I].sizereported;
				found = true;
				break;
			}else{
				debugprint(file,line,"error","[memop:%d] free(%p) (prev memop=%d) ERROR: double-free!\n",debug_mem_op_index,ptr,I);
				abort();
			}
		}
	}
	if(found){debugprint(file,line,"memory","[memop:%d] free(%p) (prev memop=%d)\n",debug_mem_op_index,ptr,I);}
	else{debugprint(file,line,"error","[memop:%d] free(%p) ERROR: wild free!\n",debug_mem_op_index,ptr);abort();}
	free(ptr);
	debug_mem_op_index++;
}
#endif
#define CANARY 247
#define STACK_CANARY_SIZE 10000
void debugSetStackCanary(){
	char s[STACK_CANARY_SIZE];
	memset(s,STACK_CANARY_SIZE,CANARY);
}
int debugCheckStackCanary(){
	char s[STACK_CANARY_SIZE];
	#ifdef __INTELLISENSE__
	#pragma diag_suppress 6001
	#endif
	for(int I = 0; I < STACK_CANARY_SIZE; I++){
		if(s[I] == CANARY){return I;}
	}
	return STACK_CANARY_SIZE;
}
void profileStart(){
	auto& currentDebugProfile = Gb->gs_debug->g_currentDebugProfile;
	auto& debug_mem_allocated = Gb->gs_debug->g_debug_mem_allocated;
	auto& debug_mem_watermark = Gb->gs_debug->g_debug_mem_watermark;
	#ifndef NO_SDL
		currentDebugProfile.time = SDL_GetPerformanceCounter();
	#endif
	currentDebugProfile.heapused = debug_mem_allocated;
	debug_mem_watermark = debug_mem_allocated;
}
debugProfile profileEnd(){
	auto& currentDebugProfile = Gb->gs_debug->g_currentDebugProfile;
	auto& debug_mem_allocated = Gb->gs_debug->g_debug_mem_allocated;
	auto& debug_mem_watermark = Gb->gs_debug->g_debug_mem_watermark;
	#ifndef NO_SDL
		uint64_t time = SDL_GetPerformanceCounter() - currentDebugProfile.time;
		uint64_t freq = SDL_GetPerformanceFrequency();
	#else
		uint64_t time = 0;
		uint64_t freq = 1;
	#endif
	int seconds = time/freq;
	int milliseconds = time/((double)freq/1000) - 1000*seconds; 
	int microseconds = time/((double)freq/1000000) - 1000000*seconds - 1000*milliseconds; 
	currentDebugProfile.time = 1000000*time/freq;
	currentDebugProfile.seconds = seconds;
	currentDebugProfile.milliseconds = milliseconds;
	currentDebugProfile.microseconds = microseconds;
	int prevheap = currentDebugProfile.heapused;
	currentDebugProfile.heapused = debug_mem_watermark - prevheap;
	currentDebugProfile.heapleaked = debug_mem_allocated - prevheap;
	return currentDebugProfile;
}
#ifdef F_INSTRUMENT
extern "C"{
void __cyg_profile_func_enter (void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit (void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_enter (void *this_fn, void *call_site) {
    if(g_mytraceI < 100){
        g_mytrace[g_mytraceI++] = this_fn;
    }
}
void __cyg_profile_func_exit (void *this_fn, void *call_site) {
    if(g_mytraceI){
        g_mytrace[--g_mytraceI] = 0;
    }
}
}
#endif
vector<void*> getTrace(){
	auto& mytrace = Gb->gs_debug->g_mytrace;
	auto& mytraceI = Gb->gs_debug->g_mytraceI;
    vector<void*> v;
    for(int I = 0; I < mytraceI; I++){
        v.push_back(mytrace[I]);
    }
    return v;
}
stackSentinel::stackSentinel(){
	for(int I = 0; I < SSnumvals; I++){
		vals[I] = SSbaseval+I;
	}
	printf("SS constructed\n");
}
#ifndef DEBUG_PRINT
void info(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
void warning(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
void error(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	throw std::runtime_error(string(fmt));
}
void frame(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
#endif
stackSentinel::~stackSentinel(){
	int cor_first = SSnumvals;
	int cor_last = 0;
	bool corrupted = false;
	for(int I = 0; I < SSnumvals; I++){
		if(vals[I] != SSbaseval+I){
			cor_first = (int)min(cor_first, I);
			cor_last = (int)max(cor_last, I);
			corrupted = true;
		}
	}
	if(corrupted){
		printf("stackSentinel: bytes (%d) through (%d) corrupted (%d total)\n",cor_first,cor_last,cor_last-cor_first);
		error("stackSentinel: bytes (%d) through (%d) corrupted (%d total)\n",cor_first,cor_last,cor_last-cor_first);
	}else{
		printf("SS: stack ok\n");
	}
}
