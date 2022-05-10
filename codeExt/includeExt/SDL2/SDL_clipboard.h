



#ifndef _SDL_clipboard_h
#define _SDL_clipboard_h

#include "SDL_stdinc.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif




extern DECLSPEC int SDLCALL SDL_SetClipboardText(const char *text);


extern DECLSPEC char * SDLCALL SDL_GetClipboardText(void);


extern DECLSPEC SDL_bool SDLCALL SDL_HasClipboardText(void);



#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 


