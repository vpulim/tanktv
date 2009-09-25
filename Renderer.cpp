#include <stdio.h>
#include <stdlib.h>
#include "Renderer.h"

Renderer::Renderer(int argc, char **argv)
  : m_initialized(false)
{
  if (DirectFBInit(&argc, &argv) != DFB_OK) {
    fprintf(stderr, "Error in DirectFBInit!\n");
  }
  else {
    init();
  }
}

Renderer::~Renderer()
{
  for (image_map::const_iterator i=m_image_cache.begin(); i != m_image_cache.end(); i++) {
    if (i->second) cairo_surface_destroy(i->second);
  }

  if (m_initialized) destroy();
}

void Renderer::init()
{
  if (m_initialized) return;

  DFBSurfaceDescription dsc;

  if (DirectFBCreate (&m_dfb) != DFB_OK) {
    fprintf(stderr, "Error in DirectFBCreate!\n"); return;
  }

  if (m_dfb->SetCooperativeLevel(m_dfb, DFSCL_EXCLUSIVE) != DFB_OK) {
    fprintf(stderr, "Error in SetCooperativeLevel!\n"); return;
  }

  dsc.flags = DSDESC_CAPS;
  dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);

  if (m_dfb->CreateSurface( m_dfb, &dsc, &m_surface ) != DFB_OK) {
    fprintf(stderr, "Error in CreateSurface!\n"); return;
  }

  if (m_surface->GetSize (m_surface, &m_width, &m_height) != DFB_OK) {
    fprintf(stderr, "Error in GetSize!\n"); return;
  }

  if (m_surface->Flip (m_surface, NULL, DSFLIP_NONE) != DFB_OK) {
    fprintf(stderr, "Error in Flip!\n"); return;
  }
#ifdef NMT
  if (m_dfb->GetInputDevice (m_dfb, DIDID_REMOTE, &m_input) != DFB_OK) {
#else
  if (m_dfb->GetInputDevice (m_dfb, DIDID_KEYBOARD, &m_input) != DFB_OK) {
#endif
    fprintf(stderr, "Error in GetInputDevice!\n"); return;    
  }
  
  if (m_input->CreateEventBuffer (m_input, &m_eventBuffer) != DFB_OK) {
    fprintf(stderr, "Error in CreateEventBuffer!\n"); return;    
  }

  m_cairoSurface = cairo_directfb_surface_create(m_dfb, m_surface);
  m_cairo = cairo_create(m_cairoSurface);
  cairo_set_tolerance(m_cairo, 1);
  cairo_set_operator(m_cairo, CAIRO_OPERATOR_OVER);

  m_initialized = true;
}

void Renderer::destroy()
{
  if (!m_initialized) return;

  cairo_destroy(m_cairo);
  cairo_surface_destroy(m_cairoSurface);

  m_input->Release(m_input);
  m_surface->Release(m_surface);
  m_dfb->Release (m_dfb);

  m_initialized = false;
}

void Renderer::loop(EventListener *listener)
{
  DFBInputEvent dfb_event;
  Event event;
  bool exit = false;

  while (!exit) {
    while (!exit && m_eventBuffer->GetEvent (m_eventBuffer, DFB_EVENT(&dfb_event)) == DFB_OK) {
      if (dfb_event.type == DIET_KEYPRESS) {
	event.type = EVENT_KEYPRESS;
	event.key = (Key)dfb_event.key_symbol;
	if (!listener->handleEvent(event)) exit = true;
      }
    }
    if (!listener->handleIdle()) exit = true;
  }    
}

 void Renderer::color(double r, double g, double b, double alpha)
{
  cairo_set_source_rgba(m_cairo, r, g, b, alpha);
}

void Renderer::rect(const Box &box)
{
  cairo_rectangle(m_cairo, box.x, box.y, box.w, box.h);
  cairo_fill(m_cairo);  
}

void Renderer::image(double x, double y, const char *path) 
{
  cairo_surface_t *surface;
  cairo_status_t status;

  if (m_image_cache.find(path) == m_image_cache.end()) {
    surface = cairo_image_surface_create_from_png(path);
    status =cairo_surface_status(surface);
    printf("%d\n", status);
    if (status == CAIRO_STATUS_NO_MEMORY ||
	status == CAIRO_STATUS_FILE_NOT_FOUND ||
	status == CAIRO_STATUS_READ_ERROR) 
      surface = NULL;
    m_image_cache[path] = surface;
  }
  surface = m_image_cache[path];
  if (surface) {
    double w = cairo_image_surface_get_width(surface);
    double h = cairo_image_surface_get_height(surface);
    cairo_set_source_surface(m_cairo, surface, x, y);
    cairo_rectangle(m_cairo, x, y, w, h);
    cairo_fill(m_cairo);  
  }
}

void Renderer::flip()
{
  m_surface->Flip(m_surface, NULL, DSFLIP_WAITFORSYNC);
}
