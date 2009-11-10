#ifndef MENU_H
#define MENU_H

#include "Screen.h"
#include "Application.h"
#include <vector>

#define MAX_MENU_ITEMS 1000

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

 public:
  Menu(Application *application, const char *title);
  virtual ~Menu();
  int current() { return m_current; }
  void add(MenuItem *menuItem);
  virtual bool handleEvent(Event &event);  
  virtual void paint();
};

#include "MenuItem.h"

class MainMenu : public Menu
{
 private:
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  MainMenu(Application *application);
};

class MoviesMenu : public Menu
{
 private:
  std::vector<class File> m_files;
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  MoviesMenu(Application *application, const char *title="Movies", const char *path=NULL);
};

class TVShowsMenu : public Menu
{
 private:
  std::vector<class File> m_files;
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  TVShowsMenu(Application *application, const char *title="TV Shows", const char *path=NULL);
};

class DownloadsMenu : public Menu
{
 private:
  std::vector<class File> m_files;
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  DownloadsMenu(Application *application, const char *title="Downloads", const char *path=NULL);
};

class Audio;

class MusicMenu : public Menu
{
 private:
  std::vector<class File> m_files;
  static void m_cb(Menu *menu, MenuItem *menuItem);

 public:
  MusicMenu(Application *application, const char *title="Music", const char *path=NULL);
  virtual ~MusicMenu();
};

class SettingsMenu : public Menu
{
 public:
  SettingsMenu(Application *application);
  virtual void paint();
};

#endif
