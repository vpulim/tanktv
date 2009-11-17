#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application, "tankTV")
{
  new ArrowItem(this, "Movies");
  new ArrowItem(this, "TV Shows");
  new MusicItem(this, "Music");
  new ArrowItem(this, "Downloads");
  new ArrowItem(this, "Files");
  new ArrowItem(this, "Settings");
  new MenuItem(this, "Exit");
}

void MainMenu::selectItem(MenuItem *menuItem)
{
  debug("in MainMenu::m_cb\n");
  switch (menuItem->index()) {
  case 0:
    m_app->go(new FileMenu(m_app, "Movies", "/share/Video/Movies"));
    break;
  case 1:
    m_app->go(new FileMenu(m_app, "TV Shows", "/share/Video/TV Shows"));
    break;
  case 2:
    break;
  case 3:
    m_app->go(new FileMenu(m_app, "Downloads", "/share/Download"));
    break;
  case 4:
    m_app->go(new FileMenu(m_app, "Files", "/share"));
    break;
  case 5:
    m_app->go(new SettingsMenu(m_app));
    break;
  case 6: 
    m_app->exit();
    break;
  }
}
