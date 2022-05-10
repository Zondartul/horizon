



#ifndef _SDL_rwops_h
#define _SDL_rwops_h

#include "SDL_stdinc.h"
#include "SDL_error.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SDL_RWOPS_UNKNOWN   0   
#define SDL_RWOPS_WINFILE   1   
#define SDL_RWOPS_STDFILE   2   
#define SDL_RWOPS_JNIFILE   3   
#define SDL_RWOPS_MEMORY    4   
#define SDL_RWOPS_MEMORY_RO 5   


typedef struct SDL_RWops
{
    
    Sint64 (SDLCALL * size) (struct SDL_RWops * context);

    
    Sint64 (SDLCALL * seek) (struct SDL_RWops * context, Sint64 offset,
                             int whence);

    
    size_t (SDLCALL * read) (struct SDL_RWops * context, void *ptr,
                             size_t size, size_t maxnum);

    
    size_t (SDLCALL * write) (struct SDL_RWops * context, const void *ptr,
                              size_t size, size_t num);

    
    int (SDLCALL * close) (struct SDL_RWops * context);

    Uint32 type;
    union
    {
#if defined(__ANDROID__)
        struct
        {
            void *fileNameRef;
            void *inputStreamRef;
            void *readableByteChannelRef;
            void *readMethod;
            void *assetFileDescriptorRef;
            long position;
            long size;
            long offset;
            int fd;
        } androidio;
#elif defined(__WIN32__)
        struct
        {
            SDL_bool append;
            void *h;
            struct
            {
                void *data;
                size_t size;
                size_t left;
            } buffer;
        } windowsio;
#endif

#ifdef HAVE_STDIO_H
        struct
        {
            SDL_bool autoclose;
            FILE *fp;
        } stdio;
#endif
        struct
        {
            Uint8 *base;
            Uint8 *here;
            Uint8 *stop;
        } mem;
        struct
        {
            void *data1;
            void *data2;
        } unknown;
    } hidden;

} SDL_RWops;





extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFile(const char *file,
                                                  const char *mode);

#ifdef HAVE_STDIO_H
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFP(FILE * fp,
                                                SDL_bool autoclose);
#else
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFP(void * fp,
                                                SDL_bool autoclose);
#endif

extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromMem(void *mem, int size);
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromConstMem(const void *mem,
                                                      int size);


#define RW_SEEK_CUR 1       
#define RW_SEEK_END 2       



#define SDL_RWsize(ctx)         (ctx)->size(ctx)
#define SDL_RWseek(ctx, offset, whence) (ctx)->seek(ctx, offset, whence)
#define SDL_RWtell(ctx)         (ctx)->seek(ctx, 0, RW_SEEK_CUR)
#define SDL_RWread(ctx, ptr, size, n)   (ctx)->read(ctx, ptr, size, n)
#define SDL_RWwrite(ctx, ptr, size, n)  (ctx)->write(ctx, ptr, size, n)
#define SDL_RWclose(ctx)        (ctx)->close(ctx)


extern DECLSPEC Uint8 SDLCALL SDL_ReadU8(SDL_RWops * src);
extern DECLSPEC Uint16 SDLCALL SDL_ReadLE16(SDL_RWops * src);
extern DECLSPEC Uint16 SDLCALL SDL_ReadBE16(SDL_RWops * src);
extern DECLSPEC Uint32 SDLCALL SDL_ReadLE32(SDL_RWops * src);
extern DECLSPEC Uint32 SDLCALL SDL_ReadBE32(SDL_RWops * src);
extern DECLSPEC Uint64 SDLCALL SDL_ReadLE64(SDL_RWops * src);
extern DECLSPEC Uint64 SDLCALL SDL_ReadBE64(SDL_RWops * src);


extern DECLSPEC size_t SDLCALL SDL_WriteU8(SDL_RWops * dst, Uint8 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE16(SDL_RWops * dst, Uint16 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE16(SDL_RWops * dst, Uint16 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE32(SDL_RWops * dst, Uint32 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE32(SDL_RWops * dst, Uint32 value);
extern DECLSPEC size_t SDLCALL SDL_WriteLE64(SDL_RWops * dst, Uint64 value);
extern DECLSPEC size_t SDLCALL SDL_WriteBE64(SDL_RWops * dst, Uint64 value);

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif 


