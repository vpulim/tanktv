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

#ifndef MENU_H
#define MENU_H

#include <vector>
#include "Screen.h"
#include "Application.h"

#define MAX_MENU_ITEMS 1000
#define MENU_X 675

class Menu;
class MenuItem;

typedef void (*MenuItemCallback)(Menu *menu, MenuItem *menuItem);

class Menu : public Screen
{
 protected:
  MenuItem *m_menuItems[MAX_MENU_ITEMS];
  int m_size;
  int m_current;
  int m_top;
  int m_idle_count;

 private:
  void getVisibleRange(int *start, int *end);
  void paintBackground(int start, int end, int index, bool eraseOld=false);

 public:
  Menu(Application *application, const char *title=APP_NAME);
  virtual ~Menu();
  int current() { return m_current; }
  MenuItem *currentItem() { return m_current > -1 ? m_menuItems[m_current] : NULL; }
  void add(MenuItem *menuItem);
  virtual void selectItem(MenuItem *menuItem);
  virtual void focusItem(MenuItem *menuItem);
  virtual bool handleEvent(Event &event);  
  virtual bool handleIdle();  
  virtual bool paintDetails(MenuItem *menuItem);
  virtual void paint();
};

#include "MenuItem.h"

class MainMenu : public Menu
{
 public:
  MainMenu(Application *application);
  virtual void selectItem(MenuItem *menuItem);
  virtual void paint();
};

class FileMenu : public Menu
{
 private:
  std::vector<class File> m_files;

 public:
  FileMenu(Application *application, const char *title="Media", const char *path=NULL);
  virtual void selectItem(MenuItem *menuItem);
};

class MusicMenu : public Menu
{
 public:
  MusicMenu(Application *application);
  virtual void selectItem(MenuItem *menuItem);
  virtual bool paintDetails(MenuItem *menuItem);
};

class SettingsMenu : public Menu
{
 public:
  SettingsMenu(Application *application);
  virtual void selectItem(MenuItem *menuItem);
};

#endif
