#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include <directfb.h>
#include <cairo-directfb.h>
#include <map>
#include "Event.h"

#define IMAGE_CACHE_SIZE 100
#ifdef NMT
#define INPUT_DEVICE DIDID_REMOTE
#else
#define INPUT_DEVICE DIDID_KEYBOARD
#endif

#define FONT_NORMAL 0
#define FONT_BOLD 1

using namespace std;

typedef map<const char *, cairo_surface_t *> image_map;

class Renderer
{
 private:
  bool m_initialized;
  bool m_exit;
  IDirectFB *m_dfb;
  IDirectFBSurface *m_surface;
  IDirectFBEventBuffer *m_eventBuffer;
  IDirectFBInputDevice *m_input;
  IDirectFBFont *m_font;
  cairo_surface_t* m_cairoSurface;
  cairo_t *m_cairo;
  int m_width;
  int m_height;
  image_map m_image_cache;

 private:
  void init();
  void destroy();

 public:
  Renderer(int argc, char **argv);
  ~Renderer();
  bool initialized() { return m_initialized; }
  void exit() { m_exit = true; }
  int width() { return m_width; }
  int height() { return m_height; }
  void loop(EventListener *listener);
  void color(double r, double g, double b, double alpha);
  void rect(double x, double y, double w, double h);
  void image(double x, double y, const char *path);
  void font(const char *face, double size = 32, int style = FONT_NORMAL);
  void text(double x, double y, const char *str);
  void flip();
  void execute();
};

#endif
