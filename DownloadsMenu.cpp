#include "Menu.h"
#include "File.h"
#include <algorithm>

DownloadsMenu::DownloadsMenu(Application *application, const char *title, const char *path)
  : MediaMenu(application, title, path)
{
}

void DownloadsMenu::selectFile(File &file)
{
  if (!m_app->audio()->isStopped()) m_app->audio()->close();
  debug("playing %s\n", file.path());
  m_app->renderer()->play(file.path());
}

void DownloadsMenu::selectDirectory(File &file)
{
  m_app->go(new DownloadsMenu(m_app, file.name(), file.path()));      
}
