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

class MediaMenu : public Menu
{
 private:
  std::vector<class File> m_files;

 public:
  MediaMenu(Application *application, const char *title="Media", const char *path=NULL);
  virtual ~MediaMenu();
  virtual void selectItem(MenuItem *menuItem);
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
};

class MoviesMenu : public MediaMenu
{
 public:
  MoviesMenu(Application *application, const char *title="Movies", const char *path="/share/Video/Movies");
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
};

class TVShowsMenu : public MediaMenu
{
 public:
  TVShowsMenu(Application *application, const char *title="TV Shows", const char *path="/share/Video/TV Shows");
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
};

class DownloadsMenu : public MediaMenu
{
 public:
  DownloadsMenu(Application *application, const char *title="Downloads", const char *path="/share/Download");
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
};

class Audio;

class MusicMenu : public MediaMenu
{
 public:
  MusicMenu(Application *application, const char *title="Music", const char *path="/share/Music");
  virtual void selectFile(File &file);
  virtual void selectDirectory(File &file);
  virtual bool paintDetails();
};

class SettingsMenu : public Menu
{
 public:
  SettingsMenu(Application *application);
  virtual void paint();
};

#endif
