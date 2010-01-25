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

#include <stdio.h>
#include "Menu.h"

class ScanningItem : public InfoItem
{
 public:
  ScanningItem(Menu *menu)
    : InfoItem(menu, "") 
  {
    update();
  }  

  virtual void update()
  {
    if (m_app->indexer()->isIndexing()) {
      char count[100];
      sprintf(count, "Files: %d", m_app->indexer()->indexCount());
      setInfo(count, "Stop Scan");
    }
    else {
      if (strcmp(m_label, "Scan Media"))
        setInfo("", "Scan Media");
    }
  }

  virtual void select() 
  {
    if (m_app->indexer()->isIndexing()) 
      m_app->indexer()->stop();
    else
      m_app->indexer()->start();
    update();
  }
};

class AboutMenu : public Menu
{
public:
  AboutMenu(Application *application) 
    : Menu(application, "About")
  {
    new InfoItem(this, "Software Version", VERSION);
    new InfoItem(this, "Build Date", __DATE__);
  }
};

SettingsMenu::SettingsMenu(Application *application)
  : Menu(application, "Settings")
{  
  new ArrowItem(this, "About");
  new ScanningItem(this);
}

void SettingsMenu::selectItem(MenuItem *menuItem)
{
  if (!strcmp(menuItem->label(), "About"))
    m_app->go(new AboutMenu(m_app));  
}
