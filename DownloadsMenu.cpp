#include "Menu.h"
#include "File.h"
#include <algorithm>

DownloadsMenu::DownloadsMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }
  else {
    File::listDirectory("/share/Download", m_files);
  }

  std::sort(m_files.begin(), m_files.end());

  for (int i=0; i < m_files.size(); i++) {
    if (m_files[i].isDirectory()) {
      add(new ArrowMenuItem(m_files[i].name(), m_cb));
    }
    else {
      add(new MenuItem(m_files[i].name(), m_cb));
    }
  }
}

void DownloadsMenu::m_cb(Menu *m, MenuItem *menuItem)
{
  DownloadsMenu *menu = (DownloadsMenu *)m;
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
