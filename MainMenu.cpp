#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application, "NMT TV")
{
  m_top = 200;
  /*
  add(new ArrowMenuItem("Movies", m_cb));
  add(new ArrowMenuItem("TV Shows", m_cb));
  add(new ArrowMenuItem("Music", m_cb));
  add(new ArrowMenuItem("Downloads", m_cb));
  add(new ArrowMenuItem("Settings", m_cb));
  */
  add(new ArrowMenuItem("Movies", m_cb, "images/movies.png"));
  add(new ArrowMenuItem("TV Shows", m_cb, "images/tvshows.png"));
  add(new ArrowMenuItem("Music", m_cb, "images/music.png"));
  add(new ArrowMenuItem("Downloads", m_cb, "images/movies.png"));
  add(new ArrowMenuItem("Settings", m_cb, "images/settings.png"));

  add(new MenuItem("Exit", m_cb));
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
