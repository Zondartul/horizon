#ifndef _SDL_touch_h
#define _SDL_touch_h
#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef Sint64 SDL_TouchID;
typedef Sint64 SDL_FingerID;
typedef struct SDL_Finger
{
    SDL_FingerID id;
    float x;
    float y;
    float pressure;
} SDL_Finger;
#define SDL_TOUCH_MOUSEID ((Uint32)-1)
extern DECLSPEC int SDLCALL SDL_GetNumTouchDevices(void);
extern DECLSPEC SDL_TouchID SDLCALL SDL_GetTouchDevice(int index);
extern DECLSPEC int SDLCALL SDL_GetNumTouchFingers(SDL_TouchID touchID);
extern DECLSPEC SDL_Finger * SDLCALL SDL_GetTouchFinger(SDL_TouchID touchID, int index);
#ifdef __cplusplus
}
#endif
#include "close_code.h"
#endif 
