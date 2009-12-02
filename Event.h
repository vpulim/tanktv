#ifndef EVENT_H
#define EVENT_H

#include "config.h"
#include <directfb.h>

typedef enum {
  EVENT_KEYPRESS
} EventType;

typedef enum {
#ifdef NMT
  //  KEY_BACK = DIKS_STOP,
  //  KEY_ENTER = DIKS_PLAY,  // DIKS_PAUSE
  KEY_BACK = DIKS_RETURN,
  KEY_ENTER = DIKS_SELECT,
  KEY_PAGE_UP = DIKS_VOLUME_UP,
  KEY_PAGE_DOWN = DIKS_VOLUME_DOWN,
#else
  KEY_BACK = DIKS_ESCAPE,
  KEY_ENTER = DIKS_RETURN,
  KEY_PAGE_UP = DIKS_PAGE_UP,
  KEY_PAGE_DOWN = DIKS_PAGE_DOWN,
#endif
  KEY_UP = DIKS_CURSOR_UP,
  KEY_DOWN = DIKS_CURSOR_DOWN,
  KEY_LEFT = DIKS_CURSOR_LEFT,
  KEY_RIGHT = DIKS_CURSOR_RIGHT
} Key;

struct Event {
  EventType type;
  Key key;
  bool repeat;
};

class EventListener
{
 public:
  virtual bool handleEvent(Event &event) { return true; };
  virtual bool handleIdle() { return true; };
};

#endif
