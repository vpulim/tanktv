#ifndef EVENT_H
#define EVENT_H

#include "config.h"
#include <directfb.h>

typedef enum {
  EVENT_KEYPRESS
} EventType;

typedef enum {
#ifdef NMT
  KEY_STOP = DIKS_STOP
#else
  KEY_STOP = DIKS_ESCAPE
#endif
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
