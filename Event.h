#ifndef EVENT_H
#define EVENT_H

#include "config.h"
#include <directfb.h>

typedef enum {
  EVENT_KEYPRESS
} EventType;

typedef enum {
#ifdef NMT
  KEY_BACK = DIKS_RETURN,
  KEY_ENTER = DIKS_SELECT,
#else
  KEY_BACK = DIKS_ESCAPE,
  KEY_ENTER = DIKS_RETURN,
#endif
  KEY_UP = DIKS_CURSOR_UP,
  KEY_DOWN = DIKS_CURSOR_DOWN
} Key;

struct Event {
  EventType type;
  Key key;
};

class EventListener
{
 public:
  virtual bool handleEvent(Event &event) { return true; };
  virtual bool handleIdle() { return true; };
};

#endif
