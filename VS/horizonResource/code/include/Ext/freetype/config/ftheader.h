#ifndef FTHEADER_H_
#define FTHEADER_H_
#ifdef __cplusplus
#define FT_BEGIN_HEADER  extern "C" {
#else
#define FT_BEGIN_HEADER  
#endif
#ifdef __cplusplus
#define FT_END_HEADER  }
#else
#define FT_END_HEADER  
#endif
#ifndef FT_CONFIG_CONFIG_H
#define FT_CONFIG_CONFIG_H  <Ext/freetype/config/ftconfig.h>
#endif
#ifndef FT_CONFIG_STANDARD_LIBRARY_H
#define FT_CONFIG_STANDARD_LIBRARY_H  <Ext/freetype/config/ftstdlib.h>
#endif
#ifndef FT_CONFIG_OPTIONS_H
#define FT_CONFIG_OPTIONS_H  <Ext/freetype/config/ftoption.h>
#endif
#ifndef FT_CONFIG_MODULES_H
#define FT_CONFIG_MODULES_H  <Ext/freetype/config/ftmodule.h>
#endif
#define FT_FREETYPE_H  <Ext/freetype/freetype.h>
#define FT_ERRORS_H  <Ext/freetype/fterrors.h>
#define FT_MODULE_ERRORS_H  <Ext/freetype/ftmoderr.h>
#define FT_SYSTEM_H  <Ext/freetype/ftsystem.h>
#define FT_IMAGE_H  <Ext/freetype/ftimage.h>
#define FT_TYPES_H  <Ext/freetype/fttypes.h>
#define FT_LIST_H  <Ext/freetype/ftlist.h>
#define FT_OUTLINE_H  <Ext/freetype/ftoutln.h>
#define FT_SIZES_H  <Ext/freetype/ftsizes.h>
#define FT_MODULE_H  <Ext/freetype/ftmodapi.h>
#define FT_RENDER_H  <Ext/freetype/ftrender.h>
#define FT_AUTOHINTER_H  <Ext/freetype/ftautoh.h>
#define FT_CFF_DRIVER_H  <Ext/freetype/ftcffdrv.h>
#define FT_TRUETYPE_DRIVER_H  <Ext/freetype/ftttdrv.h>
#define FT_TYPE1_TABLES_H  <Ext/freetype/t1tables.h>
#define FT_TRUETYPE_IDS_H  <Ext/freetype/ttnameid.h>
#define FT_TRUETYPE_TABLES_H  <Ext/freetype/tttables.h>
#define FT_TRUETYPE_TAGS_H  <Ext/freetype/tttags.h>
#define FT_BDF_H  <Ext/freetype/ftbdf.h>
#define FT_CID_H  <Ext/freetype/ftcid.h>
#define FT_GZIP_H  <Ext/freetype/ftgzip.h>
#define FT_LZW_H  <Ext/freetype/ftlzw.h>
#define FT_BZIP2_H  <Ext/freetype/ftbzip2.h>
#define FT_WINFONTS_H   <Ext/freetype/ftwinfnt.h>
#define FT_GLYPH_H  <Ext/freetype/ftglyph.h>
#define FT_BITMAP_H  <Ext/freetype/ftbitmap.h>
#define FT_BBOX_H  <Ext/freetype/ftbbox.h>
#define FT_CACHE_H  <Ext/freetype/ftcache.h>
#define FT_CACHE_IMAGE_H  FT_CACHE_H
#define FT_CACHE_SMALL_BITMAPS_H  FT_CACHE_H
#define FT_CACHE_CHARMAP_H  FT_CACHE_H
#define FT_MAC_H  <Ext/freetype/ftmac.h>
#define FT_MULTIPLE_MASTERS_H  <Ext/freetype/ftmm.h>
#define FT_SFNT_NAMES_H  <Ext/freetype/ftsnames.h>
#define FT_OPENTYPE_VALIDATE_H  <Ext/freetype/ftotval.h>
#define FT_GX_VALIDATE_H  <Ext/freetype/ftgxval.h>
#define FT_PFR_H  <Ext/freetype/ftpfr.h>
#define FT_STROKER_H  <Ext/freetype/ftstroke.h>
#define FT_SYNTHESIS_H  <Ext/freetype/ftsynth.h>
#define FT_FONT_FORMATS_H  <Ext/freetype/ftfntfmt.h>
#define FT_XFREE86_H  FT_FONT_FORMATS_H
#define FT_TRIGONOMETRY_H  <Ext/freetype/fttrigon.h>
#define FT_LCD_FILTER_H  <Ext/freetype/ftlcdfil.h>
#define FT_UNPATENTED_HINTING_H  <Ext/freetype/ttunpat.h>
#define FT_INCREMENTAL_H  <Ext/freetype/ftincrem.h>
#define FT_GASP_H  <Ext/freetype/ftgasp.h>
#define FT_ADVANCES_H  <Ext/freetype/ftadvanc.h>
#define FT_ERROR_DEFINITIONS_H  <Ext/freetype/fterrdef.h>
#define FT_CACHE_MANAGER_H           <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_MRU_H      <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_MANAGER_H  <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_CACHE_H    <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_GLYPH_H    <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_IMAGE_H    <Ext/freetype/ftcache.h>
#define FT_CACHE_INTERNAL_SBITS_H    <Ext/freetype/ftcache.h>
#define FT_INCREMENTAL_H          <Ext/freetype/ftincrem.h>
#define FT_TRUETYPE_UNPATENTED_H  <Ext/freetype/ttunpat.h>
#ifdef FT2_BUILD_LIBRARY
#define  FT_INTERNAL_INTERNAL_H  <Ext/freetype/internal/internal.h>
#include FT_INTERNAL_INTERNAL_H
#endif 
#endif 
