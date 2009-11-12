#include "Menu.h"
#include "File.h"
#include <algorithm>

TVShowsMenu::TVShowsMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }
  else {
    File::listDirectory("/share/Video/TV Shows", m_files);
  }

  std::sort(m_files.begin(), m_files.end());

  for (int i=0; i < m_files.size(); i++) {
    if (m_files[i].isDirectory()) {
      new ArrowMenuItem(this, m_files[i].name(), m_cb);
    }
    else {
      new MenuItem(this, m_files[i].name(), m_cb);
    }
  }
}

void TVShowsMenu::m_cb(Menu *m, MenuItem *menuItem)
{
  TVShowsMenu *menu = (TVShowsMenu *)m;
  Application *app = menu->application();
  if (menu) {
    Renderer *r = app->renderer();
    File &file = menu->m_files[menuItem->index()];
    const char *path = file.path();

    if (file.isDirectory()) {
      app->go(new TVShowsMenu(app, file.name(), path));      
    }
    else {
      if (!app->audio()->isStopped()) app->audio()->close();
      debug("playing %s\n", path);
      r->play(path);
    }
  }
}
