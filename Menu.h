#ifndef MENU_H
#define MENU_H

#include "Screen.h"
#include "Application.h"

#define MAX_MENU_ITEMS 1000

class Menu;
class MenuItem;

typedef void (*MenuItemCallback)(Menu *menu, MenuItem *menuItem);

class Menu : public Screen
{
 protected:
  MenuItem *m_menuitems[MAX_MENU_ITEMS];
  int m_size;
  int m_current;

 public:
  Menu(Application *application);
  ~Menu();
  int current() { return m_current; }
  void add(MenuItem *menuItem);
  virtual bool handleEvent(Event &event);  
  virtual void paint();
};

#include "MenuItem.h"

#endif
