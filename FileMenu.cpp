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
    new FileItem(this, &m_files[i]);
  }
}

FileMenu::~FileMenu()
{
}
