#include "Menu.h"
#include "File.h"
#include <algorithm>

MoviesMenu::MoviesMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }
  else {
    File::listDirectory("/share/Video/Movies", m_files);
  }

  std::sort(m_files.begin(), m_files.end());

  for (int i=0; i < m_files.size(); i++) {
    if (m_files[i].isDirectory()) {
      add(new ArrowMenuItem(m_files[i].name(), m_cb, "images/unknown_movie.png", 150, 45));
    }
    else {
      add(new MenuItem(m_files[i].name(), m_cb, "images/unknown_movie.png", 150, 45));
    }
  }
}

void MoviesMenu::m_cb(Menu *m, MenuItem *menuItem)
{
  MoviesMenu *menu = (MoviesMenu *)m;
  Application *app = menu->application();
  if (menu) {
    Renderer *r = app->renderer();
    File &file = menu->m_files[menuItem->index()];
    const char *path = file.path();

    if (file.isDirectory()) {
      app->go(new MoviesMenu(app, file.name(), path));      
    }
    else {
      if (!app->audio()->isStopped()) app->audio()->close();
      debug("playing %s\n", path);
      r->play(path);
    }
  }
}
