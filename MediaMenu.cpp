#include "Menu.h"
#include "File.h"
#include <algorithm>

MediaMenu::MediaMenu(Application *application, const char *title, const char *path)
  : Menu(application, title)
{
  if (path) {
    File::listDirectory(path, m_files);
  }

  std::sort(m_files.begin(), m_files.end());

  for (int i=0; i < m_files.size(); i++) {
    if (m_files[i].isDirectory()) {
      new ArrowMenuItem(this, m_files[i].name());
    }
    else {
      new MenuItem(this, m_files[i].name());
    }
  }
}

MediaMenu::~MediaMenu()
{
}

void MediaMenu::selectItem(MenuItem *menuItem)
{
  File &file = m_files[menuItem->index()];
  
  if (file.isDirectory()) {
    selectDirectory(file);
  }
  else {
    selectFile(file);
  }
}

void MediaMenu::selectFile(File &file)
{
}

void MediaMenu::selectDirectory(File &file)
{
}
