#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include <directfb.h>
#include <cairo-directfb.h>
#include <map>
#include "Event.h"
#include "Box.h"

#define IMAGE_CACHE_SIZE 100

using namespace std;

typedef map<const char *, cairo_surface_t *> image_map;

class Renderer
{
 private:
  bool m_initialized;
  IDirectFB *m_dfb;
  IDirectFBSurface *m_surface;
  IDirectFBEventBuffer *m_eventBuffer;
  IDirectFBInputDevice *m_input;
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
  void loop(EventListener *listener);
  void color(double r, double g, double b, double alpha);
  void rect(const Box &box);
  void image(double x, double y, const char *path);
  void flip();
};

#endif
