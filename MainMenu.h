#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"

class MainMenu : public Menu
{
 private:
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  MainMenu(Application *application);
};

#endif
