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
 private:
  MenuItem *m_menuitems[MAX_MENU_ITEMS];
  int m_size;
  int m_current;

 public:
  Menu(Application *application);
  ~Menu();
  void add(MenuItem *menuItem);
  virtual bool handleEvent(Event &event);  
  virtual void paint();
};

class MenuItem : public Widget
{
 private:
  const void *m_data;
  MenuItemCallback m_cb;
  int m_index;
  friend void Menu::add(MenuItem *menuItem);

 public:
  MenuItem(const char *label, MenuItemCallback callback=NULL, const void *data=NULL);
  void setCallback(MenuItemCallback callback);
  void select();
  int index() { return m_index; }
  virtual void paint();
};

#endif
