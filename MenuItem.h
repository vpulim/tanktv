#ifndef MENUITEM_H
#define MENUITEM_H

#include "Widget.h"

#define MENU_ITEM_WIDTH 500
#define MENU_ITEM_HEIGHT 50

class MenuItem : public Widget
{
 public:
  virtual void draw();

 public:
  MenuItem(Widget *parent);
};

#endif
