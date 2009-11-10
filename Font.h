#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "config.h"
#include "Renderer.h"


struct Glyph
{
  IDirectFBSurface *surface;
  int left;
  int top;
  int advance_x;
  int advance_y;
};

typedef std::map<int, Glyph *> glyph_map;

class Font
{
 private:
  static FT_Library m_ft_library;
  FT_Face m_face;
  Renderer *m_renderer;
  glyph_map m_glyph_cache;  
  int decodeUTF8(const char *bytes, long *ucs, int ucs_max_length);

 private:
  Glyph *getGlyph(int index);

 public:
  Font(Renderer *renderer, const char *path, int size);
  ~Font();
  static void init();
  static void finish();
  bool isValid() { return m_face != NULL; }
  void draw(int x, int y, const char *text, int width=0);
};

#endif
