#include "Menu.h"
#include "File.h"
#include "Player.h"
#include <algorithm>

FileMenu::FileMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }

  std::sort(m_files.begin(), m_files.end());
  for (int i=0; i < m_files.size(); i++) {
    File &file = m_files[i];
    if (file.isDirectory())
      new ArrowItem(this, file.name(), &file);
    else if (file.isAudio() || file.isVideo())
      new MenuItem(this, file.name(), &file);      
  }
}

void FileMenu::selectItem(MenuItem *menuItem)
{
  File *file = (File *)menuItem->data();

  if (file->isDirectory()) {
    m_app->go(new FileMenu(m_app, file->name(), file->path()));      
  }
  else {
    if (file->isVideo()) {
      if (!m_app->audio()->isStopped()) m_app->audio()->close();
      debug("playing %s\n", file->path());
      m_app->renderer()->play(file->path());
    }
    else if (file->isAudio()) {
      if (strcmp(m_app->audio()->nowPlaying(), file->path())) {
	debug("opening %s...\n", file->path());
	if (!m_app->audio()->open(file->path(), "", "", "", "", 0)) 
	  return;
      }
      m_app->go(new Player(m_app));
    }
  }
}
