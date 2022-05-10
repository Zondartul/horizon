

















#ifndef FTCFFDRV_H_
#define FTCFFDRV_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef FREETYPE_H
#error "freetype.h of FreeType 1 has been loaded!"
#error "Please fix the directory search order for header files"
#error "so that freetype.h of FreeType 2 is found first."
#endif


FT_BEGIN_HEADER


  


  


  
#define FT_CFF_HINTING_FREETYPE  0
#define FT_CFF_HINTING_ADOBE     1


  


  

  


FT_END_HEADER


#endif 



