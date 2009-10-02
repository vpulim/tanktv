#include "MainMenu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application)
{
  add(new MenuItem("Movies", m_cb));
  add(new MenuItem("TV Shows", m_cb));
  add(new MenuItem("Music", m_cb));
  add(new MenuItem("Downloads", m_cb));
  add(new MenuItem("Settings", m_cb));
  add(new MenuItem("Exit", m_cb));
}

void MainMenu::m_cb(Menu *menu, MenuItem *menuItem)
{
  if (menu) {
    Renderer *r = menu->application()->renderer();
    debug("in MainMenu::m_cb\n");
    switch (menuItem->index()) {
    case 4:
      r->execute();
      break;
    case 5: 
      menu->application()->renderer()->exit();
      break;
    }
    menu->setDirty(true);
  }
}
