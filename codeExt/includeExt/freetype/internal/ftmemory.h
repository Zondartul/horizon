#ifndef FTMEMORY_H_
#define FTMEMORY_H_
#include <ft2build.h>
#include FT_CONFIG_CONFIG_H
#include FT_TYPES_H
FT_BEGIN_HEADER
#undef  FT_SET_ERROR
#define FT_SET_ERROR( expression ) \
          ( ( error = (expression) ) != 0 )
#ifdef __cplusplus
  extern "C++"
  template <typename T> inline T*
  cplusplus_typeof(        T*,
                    void  *v )
  {
    return static_cast <T*> ( v );
  }
#define FT_ASSIGNP( p, val )  (p) = cplusplus_typeof( (p), (val) )
#else
#define FT_ASSIGNP( p, val )  (p) = (val)
#endif
#ifdef FT_DEBUG_MEMORY
  FT_BASE( const char* )  _ft_debug_file;
  FT_BASE( long )         _ft_debug_lineno;
#define FT_DEBUG_INNER( exp )  ( _ft_debug_file   = __FILE__, \
                                 _ft_debug_lineno = __LINE__, \
                                 (exp) )
#define FT_ASSIGNP_INNER( p, exp )  ( _ft_debug_file   = __FILE__, \
                                      _ft_debug_lineno = __LINE__, \
                                      FT_ASSIGNP( p, exp ) )
#else 
#define FT_DEBUG_INNER( exp )       (exp)
#define FT_ASSIGNP_INNER( p, exp )  FT_ASSIGNP( p, exp )
#endif 
  FT_BASE( FT_Pointer )
  ft_mem_alloc( FT_Memory  memory,
                FT_Long    size,
                FT_Error  *p_error );
  FT_BASE( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error );
  FT_BASE( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_count,
                  FT_Long    new_count,
                  void*      block,
                  FT_Error  *p_error );
  FT_BASE( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_count,
                   FT_Long    new_count,
                   void*      block,
                   FT_Error  *p_error );
  FT_BASE( void )
  ft_mem_free( FT_Memory    memory,
               const void*  P );
#define FT_MEM_ALLOC( ptr, size )                               \
          FT_ASSIGNP_INNER( ptr, ft_mem_alloc( memory,          \
                                               (FT_Long)(size), \
                                               &error ) )
#define FT_MEM_FREE( ptr )                \
          FT_BEGIN_STMNT                  \
            ft_mem_free( memory, (ptr) ); \
            (ptr) = NULL;                 \
          FT_END_STMNT
#define FT_MEM_NEW( ptr )                        \
          FT_MEM_ALLOC( ptr, sizeof ( *(ptr) ) )
#define FT_MEM_REALLOC( ptr, cursz, newsz )                        \
          FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory,           \
                                                 1,                \
                                                 (FT_Long)(cursz), \
                                                 (FT_Long)(newsz), \
                                                 (ptr),            \
                                                 &error ) )
#define FT_MEM_QALLOC( ptr, size )                               \
          FT_ASSIGNP_INNER( ptr, ft_mem_qalloc( memory,          \
                                                (FT_Long)(size), \
                                                &error ) )
#define FT_MEM_QNEW( ptr )                        \
          FT_MEM_QALLOC( ptr, sizeof ( *(ptr) ) )
#define FT_MEM_QREALLOC( ptr, cursz, newsz )                        \
          FT_ASSIGNP_INNER( ptr, ft_mem_qrealloc( memory,           \
                                                  1,                \
                                                  (FT_Long)(cursz), \
                                                  (FT_Long)(newsz), \
                                                  (ptr),            \
                                                  &error ) )
#define FT_MEM_ALLOC_MULT( ptr, count, item_size )                     \
          FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory,               \
                                                 (FT_Long)(item_size), \
                                                 0,                    \
                                                 (FT_Long)(count),     \
                                                 NULL,                 \
                                                 &error ) )
#define FT_MEM_REALLOC_MULT( ptr, oldcnt, newcnt, itmsz )           \
          FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory,            \
                                                 (FT_Long)(itmsz),  \
                                                 (FT_Long)(oldcnt), \
                                                 (FT_Long)(newcnt), \
                                                 (ptr),             \
                                                 &error ) )
#define FT_MEM_QALLOC_MULT( ptr, count, item_size )                     \
          FT_ASSIGNP_INNER( ptr, ft_mem_qrealloc( memory,               \
                                                  (FT_Long)(item_size), \
                                                  0,                    \
                                                  (FT_Long)(count),     \
                                                  NULL,                 \
                                                  &error ) )
#define FT_MEM_QREALLOC_MULT( ptr, oldcnt, newcnt, itmsz)            \
          FT_ASSIGNP_INNER( ptr, ft_mem_qrealloc( memory,            \
                                                  (FT_Long)(itmsz),  \
                                                  (FT_Long)(oldcnt), \
                                                  (FT_Long)(newcnt), \
                                                  (ptr),             \
                                                  &error ) )
#define FT_MEM_SET_ERROR( cond )  ( (cond), error != 0 )
#define FT_MEM_SET( dest, byte, count )               \
          ft_memset( dest, byte, (FT_Offset)(count) )
#define FT_MEM_COPY( dest, source, count )              \
          ft_memcpy( dest, source, (FT_Offset)(count) )
#define FT_MEM_MOVE( dest, source, count )               \
          ft_memmove( dest, source, (FT_Offset)(count) )
#define FT_MEM_ZERO( dest, count )  FT_MEM_SET( dest, 0, count )
#define FT_ZERO( p )                FT_MEM_ZERO( p, sizeof ( *(p) ) )
#define FT_ARRAY_ZERO( dest, count )                             \
          FT_MEM_ZERO( dest,                                     \
                       (FT_Offset)(count) * sizeof ( *(dest) ) )
#define FT_ARRAY_COPY( dest, source, count )                     \
          FT_MEM_COPY( dest,                                     \
                       source,                                   \
                       (FT_Offset)(count) * sizeof ( *(dest) ) )
#define FT_ARRAY_MOVE( dest, source, count )                     \
          FT_MEM_MOVE( dest,                                     \
                       source,                                   \
                       (FT_Offset)(count) * sizeof ( *(dest) ) )
#define FT_ARRAY_MAX( ptr )           ( FT_INT_MAX / sizeof ( *(ptr) ) )
#define FT_ARRAY_CHECK( ptr, count )  ( (count) <= FT_ARRAY_MAX( ptr ) )
#define FT_MEM_NEW_ARRAY( ptr, count )                              \
          FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory,            \
                                                 sizeof ( *(ptr) ), \
                                                 0,                 \
                                                 (FT_Long)(count),  \
                                                 NULL,              \
                                                 &error ) )
#define FT_MEM_RENEW_ARRAY( ptr, cursz, newsz )                     \
          FT_ASSIGNP_INNER( ptr, ft_mem_realloc( memory,            \
                                                 sizeof ( *(ptr) ), \
                                                 (FT_Long)(cursz),  \
                                                 (FT_Long)(newsz),  \
                                                 (ptr),             \
                                                 &error ) )
#define FT_MEM_QNEW_ARRAY( ptr, count )                              \
          FT_ASSIGNP_INNER( ptr, ft_mem_qrealloc( memory,            \
                                                  sizeof ( *(ptr) ), \
                                                  0,                 \
                                                  (FT_Long)(count),  \
                                                  NULL,              \
                                                  &error ) )
#define FT_MEM_QRENEW_ARRAY( ptr, cursz, newsz )                     \
          FT_ASSIGNP_INNER( ptr, ft_mem_qrealloc( memory,            \
                                                  sizeof ( *(ptr) ), \
                                                  (FT_Long)(cursz),  \
                                                  (FT_Long)(newsz),  \
                                                  (ptr),             \
                                                  &error ) )
#define FT_ALLOC( ptr, size )                           \
          FT_MEM_SET_ERROR( FT_MEM_ALLOC( ptr, size ) )
#define FT_REALLOC( ptr, cursz, newsz )                           \
          FT_MEM_SET_ERROR( FT_MEM_REALLOC( ptr, cursz, newsz ) )
#define FT_ALLOC_MULT( ptr, count, item_size )                           \
          FT_MEM_SET_ERROR( FT_MEM_ALLOC_MULT( ptr, count, item_size ) )
#define FT_REALLOC_MULT( ptr, oldcnt, newcnt, itmsz )              \
          FT_MEM_SET_ERROR( FT_MEM_REALLOC_MULT( ptr, oldcnt,      \
                                                 newcnt, itmsz ) )
#define FT_QALLOC( ptr, size )                           \
          FT_MEM_SET_ERROR( FT_MEM_QALLOC( ptr, size ) )
#define FT_QREALLOC( ptr, cursz, newsz )                           \
          FT_MEM_SET_ERROR( FT_MEM_QREALLOC( ptr, cursz, newsz ) )
#define FT_QALLOC_MULT( ptr, count, item_size )                           \
          FT_MEM_SET_ERROR( FT_MEM_QALLOC_MULT( ptr, count, item_size ) )
#define FT_QREALLOC_MULT( ptr, oldcnt, newcnt, itmsz )              \
          FT_MEM_SET_ERROR( FT_MEM_QREALLOC_MULT( ptr, oldcnt,      \
                                                  newcnt, itmsz ) )
#define FT_FREE( ptr )  FT_MEM_FREE( ptr )
#define FT_NEW( ptr )  FT_MEM_SET_ERROR( FT_MEM_NEW( ptr ) )
#define FT_NEW_ARRAY( ptr, count )                           \
          FT_MEM_SET_ERROR( FT_MEM_NEW_ARRAY( ptr, count ) )
#define FT_RENEW_ARRAY( ptr, curcnt, newcnt )                           \
          FT_MEM_SET_ERROR( FT_MEM_RENEW_ARRAY( ptr, curcnt, newcnt ) )
#define FT_QNEW( ptr )                           \
          FT_MEM_SET_ERROR( FT_MEM_QNEW( ptr ) )
#define FT_QNEW_ARRAY( ptr, count )                          \
          FT_MEM_SET_ERROR( FT_MEM_NEW_ARRAY( ptr, count ) )
#define FT_QRENEW_ARRAY( ptr, curcnt, newcnt )                          \
          FT_MEM_SET_ERROR( FT_MEM_RENEW_ARRAY( ptr, curcnt, newcnt ) )
  FT_BASE( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 const char*  str,
                 FT_Error    *p_error );
  FT_BASE( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              const void*  address,
              FT_ULong     size,
              FT_Error    *p_error );
#define FT_MEM_STRDUP( dst, str )                                            \
          (dst) = (char*)ft_mem_strdup( memory, (const char*)(str), &error )
#define FT_STRDUP( dst, str )                           \
          FT_MEM_SET_ERROR( FT_MEM_STRDUP( dst, str ) )
#define FT_MEM_DUP( dst, address, size )                                    \
          (dst) = ft_mem_dup( memory, (address), (FT_ULong)(size), &error )
#define FT_DUP( dst, address, size )                           \
          FT_MEM_SET_ERROR( FT_MEM_DUP( dst, address, size ) )
  FT_BASE( FT_Int )
  ft_mem_strcpyn( char*        dst,
                  const char*  src,
                  FT_ULong     size );
#define FT_STRCPYN( dst, src, size )                                         \
          ft_mem_strcpyn( (char*)dst, (const char*)(src), (FT_ULong)(size) )
FT_END_HEADER
#endif 
