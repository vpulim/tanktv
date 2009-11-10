#include <string.h>
#include "Font.h"

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

  for (int i=0; i < m_face->num_glyphs; i++) {
    if (FT_Load_Glyph(m_face, i, FT_LOAD_DEFAULT)) continue;
    if (FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL)) continue;    
  }
}

Font::~Font()
{  
  for (glyph_map::const_iterator i=m_glyph_cache.begin(); i != m_glyph_cache.end(); i++) {
    Glyph *glyph = i->second;
    if (glyph) {
      if (glyph->surface)
	glyph->surface->Release(glyph->surface);
      delete glyph;    
    }
  }
  m_glyph_cache.clear();
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
  if (m_glyph_cache.find(index) == m_glyph_cache.end()) {
    Glyph *glyph = NULL;
    if (!FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT) &&
	!FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL)) {
      FT_GlyphSlot  slot = m_face->glyph; 
      FT_Bitmap *bitmap = &slot->bitmap;
      unsigned char *dst, *src = bitmap->buffer;
      int width = bitmap->width;
      int height = bitmap->rows;
      int pitch;
      glyph = new Glyph;
      glyph->surface = m_renderer->createSurface(width, height, DSPF_A8);
      if (glyph->surface) {
	glyph->surface->Lock(glyph->surface, DSLF_WRITE, (void **)&dst, &pitch);
	for(int i=0; i<height; i++) {
	  memcpy(dst, src, width);
	  src += width;
	  dst += pitch;
	}
	glyph->surface->Unlock(glyph->surface);
      }
      glyph->left = slot->bitmap_left;
      glyph->top = slot->bitmap_top;
      glyph->advance_x = slot->advance.x >> 6;
      glyph->advance_y = slot->advance.y >> 6;
    }
    m_glyph_cache[index] = glyph;
  }
  return m_glyph_cache[index];  
}

int Font::decodeUTF8(const char *bytes, long *ucs, int ucs_max_len)
{
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

void Font::draw( int x, int y, const char *text, int max_width)
{
  if (!m_face) return;

  FT_UInt index, previous = 0;
  FT_Bool use_kerning = FT_HAS_KERNING(m_face);
  long ucs[2048];
  Glyph *glyph;
  int num_chars = decodeUTF8(text, ucs, sizeof(ucs)/sizeof(long));
  IDirectFBSurface *primary = m_renderer->surface();

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
}
