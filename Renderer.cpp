#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Renderer.h"
#include "Font.h"
#include "Utils.h"

#define VIRTUAL_WIDTH 1280
#define VIRTUAL_HEIGHT 720

Renderer::Renderer(int argc, char **argv)
  : m_initialized(false),
    m_exit(false),
    m_image_loader(NULL),
    m_curr_buffer(0),
    m_scale(1.0)
{
  Font::init();
  if (DirectFBInit(&argc, &argv) != DFB_OK) {
    fprintf(stderr, "Error in DirectFBInit!\n");
  }
  else {
    init();
  }
}

Renderer::~Renderer()
{
  if (m_initialized) destroy();
  for (image_map::const_iterator i=m_image_cache.begin(); i != m_image_cache.end(); i++) {
    Image *image = i->second;
    if (image) {
      if (image->dsc.preallocated[0].data)
	free(image->dsc.preallocated[0].data);
      delete image;    
    }
  }
  for (font_map::const_iterator i=m_font_cache.begin(); i != m_font_cache.end(); i++) {
    if (i->second) delete i->second;    
  }
  Font::finish();
}

void Renderer::init()
{
  if (m_initialized) return;

  DFBSurfaceDescription dsc;
  
  if (DirectFBCreate(&m_dfb) != DFB_OK) {
    fprintf(stderr, "Error in DirectFBCreate!\n"); return;
  }

  if (m_dfb->SetCooperativeLevel(m_dfb, DFSCL_EXCLUSIVE) != DFB_OK) {
    fprintf(stderr, "Error in SetCooperativeLevel!\n"); return;
  }
  
  dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_PIXELFORMAT);
  dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_DOUBLE);
  dsc.pixelformat = (DFBSurfacePixelFormat)DSPF_ARGB;
  
  if (m_dfb->CreateSurface( m_dfb, &dsc, &m_surface ) != DFB_OK) {
    fprintf(stderr, "Error in CreateSurface!\n"); return;
  }
  
  if (m_surface->GetSize (m_surface, &m_width, &m_height) != DFB_OK) {
    fprintf(stderr, "Error in GetSize!\n"); return;
  }

  m_scale = ((float)m_width) / VIRTUAL_WIDTH;
  m_width = VIRTUAL_WIDTH;
  m_height = VIRTUAL_HEIGHT;
  
  m_curr_buffer = 0;

  color(0,0,0,0xff);
  rect(0,0,m_width, m_height); flip();
  rect(0,0,m_width, m_height); flip();

  if (m_dfb->GetInputDevice (m_dfb, INPUT_DEVICE, &m_input) != DFB_OK) {
    fprintf(stderr, "Error in GetInputDevice!\n"); return;    
  }
  
  if (m_input->CreateEventBuffer (m_input, &m_eventBuffer) != DFB_OK) {
    fprintf(stderr, "Error in CreateEventBuffer!\n"); return;    
  }

  // m_image_loader = new ImageLoader();
  // m_image_loader->start();

  m_exit = false;
  m_initialized = true;
}

void Renderer::destroy()
{
  if (!m_initialized) return;

  if (m_image_loader) delete m_image_loader;

  for (image_map::const_iterator i=m_image_cache.begin(); i != m_image_cache.end(); i++) {
    if (i->second && i->second->surface) {
      i->second->surface->Release(i->second->surface);    
      i->second->surface = NULL;
    }
  }

  for (font_map::const_iterator i=m_font_cache.begin(); i != m_font_cache.end(); i++) {
    if (i->second) i->second->clearCache();    
  }

  m_input->Release(m_input);
  m_surface->Release(m_surface);
  m_dfb->Release (m_dfb);

  m_initialized = false;
}

void Renderer::loop(EventListener *listener)
{
  DFBInputEvent dfb_event;
  Event event;

  while (!m_exit) {
    m_eventBuffer->WaitForEventWithTimeout(m_eventBuffer, 0, 100);
    while (!m_exit && m_eventBuffer->GetEvent (m_eventBuffer, DFB_EVENT(&dfb_event)) == DFB_OK) {
      if (dfb_event.type == DIET_KEYPRESS) {
	event.type = EVENT_KEYPRESS;
	event.key = (Key)dfb_event.key_symbol;
	event.repeat = dfb_event.flags & DIEF_REPEAT;
#ifdef NMT
	if (event.key == (Key)DIKS_PAUSE) event.key = (Key)DIKS_PLAY;
#endif
        debug("got key: 0x%x 0x%x\n", (int)(event.key & 0xFF00), (int)(event.key & 0xFF));
	if (!listener->handleEvent(event)) m_exit = true;
      }
    }
    if (!listener->handleIdle()) m_exit = true;
  }    
}

IDirectFBSurface *Renderer::createSurface(DFBSurfaceDescription *dsc)
{
  IDirectFBSurface *surface = NULL;
  if (m_dfb->CreateSurface(m_dfb, dsc, &surface) != DFB_OK) {
    fprintf(stderr, "Error creating surface2!\n");
    surface = NULL;
  }
  return surface;  
}
 
IDirectFBSurface *Renderer::createSurface(int width, int height, int pixelFormat)
{
  if (!width || !height) return NULL;

  DFBSurfaceDescription dsc;
  IDirectFBSurface *surface = NULL;
  dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
  dsc.caps = DSCAPS_NONE;
  dsc.width = width;
  dsc.height = height;
  dsc.pixelformat = (DFBSurfacePixelFormat)pixelFormat;
  if (m_dfb->CreateSurface(m_dfb, &dsc, &surface) != DFB_OK) {
    fprintf(stderr, "Error creating surface!\n");
    surface = NULL;
  }
  return surface;
}

void Renderer::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  m_surface->SetColor(m_surface, r & 0xff, g & 0xff, b & 0xff, a & 0xff);
}

void Renderer::setClip(Box *box)
{
  DFBRegion clip = {box->x, box->y, box->x + box->w - 1, box->y + box->h - 1};  
  scale(&clip.x1);
  scale(&clip.x2);
  scale(&clip.y1);
  scale(&clip.y2);
  m_surface->SetClip(m_surface, &clip);
}

void Renderer::getClip(Box *box)
{
  DFBRegion clip;
  m_surface->GetClip(m_surface, &clip);
  unscale(&clip.x1);
  unscale(&clip.x2);
  unscale(&clip.y1);
  unscale(&clip.y2);
  box->x = clip.x1;
  box->y = clip.y1;
  box->w = clip.x2-clip.x1+1;
  box->h = clip.y2-clip.y1+1;
}

void Renderer::rect(int x, int y, int w, int h)
{
  scale(&x);
  scale(&y);
  scale(&w);
  scale(&h);
  m_surface->FillRectangle(m_surface, x, y, w, h);
}

void Renderer::line(int x1, int y1, int x2, int y2, bool blend)
{
  scale(&x1);
  scale(&y1);
  scale(&x2);
  scale(&y2);
  if (blend) m_surface->SetDrawingFlags(m_surface, DSDRAW_BLEND);
  m_surface->DrawLine(m_surface, x1, y1, x2, y2);
  if (blend) m_surface->SetDrawingFlags(m_surface, DSDRAW_NOFX);
}

Image *Renderer::loadImage(const char *path)
{
  if (!path || !path[0]) return NULL;

  unsigned key = hash(path);

  if (m_image_cache.find(key) == m_image_cache.end()) {
    Image *image = new Image;
    image->surface = NULL;
    image->dsc.preallocated[0].data = NULL;

    IDirectFBImageProvider *provider = NULL;
    DFBSurfaceDescription &dsc = image->dsc;
    if (m_dfb->CreateImageProvider(m_dfb, path, &provider) == DFB_OK) {
      if (provider->GetSurfaceDescription(provider, &dsc) == DFB_OK) {
	scale(&(dsc.width));
	scale(&(dsc.height));
	if ((image->surface = createSurface(&dsc))) {
	  void *data;
	  int pitch;
	  provider->RenderTo(provider, image->surface, NULL);
	  dsc.flags = (DFBSurfaceDescriptionFlags)(dsc.flags | DSDESC_PREALLOCATED);
	  image->surface->Lock(image->surface, DSLF_READ, &data, &pitch);
 	  if ((dsc.preallocated[0].data = malloc(pitch * dsc.height))) {
	    memcpy(dsc.preallocated[0].data, data, pitch * dsc.height);
	    dsc.preallocated[0].pitch = pitch;
	    dsc.preallocated[1].data = NULL;
	    dsc.preallocated[1].pitch = 0;
	  }
	  image->surface->Unlock(image->surface);
	}
	else {
	  debug("CreateSurface failed\n");
	}
      }
      else {
	debug("GetSurfaceDescription failed\n");
      }
      provider->Release(provider);
    }
    else {
      debug("CreateImageProvider failed\n");
    }
    m_image_cache[key] = image;
  }
  return m_image_cache[key];
}

void Renderer::image(int x, int y, const char *path, bool blend) 
{
  fflush(stdout);

  scale(&x);
  scale(&y);

  Image *image = loadImage(path);

  if (image) {
    if (!image->surface && image->dsc.preallocated[0].data) {
      m_dfb->CreateSurface(m_dfb, &image->dsc, &image->surface);
    }
    if (image->surface) {
      if (blend) m_surface->SetBlittingFlags(m_surface, DSBLIT_BLEND_ALPHACHANNEL);
      m_surface->Blit(m_surface, image->surface, NULL, x, y);
      if (blend) m_surface->SetBlittingFlags(m_surface, DSBLIT_NOFX);
    }
  }
}

void Renderer::flip()
{
  m_surface->Flip(m_surface, NULL, DSFLIP_WAITFORSYNC);
  m_curr_buffer = !m_curr_buffer;
}

void Renderer::play(const char *file)
{
  if (!file || !file[0]) return;

  int status;
  pid_t pid;

  destroy();
  if ((pid = fork()) == -1)
    perror("couldn't fork");
  else if (pid == 0)
    execl("/bin/mono", "/bin/mono", "-single", "-nogui", "-dram", "1", file, NULL);
  else if ((pid = wait(&status)) == -1)
    perror("wait error");
  init();
}

void Renderer::font(const char *path, int size)
{
  if (!path || !path[0]) return;

  unsigned key = hash(path) + size;

  scale(&size);

  if (m_font_cache.find(key) == m_font_cache.end()) {
    m_font = new Font(this, path, size);
    if (!m_font->isValid()) { 
      fprintf(stderr, "FreeType: Could not find or load font file.\n");
      delete m_font;
      m_font = NULL;
    }
    m_font_cache[key] = m_font;
  }
  m_font = m_font_cache[key];
}

int Renderer::textWidth(const char *str)
{
  if (!m_font) return 0;

  int width = m_font->width(str);
  unscale(&width);
  return width;
}

void Renderer::text( int x, int y, const char *text, int max_width, FontJustify justify, bool hardclip)
{
  if (!m_font) return;

  scale(&x);
  scale(&y);
  scale(&max_width);
  
  m_font->draw(x, y, text, max_width, justify, hardclip);
}
