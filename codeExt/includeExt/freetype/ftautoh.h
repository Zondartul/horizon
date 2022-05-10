#ifndef FTAUTOH_H_
#define FTAUTOH_H_
#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef FREETYPE_H
#error "freetype.h of FreeType 1 has been loaded!"
#error "Please fix the directory search order for header files"
#error "so that freetype.h of FreeType 2 is found first."
#endif
FT_BEGIN_HEADER
#define FT_AUTOHINTER_SCRIPT_NONE   0
#define FT_AUTOHINTER_SCRIPT_LATIN  1
#define FT_AUTOHINTER_SCRIPT_CJK    2
#define FT_AUTOHINTER_SCRIPT_INDIC  3
  typedef struct  FT_Prop_GlyphToScriptMap_
  {
    FT_Face     face;
    FT_UShort*  map;
  } FT_Prop_GlyphToScriptMap;
  typedef struct  FT_Prop_IncreaseXHeight_
  {
    FT_Face  face;
    FT_UInt  limit;
  } FT_Prop_IncreaseXHeight;
FT_END_HEADER
#endif 
