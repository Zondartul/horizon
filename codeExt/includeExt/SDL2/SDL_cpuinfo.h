



#ifndef _SDL_cpuinfo_h
#define _SDL_cpuinfo_h

#include "SDL_stdinc.h"



#if defined(_MSC_VER) && (_MSC_VER >= 1500) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
#ifndef _WIN64
#define __MMX__
#define __3dNOW__
#endif
#define __SSE__
#define __SSE2__
#elif defined(__MINGW64_VERSION_MAJOR)
#include <intrin.h>
#else
#ifdef __ALTIVEC__
#if HAVE_ALTIVEC_H && !defined(__APPLE_ALTIVEC__)
#include <altivec.h>
#undef pixel
#endif
#endif
#ifdef __MMX__
#include <mmintrin.h>
#endif
#ifdef __3dNOW__
#include <mm3dnow.h>
#endif
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#endif

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SDL_CACHELINE_SIZE  128


extern DECLSPEC int SDLCALL SDL_GetCPUCount(void);


extern DECLSPEC int SDLCALL SDL_GetCPUCacheLineSize(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasRDTSC(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasAltiVec(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasMMX(void);


extern DECLSPEC SDL_bool SDLCALL SDL_Has3DNow(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE2(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE3(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE41(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasSSE42(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasAVX2(void);


extern DECLSPEC int SDLCALL SDL_GetSystemRAM(void);



#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 


