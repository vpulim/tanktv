/*
  Copyright (c) 2009 Vinay Pulim

  This file is part of TankTV.

  TankTV is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  TankTV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "Font.h"
#include "Utils.h"

FT_Library Font::m_ft_library = NULL;

Font::Font(Renderer *renderer, const char *path, int size)
  : m_renderer(renderer),
    m_face(NULL)
{
  if (!path || !path[0]) return;

  FT_Error error;

  if (!m_ft_library) return;
  
  error =  FT_New_Face(m_ft_library, path, 0, &m_face);
  if (error) { 
    m_face = NULL;
    return;
  }
  
  FT_Set_Char_Size( m_face, 0, size*64, 72, 72);

  /*
  debug("Loading font: %s (%d pt)\n", path, size);
  debug("num faces: %d\n", m_face->num_faces);
  debug("face: %d\n", m_face->face_index);
  debug("num sizes: %d\n", m_face->num_fixed_sizes);
  debug("scalable: %d\n", (m_face->face_flags & FT_FACE_FLAG_SCALABLE) != 0);
  debug("fixed sizes: %d\n", (m_face->face_flags & FT_FACE_FLAG_FIXED_SIZES) != 0);
  debug("kerning: %d\n", (m_face->face_flags & FT_FACE_FLAG_KERNING) != 0);
  debug("hinter: %d\n", (m_face->face_flags & FT_FACE_FLAG_HINTER) != 0);
  debug("fast glyphs: %d\n", (m_face->face_flags & FT_FACE_FLAG_FAST_GLYPHS) != 0);
  */

  for (int i=0; i < m_face->num_glyphs; i++) {
    if (FT_Load_Glyph(m_face, i, FT_LOAD_DEFAULT)) continue;
    if (FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL)) continue;    
  }
}

Font::~Font()
{  
  clearCache();

  for (glyph_map::const_iterator i=m_glyph_cache.begin(); i != m_glyph_cache.end(); i++) {
    Glyph *glyph = i->second;
    if (glyph) {
      if (glyph->dsc.preallocated[0].data)
	free(glyph->dsc.preallocated[0].data);
      delete glyph;    
    }
  }
}

void Font::init()
{
  if (!m_ft_library) {
    if (FT_Init_FreeType(&m_ft_library)) { 
      fprintf(stderr, "FreeType: Couldn't init library.\n");
      m_ft_library = NULL;
    }
  }
}

void Font::finish()
{
  if (m_ft_library) {
    FT_Done_FreeType(m_ft_library);
    m_ft_library = NULL;
  }
}

Glyph *Font::getGlyph(int index)
{
  Glyph *glyph = NULL;

  if (m_glyph_cache.find(index) == m_glyph_cache.end()) {
    if (!FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT) &&
	!FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL)) {
      glyph = new Glyph;
      glyph->surface = NULL;

      FT_GlyphSlot  slot = m_face->glyph; 
      FT_Bitmap *bitmap = &slot->bitmap;
      unsigned char *dst, *src = bitmap->buffer;
      int width = bitmap->width;
      int height = bitmap->rows;
      DFBSurfaceDescription &dsc = glyph->dsc;

      dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT | DSDESC_PREALLOCATED);
      dsc.caps = DSCAPS_NONE;
      dsc.width = width;
      dsc.height = height;
      dsc.pixelformat = DSPF_A8;
      if (width && height && (dsc.preallocated[0].data = malloc(width * height))) {
	memcpy(dsc.preallocated[0].data, src, width * height);
	dsc.preallocated[0].pitch = width;
	dsc.preallocated[1].data = NULL;
	dsc.preallocated[1].pitch = 0;
	glyph->surface = m_renderer->createSurface(&dsc);
      }
      else {
	dsc.preallocated[0].data = NULL;
      }
      glyph->left = slot->bitmap_left;
      glyph->top = slot->bitmap_top;
      glyph->advance_x = slot->advance.x >> 6;
      glyph->advance_y = slot->advance.y >> 6;
    }
    m_glyph_cache[index] = glyph;
  }
  if ((glyph = m_glyph_cache[index]))
    if (!glyph->surface && glyph->dsc.preallocated[0].data)
      glyph->surface = m_renderer->createSurface(&glyph->dsc);
  return glyph;
}

int Font::decodeUTF8(const char *bytes, long *ucs, int ucs_max_len)
{
  if (!bytes) return 0;
  int num_bytes = strlen(bytes), num_chars=0, i=0;
  unsigned char *b = (unsigned char *)bytes;
  
  bytes += num_bytes;
  while(b < (unsigned char *)bytes && num_chars < ucs_max_len) {
    if(*b<=127) ucs[num_chars++] = *b++;
    else if(*b<=192) { debug("invalid utf-8 data\n"); b++; }
    else if(*b<=223) ucs[num_chars++] = ((*b++-192)<<6) + (*b++-128);
    else if(*b<=239) ucs[num_chars++] = ((*b++-224)<<12) + ((*b++-128)<<6) + (*b++-128);
    else if(*b<=247) ucs[num_chars++] = ((*b++-240)<<18) + ((*b++-128)<<12) + ((*b++-128)<<6) + (*b++-128);
    else if(*b<=251) 
      ucs[num_chars++] = ((*b++-248)<<24) + ((*b++-128)<<18) + ((*b++-128)<<12) + ((*b++-128)<<6) + (*b++-128);
    else if(*b<=253)
      ucs[num_chars++] = ((*b++-252)<<30) + ((*b++-128)<<24)  + ((*b++-128)<<18) + ((*b++-128)<<12) + ((*b++-128)<<6) + (*b++-128);
    else { debug("invalid utf-8 data\n"); b++; }
  }  
  return num_chars;
}

int Font::getWidth(long *ucs, int *ucs_length, int max_width, bool hardclip)
{
  if (!m_face) return 0;

  FT_UInt index, previous = 0;
  FT_Bool use_kerning = FT_HAS_KERNING(m_face);
  int num_chars = *ucs_length, width = 0, ellipsis_width = 0, ellipsis_n = -999;
  Glyph *glyph;

  if ((glyph = getGlyph(FT_Get_Char_Index( m_face, '.' )))) {
    ellipsis_width = glyph->advance_x * 3;
  }   

  for (int n=0; n < num_chars; n++) {
    if (ellipsis_n < -1 && width + ellipsis_width >= max_width) 
      ellipsis_n = n-1;
    index = FT_Get_Char_Index( m_face, ucs[n] );
    if (use_kerning && previous && index) {
      FT_Vector delta;
      FT_Get_Kerning( m_face, previous, index, ft_kerning_default, &delta );
      width += delta.x >> 6;
    }  
    if ((glyph = getGlyph(index))) {
      width += glyph->advance_x;
    }   
    if (max_width && width > max_width) {
      if (!hardclip) {
	if (ellipsis_n > 0 && ellipsis_n < MAX_TEXT_LENGTH - 4) {
	  ucs[ellipsis_n] = ucs[ellipsis_n+1] = ucs[ellipsis_n+2] = '.';
	  ucs[ellipsis_n+3] = 0;
	  *ucs_length = ellipsis_n + 3;
	}
      }
      return max_width; 	
    }
  }
  return width;
}

int Font::width(const char *text)
{
  if (!text) return 0;
  long ucs[MAX_TEXT_LENGTH];
  int num_chars = decodeUTF8(text, ucs, sizeof(ucs)/sizeof(long));  
  return getWidth(ucs, &num_chars);  
}

void Font::draw( int x, int y, const char *text, int max_width, FontJustify justify, bool hardclip)
{
  if (!m_face) return;

  FT_UInt index, previous = 0;
  FT_Bool use_kerning = FT_HAS_KERNING(m_face);
  long ucs[MAX_TEXT_LENGTH];
  Glyph *glyph;
  int num_chars = decodeUTF8(text, ucs, sizeof(ucs)/sizeof(long));
  IDirectFBSurface *primary = m_renderer->surface();
  DFBRegion clip, textclip;
  int text_width = getWidth(ucs, &num_chars, max_width, hardclip);

  primary->GetClip(primary, &clip);
  primary->GetClip(primary, &textclip);

  switch (justify) {
  case JUSTIFY_LEFT: 
    textclip.x2 = min(clip.x2, x+text_width-1); 
    break;
  case JUSTIFY_RIGHT: 
    textclip.x1 = max(clip.x1, x-text_width+1); 
    textclip.x2 = min(clip.x2, x); 
    x -= text_width; 
    break;
  case JUSTIFY_CENTER: 
    textclip.x1 = max(clip.x1, x-text_width/2); 
    textclip.x2 = min(clip.x2, x+text_width/2-1); 
    x -= text_width/2; 
    break;
  }

  primary->SetClip(primary, &textclip);    
  
  primary->SetBlittingFlags(primary, (DFBSurfaceBlittingFlags)(DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_COLORIZE)); 

  for (int n=0; n < num_chars; n++) {
    index = FT_Get_Char_Index( m_face, ucs[n] );

    if (use_kerning && previous && index) {
      FT_Vector delta;
      FT_Get_Kerning( m_face, previous, index, ft_kerning_default, &delta );
      x += delta.x >> 6;
    }
    
    if ((glyph = getGlyph(index))) {
      if (glyph->surface)
	primary->Blit(primary, glyph->surface, NULL, x + glyph->left, y - glyph->top);
      x += glyph->advance_x;
      y += glyph->advance_y;
    }   
    
    previous = index;
  }

  primary->SetBlittingFlags(primary, DSBLIT_NOFX);
  primary->SetClip(primary, &clip);
}

void Font::clearCache()
{
  for (glyph_map::const_iterator i=m_glyph_cache.begin(); i != m_glyph_cache.end(); i++) {
    Glyph *glyph = i->second;
    if (glyph && glyph->surface) {
      glyph->surface->Release(glyph->surface);
      glyph->surface = NULL;
    }
  }  
}
