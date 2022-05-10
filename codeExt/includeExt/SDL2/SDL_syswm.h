



#ifndef _SDL_syswm_h
#define _SDL_syswm_h

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "SDL_version.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef SDL_PROTOTYPES_ONLY
struct SDL_SysWMinfo;
#else

#if defined(SDL_VIDEO_DRIVER_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#if defined(SDL_VIDEO_DRIVER_WINRT)
#include <Inspectable.h>
#endif


#if defined(SDL_VIDEO_DRIVER_X11)
#if defined(__APPLE__) && defined(__MACH__)

#define Cursor X11Cursor
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#if defined(__APPLE__) && defined(__MACH__)

#undef Cursor
#endif

#endif 

#if defined(SDL_VIDEO_DRIVER_DIRECTFB)
#include <directfb.h>
#endif

#if defined(SDL_VIDEO_DRIVER_COCOA)
#ifdef __OBJC__
@class NSWindow;
#else
typedef struct _NSWindow NSWindow;
#endif
#endif

#if defined(SDL_VIDEO_DRIVER_UIKIT)
#ifdef __OBJC__
#include <UIKit/UIKit.h>
#else
typedef struct _UIWindow UIWindow;
typedef struct _UIViewController UIViewController;
#endif
typedef Uint32 GLuint;
#endif

#if defined(SDL_VIDEO_DRIVER_ANDROID)
typedef struct ANativeWindow ANativeWindow;
typedef void *EGLSurface;
#endif


typedef enum
{
    SDL_SYSWM_UNKNOWN,
    SDL_SYSWM_WINDOWS,
    SDL_SYSWM_X11,
    SDL_SYSWM_DIRECTFB,
    SDL_SYSWM_COCOA,
    SDL_SYSWM_UIKIT,
    SDL_SYSWM_WAYLAND,
    SDL_SYSWM_MIR,
    SDL_SYSWM_WINRT,
    SDL_SYSWM_ANDROID
} SDL_SYSWM_TYPE;


struct SDL_SysWMmsg
{
    SDL_version version;
    SDL_SYSWM_TYPE subsystem;
    union
    {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        struct {
            HWND hwnd;                  
            UINT msg;                   
            WPARAM wParam;              
            LPARAM lParam;              
        } win;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
        struct {
            XEvent event;
        } x11;
#endif
#if defined(SDL_VIDEO_DRIVER_DIRECTFB)
        struct {
            DFBEvent event;
        } dfb;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
        struct
        {
            
            int dummy;
            
        } cocoa;
#endif
#if defined(SDL_VIDEO_DRIVER_UIKIT)
        struct
        {
            int dummy;
            
        } uikit;
#endif
        
        int dummy;
    } msg;
};


struct SDL_SysWMinfo
{
    SDL_version version;
    SDL_SYSWM_TYPE subsystem;
    union
    {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        struct
        {
            HWND window;                
            HDC hdc;                    
        } win;
#endif
#if defined(SDL_VIDEO_DRIVER_WINRT)
        struct
        {
            IInspectable * window;      
        } winrt;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
        struct
        {
            Display *display;           
            Window window;              
        } x11;
#endif
#if defined(SDL_VIDEO_DRIVER_DIRECTFB)
        struct
        {
            IDirectFB *dfb;             
            IDirectFBWindow *window;    
            IDirectFBSurface *surface;  
        } dfb;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
        struct
        {
#if defined(__OBJC__) && defined(__has_feature) && __has_feature(objc_arc)
            NSWindow __unsafe_unretained *window; 
#else
            NSWindow *window;                     
#endif
        } cocoa;
#endif
#if defined(SDL_VIDEO_DRIVER_UIKIT)
        struct
        {
#if defined(__OBJC__) && defined(__has_feature) && __has_feature(objc_arc)
            UIWindow __unsafe_unretained *window; 
#else
            UIWindow *window;                     
#endif
            GLuint framebuffer; 
            GLuint colorbuffer; 
            GLuint resolveFramebuffer; 
        } uikit;
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
        struct
        {
            struct wl_display *display;            
            struct wl_surface *surface;            
            struct wl_shell_surface *shell_surface; 
        } wl;
#endif
#if defined(SDL_VIDEO_DRIVER_MIR)
        struct
        {
            struct MirConnection *connection;  
            struct MirSurface *surface;  
        } mir;
#endif

#if defined(SDL_VIDEO_DRIVER_ANDROID)
        struct
        {
            ANativeWindow *window;
            EGLSurface surface;
        } android;
#endif

        
        int dummy;
    } info;
};

#endif 

typedef struct SDL_SysWMinfo SDL_SysWMinfo;



extern DECLSPEC SDL_bool SDLCALL SDL_GetWindowWMInfo(SDL_Window * window,
                                                     SDL_SysWMinfo * info);



#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 


