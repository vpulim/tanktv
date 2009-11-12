#include "Menu.h"
#include "Player.h"
#include "File.h"
#include <algorithm>

MusicMenu::MusicMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }
  else {
    File::listDirectory("/share/Music", m_files);
    File::listDirectory("/opt/sybhttpd/localhost.drives/NETWORK_SHARE/vpmac-air/Music", m_files);
  }

  std::sort(m_files.begin(), m_files.end());

  for (int i=0; i < m_files.size(); i++) {
    if (m_files[i].isDirectory()) {
      new ArrowMenuItem(this, m_files[i].name(), m_cb, "images/unknown_album.png", 100, 92);
    }
    else {
      new MenuItem(this, m_files[i].name(), m_cb, "images/unknown_album.png", 100, 92);
    }
  }
}

MusicMenu::~MusicMenu()
{
}

void MusicMenu::m_cb(Menu *m, MenuItem *menuItem)
{
  MusicMenu *menu = (MusicMenu *)m;
  Application *app = menu->application();
  if (menu) {
    Renderer *r = app->renderer();
    File &file = menu->m_files[menuItem->index()];
    const char *path = file.path();
    
    if (file.isDirectory()) {
      app->go(new MusicMenu(app, file.name(), path));      
    }
    else {
      if (strcmp(app->audio()->nowPlaying(), path)) {
	debug("opening %s...\n", path);
	if (!app->audio()->open(path)) 
	  return;
      }
      app->go(new Player(app));      
    }
  }
}
