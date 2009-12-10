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

#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "config.h"
#include "Renderer.h"

#define MAX_TEXT_LENGTH 2048

struct Glyph
{
  IDirectFBSurface *surface;
  DFBSurfaceDescription dsc;
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
  int getWidth(long *ucs, int *ucs_length, int max_width=0, bool hardclip=false);

 public:
  Font(Renderer *renderer, const char *path, int size);
  ~Font();
  static void init();
  static void finish();
  bool isValid() { return m_face != NULL; }
  void draw(int x, int y, const char *text, int width=0, FontJustify justify=JUSTIFY_LEFT, bool hardclip=false);
  int width(const char *text);
  void clearCache();
};

#endif
