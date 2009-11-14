#ifndef MENU_H
#define MENU_H

#include "Screen.h"
#include "Application.h"
#include <vector>

#define MAX_MENU_ITEMS 1000
#define MENU_X 675

class Menu;
class MenuItem;

typedef void (*MenuItemCallback)(Menu *menu, MenuItem *menuItem);

class Menu : public Screen
{
 protected:
  MenuItem *m_menuitems[MAX_MENU_ITEMS];
  int m_size;
  int m_current;
  int m_top;
  bool m_dirty_back_buffer;
  bool m_dirty_details;

 private:
  void paintBackground();

 public:
  Menu(Application *application, const char *title);
  virtual ~Menu();
  int current() { return m_current; }
  void add(MenuItem *menuItem);
  virtual void selectItem(MenuItem *menuItem);
  virtual bool handleEvent(Event &event);  
  virtual bool handleIdle();  
  virtual bool paintDetails();
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
  virtual ~FileMenu();
  virtual void selectItem(MenuItem *menuItem);
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
};

class SettingsMenu : public Menu
{
 public:
  SettingsMenu(Application *application);
  virtual void paint();
};

#endif
