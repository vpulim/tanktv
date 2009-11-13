#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application, "tankTV")
{
  m_top = 200;
  new ArrowMenuItem(this, "Movies", "images/movies.png");
  new ArrowMenuItem(this, "TV Shows", "images/tvshows.png");
  new ArrowMenuItem(this, "Music", "images/music.png");
  new ArrowMenuItem(this, "Downloads", "images/movies.png");
  new ArrowMenuItem(this, "Settings", "images/settings.png");

  new MenuItem(this, "Exit");
}

void MainMenu::selectItem(MenuItem *menuItem)
{
  debug("in MainMenu::m_cb\n");
  switch (menuItem->index()) {
  case 0:
    m_app->go(new MoviesMenu(m_app));
    break;
  case 1:
    m_app->go(new TVShowsMenu(m_app));
    break;
  case 2:
    m_app->go(new MusicMenu(m_app));
    break;
  case 3:
    m_app->go(new DownloadsMenu(m_app));
    break;
  case 4:
    m_app->go(new SettingsMenu(m_app));
    break;
  case 5: 
    m_app->exit();
    break;
  }
}
