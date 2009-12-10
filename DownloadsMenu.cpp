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
