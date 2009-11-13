#include "Menu.h"
#include "Player.h"
#include "File.h"
#include <algorithm>

MusicMenu::MusicMenu(Application *application, const char *title, const char *path)
  : MediaMenu(application, title, path)
{
}

void MusicMenu::selectFile(File &file)
{
  if (strcmp(m_app->audio()->nowPlaying(), file.path())) {
    debug("opening %s...\n", file.path());
    if (!m_app->audio()->open(file.path())) 
      return;
  }
  m_app->go(new Player(m_app));      
}

void MusicMenu::selectDirectory(File &file)
{
  m_app->go(new MusicMenu(m_app, file.name(), file.path()));      
}

bool MusicMenu::paintDetails()
{
  debug("paintDetails\n");
  return true;
}
