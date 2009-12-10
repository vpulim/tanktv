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
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Menu.h"

MainMenu::MainMenu(Application *application)
  : Menu(application)
{
  m_top = 180;
  new ArrowItem(this, "Movies");
  new ArrowItem(this, "TV Shows");
  new ArrowItem(this, "Music");
  new ArrowItem(this, "Downloads");
  new ArrowItem(this, "Files");
  new ArrowItem(this, "Settings");
  new MenuItem(this, "Exit");
}

void MainMenu::selectItem(MenuItem *menuItem)
{
  debug("in MainMenu::m_cb\n");
  if (!strcmp(menuItem->label(), "Movies"))
    m_app->go(new FileMenu(m_app, "Movies", "/share/Video/Movies"));
  else if (!strcmp(menuItem->label(), "TV Shows"))
    m_app->go(new FileMenu(m_app, "TV Shows", "/share/Video/TV Shows"));
  else if (!strcmp(menuItem->label(), "Music"))
    m_app->go(new MusicMenu(m_app));
  else if (!strcmp(menuItem->label(), "Downloads"))
    m_app->go(new FileMenu(m_app, "Downloads", "/share/Download"));
  else if (!strcmp(menuItem->label(), "Files"))
    m_app->go(new FileMenu(m_app, "Files", "/share"));
  else if (!strcmp(menuItem->label(), "Settings"))
    m_app->go(new SettingsMenu(m_app));
  else if (!strcmp(menuItem->label(), "Exit"))
    m_app->exit();
}

void MainMenu::paint()
{
  Menu::paint();
  Renderer *r = m_app->renderer();

  r->color(0,0,0,0xff);
  r->rect(150, 150,400,400);
  if (!strcmp(currentItem()->label(), "Movies"))
    r->image(150, 150, "data/movies.png", false, 2.0);
  else if (!strcmp(currentItem()->label(), "TV Shows"))
    r->image(150, 150, "data/tvshows.png", false, 2.0);
  else if (!strcmp(currentItem()->label(), "Music"))
    r->image(150, 150, "data/music.png", false, 2.0);
  else if (!strcmp(currentItem()->label(), "Downloads"))
    ;
  else if (!strcmp(currentItem()->label(), "Files"))
    ;
  else if (!strcmp(currentItem()->label(), "Settings"))
    r->image(150, 150, "data/settings.png", false, 2.0);
}
