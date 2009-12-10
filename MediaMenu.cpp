/*
  Copyright (c) 2009 Vinay Pulim

  This file is part of TankTV.

  TankTV is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  TankTV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.
*/

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
