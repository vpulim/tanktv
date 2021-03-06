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
