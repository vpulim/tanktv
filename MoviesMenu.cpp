#include "Menu.h"
#include "File.h"
#include <algorithm>

MoviesMenu::MoviesMenu(Application *application, const char *title, const char *path)
  : FileMenu(application, title, path)
{
}

void MoviesMenu::selectFile(File &file)
{
  if (!m_app->audio()->isStopped()) m_app->audio()->close();
  debug("playing %s\n", file.path());
  m_app->renderer()->play(file.path());
}

void MoviesMenu::selectDirectory(File &file)
{
  m_app->go(new MoviesMenu(m_app, file.name(), file.path()));      
}
