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
  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/assert.hpp>
#include "Renderer.h"
#include "Font.h"
#include "Utils.h"
#include "NMTSettings.h"

#define VIRTUAL_WIDTH 1280
#define VIRTUAL_HEIGHT 720

Renderer::Renderer()
  : m_initialized(false),
    m_exit(false),
    m_image_loader(NULL),
    m_curr_buffer(0),
    m_scale(1.0),
    m_videoMode(-1)
{
  Font::init();
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

void Renderer::initialize(int argc, char **argv, NMTSettings * nmtSettings)
{
  if (DirectFBInit(&argc, &argv) != DFB_OK) {
    fprintf(stderr, "Error in DirectFBInit!\n");
  }
  else {
    BOOST_ASSERT(nmtSettings != NULL);
    m_videoMode = nmtSettings->getVideoMode();
    init();
  }
}

void Renderer::init()
{
  if (m_initialized) return;

  // Set Video mode, if requested.
  // Must be done before DirectFBCreate()!
  if (m_videoMode != -1)
  {
    setVideoMode(m_videoMode);
  }

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

void Renderer::setVideoMode(int videoMode)
{
  int full_width;
  int full_height;
  char dfb_mode[32];
	char * dtv_signal = NULL;
	char * dtv_tv_standard = NULL;
	char * dtv_connector = NULL;
	char * component_signal = NULL;
	char * component_tv_standard = NULL;
	char * component_connector = NULL;
	char * analog_signal = NULL;
	char * analog_tv_standard = NULL;
	char * analog_connector = NULL;

  debug("Set video mode: %d\n", videoMode);

  switch (videoMode)
  {
#if 0
    case (0):
    {
      debug("Mode: Auto\n");

      //! \todo How should this be implemented?
    }
    break;
#endif

    case (1):
    {
      debug("Mode: Composite NTSC\n");

      full_width = 720;
      full_height = 480;
      analog_signal = "ntsc";
      analog_tv_standard = "ntsc";
      analog_connector = "yc";  // or scart ?
    }
    break;

    case (2):
    {
      debug("Mode: Composite PAL\n");

      full_width = 720;
      full_height = 576;
      analog_signal = "pal";
      analog_tv_standard = "pal";
      analog_connector = "yc";  // or scart ?
    }
    break;

    case (3):
    {
      debug("Mode: Component NTSC 480i 60Hz\n");

      full_width = 720;
      full_height = 480;
      component_signal = "edtv";  // is 480 ntsc / 576 pal
      component_tv_standard = "hdtv60";
      component_connector = "ycrcb";
    }
    break;

    case (4):
    {
      debug("Mode: Component PAL 576i 50Hz\n");

      full_width = 720;
      full_height = 576;
      component_signal = "edtv";  // is 480 ntsc / 576 pal
      component_tv_standard = "hdtv50";
      component_connector = "ycrcb";
    }
    break;

    case (5):
    {
      debug("Mode: Component 480p 60Hz\n");

      full_width = 720;
      full_height = 480;
      //! \todo How to force 480p?
      component_signal = "edtv";
      component_tv_standard = "hdtv60";
      component_connector = "ycrcb";
    }
    break;

    case (6):
    {
      debug("Mode: Component 720p 60Hz\n");

      full_width = 1280;
      full_height = 720;
      component_signal = "720p";
      component_tv_standard = "hdtv60";
      component_connector = "ycrcb";
    }
    break;

    case (7):
    {
      debug("Mode: Component 1080p 60Hz\n");

      full_width = 1920;
      full_height = 1080;
      component_signal = "1080p";
      component_tv_standard = "hdtv60";
      component_connector = "ycrcb";
    }
    break;

    case (8):
    {
      debug("Mode: Component 1080i 60Hz\n");

      full_width = 1920;
      full_height = 1080;
      component_signal = "1080i";
      component_tv_standard = "hdtv60";
      component_connector = "ycrcb";
    }
    break;

#if 0
    case (9):
    {
      debug("Mode: HDMI 480p 60Hz\n");

      //! \todo Illegal mode?!? Not supported by DFB?

      full_width = 720;
      full_height = 480;
    }
    break;
#endif

    case (10):
    {
      debug("Mode: HDMI 720p 60Hz\n");

      full_width = 1280;
      full_height = 720;
      dtv_signal = "720p";
      dtv_tv_standard = "hdtv60";
      dtv_connector = "hdmi";
    }
    break;

    case (11):
    {
      debug("Mode: Component 1080p 24Hz\n");

      full_width = 1920;
      full_height = 1080;
      dtv_signal = "1080p24";
      dtv_tv_standard = "hdtv60"; // should this also be set?
      dtv_connector = "hdmi";
    }
    break;

    case (13):
    {
      debug("Mode: Component 720p 50Hz\n");

      full_width = 1280;
      full_height = 720;
      component_signal = "720p";
      component_tv_standard = "hdtv50";
      component_connector = "ycrcb";
    }
    break;

    case (14):
    {
      debug("Mode: Component 1080p 50Hz\n");

      full_width = 1920;
      full_height = 1080;
      component_signal = "1080p";
      component_tv_standard = "hdtv50";
      component_connector = "ycrcb";
    }
    break;

    case (15):
    {
      debug("Mode: Component 1080i 50Hz\n");

      full_width = 1920;
      full_height = 1080;
      component_signal = "1080i";
      component_tv_standard = "hdtv50";
      component_connector = "ycrcb";
    }
    break;

    case (16):
    {
      debug("Mode: HDMI 720p 50Hz\n");

      full_width = 1280;
      full_height = 720;
      dtv_signal = "720p";
      dtv_tv_standard = "hdtv50";
      dtv_connector = "hdmi";
    }
    break;

    case (18):
    {
      debug("Mode: HDMI 1080p 50Hz\n");

      full_width = 1920;
      full_height = 1080;
      dtv_signal = "1080p";
      dtv_tv_standard = "hdtv50";
      dtv_connector = "hdmi";
    }
    break;

    case (29):
    {
      debug("Mode: HDMI 1080p 60Hz\n");

      full_width = 1920;
      full_height = 1080;
      dtv_signal = "1080p";
      dtv_tv_standard = "hdtv60";
      dtv_connector = "hdmi";
    }
    break;

    case (30):
    {
      debug("Mode: Component 576p 50Hz\n");

      full_width = 720;
      full_height = 576;
      //! \todo How to force 576p?
      component_signal = "edtv";
      component_tv_standard = "hdtv50";
      component_connector = "ycrcb";
    }
    break;

    case (31):
    {
      debug("Mode: HDMI 576p 50Hz\n");

      full_width = 720;
      full_height = 576;
      //! \todo Should EDID be used to configure special resolutions?
      dtv_signal = "edid";
      dtv_tv_standard = "hdtv50";
      dtv_connector = "hdmi";
    }
    break;
 
    case (32):
    {
      debug("Mode: HDMI 1080i 60Hz\n");

      full_width = 1920;
      full_height = 1080;
      dtv_signal = "1080i";
      dtv_tv_standard = "hdtv60";
      dtv_connector = "hdmi";
    }
    break;

    default:
    {
      fprintf(stderr, "Unsupported video mode: %d!\n", videoMode);
      return;
    }
    break;
  }

  // Set resolution (mode)
	snprintf(dfb_mode, sizeof(dfb_mode), "%dx%d", full_width, full_height);
  if (DirectFBSetOption("mode", dfb_mode) != DFB_OK)
  {
    fprintf(stderr, "Error setting mode: %s\n", dfb_mode);
    return;
  }

  // Set which signals should be enabled
  
  // dtv
  if (dtv_signal)
  {
		if (DirectFBSetOption ("dtv-signal", dtv_signal) != DFB_OK)
    {
      fprintf(stderr, "Error setting dtv-signal: %s\n", dtv_signal);
      return;
    }

		if (DirectFBSetOption ("dtv-tv-standard", dtv_tv_standard) != DFB_OK)
    {
      fprintf(stderr, "Error setting dtv-tv-standard: %s\n", dtv_tv_standard);
      return;
    }

		if (DirectFBSetOption ("dtv-connector", dtv_connector) != DFB_OK)
    {
      fprintf(stderr, "Error setting dtv-connector: %s\n", dtv_connector);
      return;
    }
  }
  else
  {
    // Disable dtv signal
		if (DirectFBSetOption ("dtv-signal", "none") != DFB_OK)
    {
      fprintf(stderr, "Error disabling dtv-signal\n");
      return;
    }
  }

  // component
  if (component_signal)
  {
		if (DirectFBSetOption ("component-signal", component_signal) != DFB_OK)
    {
      fprintf(stderr, "Error setting component-signal: %s\n", component_signal);
      return;
    }

		if (DirectFBSetOption ("component-tv-standard", component_tv_standard) != DFB_OK)
    {
      fprintf(stderr, "Error setting component-tv-standard: %s\n", component_tv_standard);
      return;
    }

		if (DirectFBSetOption ("component-connector", component_connector) != DFB_OK)
    {
      fprintf(stderr, "Error setting component-connector: %s\n", component_connector);
      return;
    }
  }
  else
  {
    // Disable component signal
		if (DirectFBSetOption ("component-signal", "none") != DFB_OK)
    {
      fprintf(stderr, "Error disabling component-signal\n");
      return;
    }
  }

  // analog
  if (analog_signal)
  {
		if (DirectFBSetOption ("analog-signal", analog_signal) != DFB_OK)
    {
      fprintf(stderr, "Error setting analog-signal: %s\n", analog_signal);
      return;
    }

		if (DirectFBSetOption ("analog-tv-standard", analog_tv_standard) != DFB_OK)
    {
      fprintf(stderr, "Error setting analog-tv-standard: %s\n", analog_tv_standard);
      return;
    }

		if (DirectFBSetOption ("analog-connector", analog_connector) != DFB_OK)
    {
      fprintf(stderr, "Error setting analog-connector: %s\n", analog_connector);
      return;
    }
  }
  else
  {
    // Disable component signal
		if (DirectFBSetOption ("analog-signal", "none") != DFB_OK)
    {
      fprintf(stderr, "Error disabling analog-signal\n");
      return;
    }
  }
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

Image *Renderer::loadImage(const char *path, float scaleFactor, const char *prescaled)
{
  if (!path || !path[0]) return NULL;

  unsigned key = hash(path) + (unsigned)(100*scaleFactor);

  if (m_image_cache.find(key) == m_image_cache.end()) {
    Image *image = new Image;
    image->surface = NULL;
    image->dsc.preallocated[0].data = NULL;
    
    IDirectFBImageProvider *provider = NULL;
    DFBSurfaceDescription &dsc = image->dsc;
    if (prescaled) path = prescaled;
    if (m_dfb->CreateImageProvider(m_dfb, path, &provider) == DFB_OK) {
      if (provider->GetSurfaceDescription(provider, &dsc) == DFB_OK) {
        if (!prescaled) {
          dsc.width = (int)(dsc.width * scaleFactor * m_scale);
          dsc.height = (int)(dsc.height * scaleFactor * m_scale);
        }
        dsc.flags = (DFBSurfaceDescriptionFlags)(dsc.flags | DSDESC_CAPS);
        dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_VIDEOONLY);
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

void Renderer::image(int x, int y, const char *path, bool blend, float scaleFactor) 
{
  fflush(stdout);

  scale(&x);
  scale(&y);

  Image *image = loadImage(path, scaleFactor);

  if (image) {
    if (!image->surface && image->dsc.preallocated[0].data) {
      image->dsc.caps = DSCAPS_NONE;
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
