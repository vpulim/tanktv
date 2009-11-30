#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application, "tankTV")
{
  new ArrowItem(this, "Movies");
  new ArrowItem(this, "TV Shows");
  new ArrowItem(this, "Music");
  new ArrowItem(this, "Downloads");
  new ArrowItem(this, "Files");
  new ArrowItem(this, "Settings");
  new MenuItem(this, "Exit");
}

void MainMenu::selectItem(MenuItem *menuItem)
{
  debug("in MainMenu::m_cb\n");
  if (!strcmp(menuItem->label(), "Movies"))
    m_app->go(new FileMenu(m_app, "Movies", "/share/Video/Movies"));
  else if (!strcmp(menuItem->label(), "TV Shows"))
    m_app->go(new FileMenu(m_app, "TV Shows", "/share/Video/TV Shows"));
  else if (!strcmp(menuItem->label(), "Music"))
    m_app->go(new MusicMenu(m_app));
  else if (!strcmp(menuItem->label(), "Downloads"))
    m_app->go(new FileMenu(m_app, "Downloads", "/share/Download"));
  else if (!strcmp(menuItem->label(), "Files"))
    m_app->go(new FileMenu(m_app, "Files", "/share"));
  else if (!strcmp(menuItem->label(), "Settings"))
    m_app->go(new SettingsMenu(m_app));
  else if (!strcmp(menuItem->label(), "Exit"))
    m_app->exit();
}
