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
