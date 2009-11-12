#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application, "tankTV")
{
  m_top = 200;
  new ArrowMenuItem(this, "Movies", m_cb, "images/movies.png");
  new ArrowMenuItem(this, "TV Shows", m_cb, "images/tvshows.png");
  new ArrowMenuItem(this, "Music", m_cb, "images/music.png");
  new ArrowMenuItem(this, "Downloads", m_cb, "images/movies.png");
  new ArrowMenuItem(this, "Settings", m_cb, "images/settings.png");

  new MenuItem(this, "Exit", m_cb);
}

void MainMenu::m_cb(Menu *menu, MenuItem *menuItem)
{
  if (menu) {
    Application *app = menu->application();
    Renderer *r = app->renderer();
    debug("in MainMenu::m_cb\n");
    switch (menuItem->index()) {
    case 0:
      app->go(new MoviesMenu(app));
      break;
    case 1:
      app->go(new TVShowsMenu(app));
      break;
    case 2:
      app->go(new MusicMenu(app));
      break;
    case 3:
      app->go(new DownloadsMenu(app));
      break;
    case 4:
      app->go(new SettingsMenu(app));
      break;
    case 5: 
      menu->application()->renderer()->exit();
      break;
    }
  }
}
