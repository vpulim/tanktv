#include "Menu.h"
#include "File.h"
#include <algorithm>

TVShowsMenu::TVShowsMenu(Application *application, const char *title, const char *path)
  : MediaMenu(application, title, path)
{
}

void TVShowsMenu::selectFile(File &file)
{
  if (!m_app->audio()->isStopped()) m_app->audio()->close();
  debug("playing %s\n", file.path());
  m_app->renderer()->play(file.path());
}

void TVShowsMenu::selectDirectory(File &file)
{
  m_app->go(new TVShowsMenu(m_app, file.name(), file.path()));      
}
