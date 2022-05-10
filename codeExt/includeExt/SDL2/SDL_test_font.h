



#ifndef _SDL_test_font_h
#define _SDL_test_font_h

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif



#define FONT_CHARACTER_SIZE  8


int SDLTest_DrawCharacter( SDL_Renderer *renderer, int x, int y, char c );


int SDLTest_DrawString( SDL_Renderer * renderer, int x, int y, const char *s );



#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 


