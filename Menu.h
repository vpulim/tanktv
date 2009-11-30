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
  Menu(Application *application, const char *title);
  virtual ~Menu();
  int current() { return m_current; }
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
};

class SettingsMenu : public Menu
{
 public:
  SettingsMenu(Application *application);
};

#endif
