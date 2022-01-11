//i claim temporary insanity while writing this

#include <new>
#include "stdlib.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stringUtils.h"
#include <vector>
using std::vector;
#include "SDL2/SDL.h"
#include "simplemath.h"
#define DEBUG_GUARD
#include "globals.h"
//extern int ticks;
int ticks = 0;
FILE *debugFile;

int debug_mem_allocated = 0;
int debug_mem_watermark = 0;
int debug_mem_op_index = 0;
bool redirect_new = 1;
bool redirect_delete = 1;
const char *debug_file = "unknown";
int debug_line = 0;

struct debugAllocation{
	void *reported;
	void *actual;
	size_t oldsizereported;
	size_t sizereported;
	size_t sizeactual;
	const char *file;
	int line;
	const char *type;
	uint32_t seconds;
	uint32_t milliseconds;
	int frame;
	int op_index;
	bool freed;
};
#ifdef DEBUG_NEW
#define debugAllocListSize 10000
int debugAllocListI = 0;
debugAllocation debugAllocList[debugAllocListSize];
#else
vector<debugAllocation> debugAllocList;
#endif
void debuginit(){
	debugFile = fopen("log.txt","w");
	setbuf(debugFile,0);
	if(!debugFile){printf("ERROR: can't open debug log file!\n");exit(0);}
	else{printf("opened log file %p\n",debugFile);}
}

void debugprint(const char *file, int line, const char *mode, const char *format, ...){
	va_list ap;
	va_start(ap, format);
	
	char buff[240];
	vsnprintf(buff, 239, format, ap);
	//vsprintf(buff,format,ap);
	char buff2[320];
	uint32_t time = SDL_GetTicks();
	int seconds = time/1000;
	int miliseconds = time%1000;
	snprintf(buff2,319,"[%d.%d]:\tf%d:\t%s:%d:\t\t%s",seconds,miliseconds,ticks,file,line,buff);
	sprintf(buff2,"[%d.%d]:\tf%d:\t%s:%d:\t\t%s",seconds,miliseconds,ticks,file,line,buff);
	fprintf(debugFile,"%s",buff2);
	va_end(ap);
	
	if(!strcmp(mode,"memory")){return;}
	if(!strcmp(mode,"printf")){printf("%s",buff);}
	if(!strcmp(mode,"warning")){printf("%s",buff);}
	if(!strcmp(mode,"error")){printf("%s",buff);exit(0);}
}

#ifdef DEBUG_NEW
void *operator new(size_t size){
	if(redirect_new || !debug_line){
		redirect_new = 0;
		void *p = malloc(size);
		debugprint(debug_file,debug_line,"memory","[memop:%d] new(%d) = %p\n",debug_mem_op_index,size,p);
		if(!p){printf("ERROR: allocation failed!\n");exit(0);}
		debug_mem_allocated += size;
		debug_mem_watermark = max(debug_mem_watermark, debug_mem_allocated);
		
		int time = SDL_GetTicks();
		int seconds = time/1000;
		int milliseconds = time%1000;
		debugAllocation A;
		A.reported = p;
		A.actual = p;
		A.oldsizereported = 0;
		A.sizereported = size;
		A.sizeactual = size;
		A.file = debug_file;
		A.line = debug_line;
		A.type = "new";
		A.seconds = seconds;
		A.milliseconds = milliseconds;
		A.frame = ticks;
		A.op_index = debug_mem_op_index++;
		A.freed = false;
		debug_file = "unknown";
		debug_line = 0;
		#ifdef DEBUG_NEW
		debugAllocList[debugAllocListI++]=A;
		#else
		debugAllocList.push_back(A);
		#endif
		
		redirect_new = 1;
		return p;
	}else{
		void *p = malloc(size);
		return p;
	}
}
void operator delete(void *ptr){
	if(redirect_delete || !debug_line){
		redirect_delete = 0;
		//debugprint("unknown",0,"memory","[memop:%d] delete(%p)\n",debug_mem_op_index,ptr);
		
		bool found = false;
		int I;
		#ifdef DEBUG_NEW
		for(I = debugAllocListSize-1;I >= 0;I--){
		#else
		for(I = debugallocList.size();I>= 0;I--){
		#endif
			if(debugAllocList[I].reported == ptr){
				if(!debugAllocList[I].freed){
					debugAllocList[I].freed = true;
					debug_mem_allocated -= debugAllocList[I].sizereported;
					found = true;
					break;
				}else{
					debugprint(debug_file,debug_line,"error","[memop:%d] delete(%p) (prev memop=%d) ERROR: double-delete!\n",debug_mem_op_index,ptr,I);
					exit(0);
				}
			}
		}
		if(found){debugprint(debug_file,debug_line,"memory","[memop:%d] delete(%p) (prev memop=%d)\n",debug_mem_op_index,ptr,I);}
		else{debugprint(debug_file,debug_line,"error","[memop:%d] delete(%p) ERROR: wild delete!\n",debug_mem_op_index,ptr);abort();}
		
		free(ptr);
		debug_mem_op_index++;
		debug_file = "unknown";
		debug_line = 0;
		redirect_delete = 1;
		return;
	}else{
		free(ptr);
		return;
	}
}
#endif

void *debugmalloc(const char *file, int line, size_t size){
	void *p = malloc(size);
	debugprint(file,line,"memory","[memop:%d] malloc(%d) = %p\n",debug_mem_op_index,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	
	debug_mem_allocated += size;
	debug_mem_watermark = max(debug_mem_watermark, debug_mem_allocated);
	
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
	debugAllocList[debugAllocListI++]=A;
	#else
	debugAllocList.push_back(A);
	#endif
	
	return p;
}

void *debugcalloc(const char *file, int line, size_t num, size_t size){
	void *p = calloc(num,size);
	debugprint(file,line,"memory","[memop:%d] calloc(%d,%d) = %p\n",debug_mem_op_index,num,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	
	debug_mem_allocated += size;
	debug_mem_watermark = max(debug_mem_watermark, debug_mem_allocated);
	
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
	debugAllocList[debugAllocListI++]=A;
	#else
	debugAllocList.push_back(A);
	#endif
	return p;
}

void *debugrealloc(const char *file, int line, void *ptr, size_t size){
	void *p = realloc(ptr,size);
	debugprint(file,line,"memory","[memop:%d] realloc(%p,%d) = %p\n",debug_mem_op_index,ptr,size,p);
	if(!p){printf("ERROR: allocation failed!\n");exit(0);}
	
	//subtract previous size
	debug_mem_allocated += size;
	debug_mem_watermark = max(debug_mem_watermark, debug_mem_allocated);
	
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
	debugAllocList[debugAllocListI++]=A;
	#else
	debugAllocList.push_back(A);
	#endif
	return p;
}

void debugfree(const char *file, int line, void *ptr){
	//debugprint(file,line,"memory","[memop:%d] free(%p)\n",debug_mem_op_index,ptr);
	
	bool found = false;
	int I;
	#ifdef DEBUG_NEW
	for(I = debugAllocListSize-1;I >= 0;I--){
	#else
	for(I = debugAllocList.size();I>= 0;I--){
	#endif
		if(debugAllocList[I].reported == ptr){
			if(!debugAllocList[I].freed){
				debugAllocList[I].freed = true;
				debug_mem_allocated -= debugAllocList[I].sizereported;
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

struct debugProfile{
	uint64_t time;
	uint32_t seconds;
	uint32_t milliseconds;
	uint32_t microseconds;
	uint32_t stackused;
	uint32_t heapused;
	uint32_t heapleaked;
} currentDebugProfile;
#define CANARY 247
#define STACK_CANARY_SIZE 10000
debugSetStackCanary(){
	char s[STACK_CANARY_SIZE];
	memset(s,STACK_CANARY_SIZE,CANARY);
}
int debugCheckStackCanary(){
	char s[STACK_CANARY_SIZE];
	for(int I = 0; I < STACK_CANARY_SIZE; I++){
		if(s[I] == CANARY){return I;}
	}
	return STACK_CANARY_SIZE;
}
void profileStart(){
	//debugSetStackCanary();
	currentDebugProfile.time = SDL_GetPerformanceCounter();
	currentDebugProfile.heapused = debug_mem_allocated;
	debug_mem_watermark = debug_mem_allocated;
}
debugProfile profileEnd(){
	uint64_t time = SDL_GetPerformanceCounter() - currentDebugProfile.time;
	uint64_t freq = SDL_GetPerformanceFrequency();
	//int stack = debugCheckStackCanary();
	int seconds = time/freq;
	int milliseconds = time/((double)freq/1000) - 1000*seconds; //(1000*(time - freq*seconds))/freq;
	int microseconds = time/((double)freq/1000000) - 1000000*seconds - 1000*milliseconds; //(1000000*(time - freq*seconds) - 1000*milliseconds)/freq;
	currentDebugProfile.time = 1000000*time/freq;
	currentDebugProfile.seconds = seconds;
	currentDebugProfile.milliseconds = milliseconds;
	currentDebugProfile.microseconds = microseconds;
	//currentDebugProfile.stackused = stack;
	int prevheap = currentDebugProfile.heapused;
	currentDebugProfile.heapused = debug_mem_watermark - prevheap;
	currentDebugProfile.heapleaked = debug_mem_allocated - prevheap;
	return currentDebugProfile;
}




