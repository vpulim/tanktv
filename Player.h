#ifndef PLAYER_H
#define PLAYER_H

#include "Screen.h"
#include "Audio.h"

class Player : public Screen
{
 public:
  Player(Application *application);  
  virtual ~Player();
  virtual void paint();
  virtual bool handleEvent(Event &event);
  virtual bool handleIdle();
};

#endif
