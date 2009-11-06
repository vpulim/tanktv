#ifndef SCREEN_H
#define SCREEN_H

#include "Widget.h"

class Application;

class Screen : public Widget
{
 public:
  Screen(Application *application);  
  virtual ~Screen() {};
};

#endif
